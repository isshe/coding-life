
# 协程

lua-nginx-module 提供了 coroutine 系列 Lua 接口，用于操作协程。

这个与 ngx_http_lua_new_thread 相关的 lua-nginx-module 内部使用的协程不同，切勿搞混了。

**这系列函数较为少用，需要并行操作时通常使用[轻量级线程（light thread）](./012-ngx-lightthread.md)。**

目的：

- coroutine Lua 接口的使用？主要使用场景？
    - 如前面提到，这个较为少用。lua-nginx-module 支持，主要应该是为了兼容第三方 Lua 模块。
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

### ngx_http_lua_coroutine_create VS lj_cf_coroutine_create

- ngx_http_lua_coroutine_create

    ```lua
    - ngx_http_lua_coroutine_create
        \- ngx_http_lua_get_req(L)：获取请求。
        \- ngx_http_get_module_ctx(r, ngx_http_lua_module)：获取 ctx。
        \- ngx_http_lua_coroutine_create_helper：在 wrap 中也用了，因此封装了个 helper 函数。
            \- luaL_argcheck：参数检查。首先检查 coroutine.create 的传参是否是函数。
            \- ngx_http_lua_check_context：检查这个接口是否能在当前上下文中被调用。这是一个宏，实际上就是对相关标记位进行"按位与"。
            \- ngx_http_lua_get_lua_vm(r, ctx)：获取 Lua VM。
            \- lua_newthread：如果不需要 co_ref 就直接创建线程。在根 Lua State 创建新线程，使之总是 yield 到主线程。
            \- ngx_http_lua_new_cached_thread：如果需要 co_ref（一个整数，用于查找线程，速度快），则调用此函数来创建。在根 Lua State 创建新线程，使之总是 yield 到主线程。
            \- ngx_http_lua_probe_user_coroutine_create：一个 dtrace 挂载点，没有定义 NGX_DTRACE 时，此宏为空。
            \- ngx_http_lua_get_co_ctx：从模块 ctx 中获取协程 ctx，如果没获取到，就创建；获取到就初始化一下。
            \- ngx_http_lua_create_co_ctx：创建协程 ctx。
            \- ngx_http_lua_set_req：把协程和请求关联起来。
            \- ngx_http_lua_attach_co_ctx_to_L：把协程 ctx 关联到协程。
            \- lua_xmove(vm, L, 1)：把协程从主线程移到 L。lua_xmove：从 vm 栈中弹出 1 个元素压到 L 栈中。
            \- lua_pushvalue(L, 1)：把入口函数复制到 L 的栈顶。
            \- lua_xmove(L, co, 1)：把入口函数从 L 移动到协程 co。
    ```

可以看到，与前面一致，新的 coroutine 函数需要 request 和 ngx_http_lua_module 的 ctx。
为了更清晰整个原理，我们完整地跟踪一下栈变化：（无关代码已经省去）

```c
int ngx_http_lua_coroutine_create_helper(lua_State *L, ngx_http_request_t *r,
    ngx_http_lua_ctx_t *ctx, ngx_http_lua_co_ctx_t **pcoctx, int *co_ref)
{
    // L: entry_func
    luaL_argcheck(L, lua_isfunction(L, 1) && !lua_iscfunction(L, 1), 1,
                  "Lua function expected");

    if (co_ref == NULL) {
        // vm: co
        co = lua_newthread(vm);

    } else {
        // vm: coroutines_key registry co co
        lmcf = ngx_http_get_module_main_conf(r, ngx_http_lua_module);
        *co_ref = ngx_http_lua_new_cached_thread(vm, &co, lmcf, 0);
    }

    // co(extdata) = r
    // co(extdata2) = coctx
    ngx_http_lua_set_req(co, r);
    ngx_http_lua_attach_co_ctx_to_L(co, coctx);

    // vm: coroutines_key registry co
    // L: entry_func arg1 ... argn co
    lua_xmove(vm, L, 1);    /* move coroutine from main thread to L */

    if (co_ref) {
        // vm: coroutines_key registry
        lua_pop(vm, 1);  /* pop coroutines */
    }

    // L: entry_func arg1 ... argn co entry_func
    lua_pushvalue(L, 1);    /* copy entry function to top of L*/
    // L: entry_func arg1 ... argn co
    // co: entry_func
    lua_xmove(L, co, 1);    /* move entry function from L to co */
}
```

可以看到，co 栈中只有一个 entry_func（此时 coroutine.create 接口也没有传髯口函数的参数）。

