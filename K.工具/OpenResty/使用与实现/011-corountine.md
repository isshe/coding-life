
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

```c
- ngx_http_lua_init_vm
    \- ngx_pool_cleanup_add
    \- ngx_http_lua_new_state
        \- ngx_http_lua_init_globals
            \- ngx_http_lua_inject_ngx_api：注入 ngx API，实际上就是创建 ngx 表和填充这个表。
                \- ngx_http_lua_inject_coroutine_api
```
