
# 协程

lua-nginx-module 提供了 coroutine 系列 Lua 接口，用于操作协程。

这个与 ngx_http_lua_new_thread 相关的 lua-nginx-module 内部使用的协程不同，切勿搞混了。

目的：

- coroutine Lua 接口的使用？主要使用场景？
- coroutine 是如何实现的？
- 与 Luajit 的协程是什么关系？

## 使用

- coroutine.create(f)：创建一个协程并返回此协程，类型为“thread”，f 必须是一个函数。
- coroutine.resume (co [, val1, ···])：开始或继续执行协程 `co`。如果协程执行出错，则返回 false 和错误信息，如果协程执行正常，返回 true。
    - 首次调用会开始调用 f 函数，val1 等作为参数传递给 f。
    - 后续调用会用 yield 的地方继续，val1 等作为 yield 的结果传递给 f。
- coroutine.yield (···)：暂停正在执行的协程的执行。传递给 yield 的参数，将作为 resume 额外的结果返回。
- coroutine.running ()：返回正在运行的协程，主协程正在运行，则返回 nil。
- coroutine.status (co)：返回协程 `co` 的状态。
    - running：协程正在执行
    - suspended：调用了 yield，或者还没开始运行。
    - normal：激活但没有在运行，意味着它正在 resume 其他协程。
    - dead：执行完 f 函数，或者出错了。
- coroutine.wrap (f)：create 和 resume 的包裹函数。创建一个协程，参数 f 必须是一个 Lua 函数，结果也是一个 Lua 函数（假设为 g）。g 的后续每次调用都会 resume 协程，传递给 g 的参数，会作为 resume 的参数（val1 等），resume 返回的结果即是 g 的返回结果。

示例：

```lua
local function f()
    local cnt = 0
    for i = 1, 20 do
        ngx.say("Hello, ", cnt)
        coroutine.yield()
        cnt = cnt + 1
    end
end

local c = coroutine.create(f)
for i = 1, 3 do
    coroutine.resume(c)
    ngx.say("***")
end
```

## 实现

在前一篇文章中，我们了解到 lua-nginx-module 主要是通过注入的方式把 Lua 接口注册到 Lua VM 中，coroutine 当然也不例外，我们来看下。

```lua
- ngx_http_lua_init_vm
    \- ngx_pool_cleanup_add
    \- ngx_http_lua_new_state
        \- ngx_http_lua_init_globals
            \- ngx_http_lua_inject_ngx_api：注入 ngx API，实际上就是创建 ngx 表和填充这个表。
                \- ngx_http_lua_inject_coroutine_api
```

### ngx_http_lua_inject_coroutine_api 的执行流程

ngx_http_lua_inject_coroutine_api 又做了什么哪些呢？我们继续来看下

```lua
- ngx_http_lua_inject_coroutine_api
    \- lua_createtable(L, 0 /* narr */, 16 /* nrec */)：新建一个 coroutine 表
    \- lua_getglobal(L, "coroutine")：把旧的 coroutine 表放到栈顶
    \- lua_getfield(L, -1, "running")：把旧 coroutine["running"] 放到栈顶
    \- lua_setfield(L, -3, "running")：把旧 coroutine["running"] 赋值给 新 coroutine["running"]，并出栈 旧 coroutine["running"]。
        \- 其他函数类似：create -> _create，wrap -> _wrap，resume -> _resume，yield -> _yield，status -> _status
    \- lua_pop(L, 1)：不在需要操作旧的 coroutine 表了，弹出栈。
    \- lua_pushcfunction(L, ngx_http_lua_coroutine_create)：把函数压入栈中
    \- lua_setfield(L, -2, "__create")：设置新 coroutine["__create"] = ngx_http_lua_coroutine_create
        \- 其他函数类似：["wrap"] = ngx_http_lua_coroutine_wrap，["resume"] = ngx_http_lua_coroutine_resume, ...
    \- lua_setglobal(L, "coroutine")：把新 coroutine 设置到全局表中替换旧的
    \- luaL_loadbuffer
    \- lua_pcall：加载并执行一段 Lua 代码（下面贴出），设置新 coroutine 表的 "create"，"resume" 等（不用设置 running，running 没变化）
```

Lua 注入 coroutine create 等接口的 Lua 代码：

```lua
local keys = {'create', 'yield', 'resume', 'status', 'wrap'}
-- ifdef OPENRESTY_LUAJIT
local get_req = require 'thread.exdata'
-- #else
local getfenv = getfenv
-- #endif
for _, key in ipairs(keys) do
    local std = coroutine['_' .. key]
    local ours = coroutine['__' .. key]
    local raw_ctx = ngx._phase_ctx
    coroutine[key] = function (...)
-- #ifdef OPENRESTY_LUAJIT
        local r = get_req()
-- #else
        local r = getfenv(0).__ngx_req
-- #endif
        if r ~= nil then
-- #ifdef OPENRESTY_LUAJIT
            local ctx = raw_ctx()
-- #else
            local ctx = raw_ctx(r)
-- #endif
            -- /* ignore header and body filters */
            if ctx ~= 0x020 and ctx ~= 0x040 then
                return ours(...)
            end
        end
        return std(...)
    end
end
package.loaded.coroutine = coroutine
```

可以看到，在 ngx_http_lua_inject_coroutine_api 中，主要是替换了旧的 coroutine 表成新的 routine 表；新旧的 coroutine 函数（新：__xxx，旧：_xxx），按需调用。
同时，新的 coroutine 函数依赖 request 和 ctx。
也就是说，`ngx.*` 相关函数可以在使用新 coroutine 函数创建的 coroutine 中使用。而 header filter 和 body filter 以及其他没有 request 的调用，则使用的是标准库的 coroutine 函数，没法使用这些函数。

新的 coroutine 函数是怎样的呢？有什么特殊的吗？我们继续看。

### ngx_http_lua_coroutine_create

```
- ngx_http_lua_coroutine_create
    \- ngx_http_lua_get_req(L)：获取请求
    \- ngx_http_get_module_ctx(r, ngx_http_lua_module)：获取 ctx
    \- ngx_http_lua_coroutine_create_helper：在 wrap 中也用了，因此封装了个 helper 函数
        \- luaL_argcheck：参数检查。首先检查 coroutine.create 的传参是否是函数。
        \- ngx_http_lua_check_context：检查这个接口是否能在当前上下文中被调用。这是一个宏，实际上就是对相关标记位进行"按位与"。
        \- ngx_http_lua_get_lua_vm(r, ctx)：获取 Lua VM
        \- TODO

```

可以看到，与前面一致，新的 coroutine 函数需要 request 和 ngx_http_lua_module 的 ctx。