- lj_cf_coroutine_create:

    ```c
    // static int lj_cf_coroutine_create(lua_State *L)
    LJLIB_CF(coroutine_create)
    {
    lua_State *L1;
    if (!(L->base < L->top && tvisfunc(L->base)))
        lj_err_argt(L, 1, LUA_TFUNCTION);
    L1 = lua_newthread(L);
    setfuncV(L, L1->top++, funcV(L->base));
    return 1;
    }
    ```

主要差别是 ngx_http_lua_coroutine_create 依赖 ctx 和 r，并且可能从 cache 中取协程；lj_cf_coroutine_create 直接 lua_newthread 了一个协程。

### ngx_http_lua_coroutine_resume VS lj_ffh_coroutine_resume

- ngx_http_lua_coroutine_resume

    ```lua
    - ngx_http_lua_coroutine_resume
        \- co = lua_tothread(L, 1)：从栈中 index = 1 的地方取出值，转换成 Lua 线程。不是线程会返回 NULL。
        \- luaL_argcheck：检查 co 是否有效。
        \- ngx_http_lua_get_req：获取请求。
        \- ngx_http_get_module_ctx：获取模块 ctx。
        \- ngx_http_lua_check_context：检查这个接口是否能在当前上下文中被调用。
        \- p_coctx = ctx->cur_co_ctx：获取当前 ctx 作为父 ctx。
        \- ngx_http_lua_get_co_ctx：从 co 中获取协程 ctx。
        \- ngx_http_lua_probe_user_coroutine_resume：dtrace 钩子。
        \- if (coctx->co_status != NGX_HTTP_LUA_CO_SUSPENDED)：检查进程状态，如果不是 NGX_HTTP_LUA_CO_SUSPENDED 则不能进行 resume，直接返回错误。
        \- p_coctx->co_status = NGX_HTTP_LUA_CO_NORMAL：设置父协程 ctx 的状态。
        \- coctx->parent_co_ctx = p_coctx：设置协程的父协程 ctx。
        \- coctx->co_status = NGX_HTTP_LUA_CO_RUNNING：设置协程的状态。
        \- ctx->co_op = NGX_HTTP_LUA_USER_CORO_RESUME：设置协程操作是用户进行 RESUME。
        \- ctx->cur_co_ctx = coctx：设置 ctx 的 cur_co_ctx 为即将 resume 的 coctx，当前协程 yield 后，将执行此协程。
        \- lua_yield：yeild 回主线程，然后让主线程来 resume。
    ```

不是应该 resume 吗？这里为什么调用的 lua_yield 呢？看到这里或许会有这样的疑惑。
其实 review、access 等阶段的 Lua 代码，都是通过 ngx_http_lua_run_thread 来执行的，ngx_http_lua_coroutine_resume 这里执行完 lua_yield 后，会回到 ngx_http_lua_run_thread 中，由于状态是 LUA_YIELD 和操作是 NGX_HTTP_LUA_USER_CORO_RESUME，会继续(continue) ngx_http_lua_run_thread 的死循环，进而执行到刚刚要 resume 的协程。

- lj_ffh_coroutine_resume

在 header filter 或 body filter 阶段，都是调用的 Luajit 的 coroutine 接口，都是在 ngx_http_lua_header_filter_by_chunk 中调用 lua_pcall 来执行的，与 lua-nginx-module 的接口不同。
不过 Luajit 的 coroutine.resume 是如何实现的，还没搞清楚，跟踪 lj_ffh_coroutine_resume 并没有发现有被调用，最终调用的 lj_vm_resume 这个汇编接口。后续学习 Luajit 时，再继续探究。
Luajit 的 coroutine.resume 及 coroutine.yield 底层接口看起来都是汇编的（使用 LJLIB_ASM 修饰）。

## 总结

回到我们前面提出的问题，

- coroutine Lua 接口的使用？主要使用场景？
答：这个较为少用。lua-nginx-module 支持，主要应该是为了兼容第三方 Lua 模块。

- coroutine 是如何实现的？
答：不同阶段使用不同的底层函数，access 等阶段时候的是 lua-nginx-module 实现的函数，相关调度也是 lua-nginx-module 接管。header filter 等阶段则是使用 Luajit 实现的函数，相关调度由 Luajit 接管。

- 与 Luajit 的协程是什么关系？
答：最终都是用了 Luajit 的 lua_resume、lua_yield 等函数。

## 疑问

- 如何判断一个 Lua State 是 root Lua State？
- Luajit 中栈是如何分布的呢？1,-1 等索引的值代表什么？
    - 后续见此文档：[1-luajit-stack.md](../../Luajit/设计与实现/1-luajit-stack.md)
- Luajit 的 coroutine.resume 是如何实现的？
