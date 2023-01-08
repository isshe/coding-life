
# 轻量级线程

与前一个文档中介绍的协程不同，轻量级线程不需要自行 resume、yield 来调度协程，因此使用起来更方便，有并行操作时，通常会使用此方式。

> 看是否和此函数强关联：ngx_http_lua_run_thread；
> 如果此文章无法完成对 ngx_http_lua_run_thread 的理解，则另外文章。

目的：

- 轻线程如何使用？
- 轻线程的使用场景？
- 什么是轻线程？
- OpenResty 的协程与 Luajit 的协程是什么关系？有什么区别？

## 介绍

轻线程是一种由 lua-nginx-module 调度的**特殊的 Lua 协程**。`rewrite/access/content_by_lua*` 的 Lua 代码块就是在 lua-nginx-module 自动创建的**样板“轻线程”**中执行，这种样板“轻线程”也称为“入口线程（entry thread）”。

默认情况下，rewrite_by_lua 等 Nginx 处理程序只有在遇到以下情况时才会中止：

- 入口线程和所有用户轻线程都中止了。
- 任意一个轻线程调用了 ngx.exit、ngx.exec、ngx.redirect、ngx.req.set_uri(uri, true)。
- 入口线程触发了 Lua 错误。

不过，用户的轻线程触发了 Lua 错误而中止，并不会导致其他轻线程也中止。

由于 Nginx 子请求模型的限制，一般不允许中止一个正在运行的 Nginx 子请求，因此也不允许中止一个正等待一个或多个子请求终止的正在运行的轻线程，而必须使用 ngx.thread.wait 来等待这些轻线程终止。
一个值得注意的例外是，可以使用状态码 ngx.ERROR(-1), 408, 444 或 499 调用 ngx.exit 来中止挂起的子请求。

轻线程不是以抢的方式进行调度的，也就是不会自动执行时间分片。轻线程会一直运行，知道遇到以下情况：

- 一个 I/O 操作无法在一次执行中完成。
- 调用了 coroutine.yield 主动让出执行权。
- 被 Lua 错误中断了，或者调用了 ngx.exit, ngx.exec, ngx.redirect, 或 ngx.req.set_uri(uri, true)。

前 2 种情况中，轻线程通常很快就会被 lua-nginx-module 的调度器重新唤醒。

在轻线程中可以创建轻线程；在 coroutine.create 创建的协程中也可以创建轻线程，创建轻线程的协程/轻线程被称为父协程/轻线。父协程可以调用 ngx.thread.wait 来等待轻线程终止。

此外，在轻线程中，也可以调用 coroutine.status 和 coroutine.yield。
轻线程将成为“僵尸”轻线程，当轻线程已经终止，父协程未终止并且父协程没有调用 ngx.thread.wait 来等待已终止的轻线程。

轻线程最适用于在单个 Nginx 请求处理程序中并发请求上游。（分别发送不同请求到不同的服务器）

## 使用

### ngx.thread.spawn

- 上下文：rewrite_by_lua*, access_by_lua*, content_by_lua*, ngx.timer.*, ssl_certificate_by_lua*, ssl_session_fetch_by_lua*, ssl_client_hello_by_lua*

- 语法：`co = ngx.thread.spawn(func, arg1, arg2, ...)`
    - 参数：
        - func: 是启动协程后需要执行的函数。
        - arg1/arg2...: 是 func 的参数。
    - 返回值：
        - co: 一个 Lua 协程对象

- 作用：创建一个协程，并使用指定参数**立即**执行 func，直到 func 函数返回或出错或遇到 I/O 参数被 yield 了。ngx.thread.spawn 返回后，新创建的“轻线程”通常会在各种 I/O 事件中保持异步运行。

### ngx.thread.wait

- 上下文：rewrite_by_lua*, access_by_lua*, content_by_lua*, ngx.timer.*, ssl_certificate_by_lua*, ssl_session_fetch_by_lua*, ssl_client_hello_by_lua*

- 语法：`ok, res1, res2, ... = ngx.thread.wait(thread1, thread2, ...)`
    - 参数：
        - thread1/thread2/...：需要等待的轻线程，由 ngx.thread.spawn 创建。
    - 返回值：
        - ok：一个布尔值，表示轻线程是否成功终止。
        - res1/res2/...：用户 Lua 函数的返回结果，或者是错误对象。

- 作用：等待一个或多个轻线程，并返回第一个终止（成功或错误）的轻线程的结果（ok 的值）。

### ngx.thread.kill

- 上下文：rewrite_by_lua*, access_by_lua*, content_by_lua*, ngx.timer.*, ssl_certificate_by_lua*, ssl_session_fetch_by_lua*, ssl_client_hello_by_lua*

- 语法：`ok, err = ngx.thread.kill(thread)`
    - 参数：
        - thread：要杀死的轻线程对象。
    - 返回值：
        - ok：杀死轻线程否成功，成功则是 true，否则是 nil。
        - err：杀死轻线程失败时的错误描述字符串。

- 作用：杀死一个**正在运行**的轻线程。

## 实现

可以预想到，相关 Lua 接口还是通过注入的方式注册到 lua-nginx-module 中，我们来追踪一下。

### Lua 接口注入

1. 调用栈

```lua
- main
    \- ngx_init_cycle
        \- ngx_conf_parse
            \- ngx_conf_handler
                \- ngx_http_block
                    \- ngx_http_lua_init
                        \- ngx_http_lua_init_vm
                            \- ngx_http_lua_new_state
                                \- ngx_http_lua_init_globals
                                    \- ngx_http_lua_inject_ngx_api
                                        \- ngx_http_lua_inject_uthread_api
```

调用栈和之前在 [010-lua-vm-init.md](./010-lua-vm-init.md) 中拿到的一样，没什么特殊的地方。

2. ngx_http_lua_inject_uthread_api

```c
void
ngx_http_lua_inject_uthread_api(ngx_log_t *log, lua_State *L)
{
    /* new thread table */
    lua_createtable(L, 0 /* narr */, 3 /* nrec */);

    lua_pushcfunction(L, ngx_http_lua_uthread_spawn);
    lua_setfield(L, -2, "spawn");

    lua_pushcfunction(L, ngx_http_lua_uthread_wait);
    lua_setfield(L, -2, "wait");

    lua_pushcfunction(L, ngx_http_lua_uthread_kill);
    lua_setfield(L, -2, "kill");

    lua_setfield(L, -2, "thread");
}
```

在这个函数中，拿到了 “spawn” 等操作实际调用的 C 函数，我们后续从这些函数着手进行探索。

### ngx.thread.spawn: ngx_http_lua_uthread_spawn

```
- ngx_http_lua_uthread_spawn
    \- lua_gettop：获取栈中元素数量。
    \- ngx_http_lua_get_req：获取请求。
    \- ngx_http_get_module_ctx：获取模块上下文。
    \- ngx_http_lua_coroutine_create_helper：创建协程，和 [011-corountine.md](./011-corountine.md) 用的同一个函数，co 栈中只有一个 entry_func —— 入口函数。
    \- if (n > 1)：如果入口函数有参数。
        \- lua_replace(L, 1)：用 co 掉栈底元素，此时 L 的栈：co arg1 ... argn co。
        \- lua_xmove(L, coctx->co, n - 1)：从 L 移动 n - 1 个元素到 coctx->co 栈中，此市场 coctx->co 的栈：entry_func arg1 ... argn co。
    \- ngx_http_lua_post_thread：加入到 post thread 链表中。
    \- ctx->cur_co_ctx = coctx：设置当前协程上下文成新的协程，下次调度时，会 resume 此协程。
    \- ngx_http_lua_attach_co_ctx_to_L(coctx->co, coctx)：关联 coctx 和 co。
    \- lua_yield：让出当前协程的执行权限。
```

主要做了以下事情：

- 创建新协程
- 配置好堆栈（设置好参数）
- yield 当前协程以开始调度新的协程

### ngx.thread.wait: ngx_http_lua_uthread_wait

```
- ngx_http_lua_uthread_wait
    \- ngx_http_lua_get_req
    \- ngx_http_get_module_ctx
    \- ngx_http_lua_check_context：检查上下文是否是可以 yield 的
    \- nargs = lua_gettop(L)：获取参数数量，参数是协程对象
    \- for (i = 1; i <= nargs; i++)：逐个遍历检查要 wait 的协程
        \- sub_co = lua_tothread(L, i)：获取协程对象
        \- ngx_http_lua_get_co_ctx：后协程上下文
        \- switch (sub_coctx->co_status)：检查协程状态
            \- case NGX_HTTP_LUA_CO_ZOMBIE：是僵尸协程
                \- nrets = lua_gettop(sub_coctx->co)：获取返回值
                \- lua_xmove(sub_coctx->co, L, nrets：设置成当前协程的返回值，作为 wait 方法的返回值
                \- ngx_http_lua_del_thread：从协程列表中删除协程
            \- case NGX_HTTP_LUA_CO_DEAD：已经是终止的协程了
                \- if (i < nargs)：如果还有其他协程需要检查，则继续
                \- else：否则就返回错误
                    \- lua_pushnil
                    \- lua_pushliteral(L, "already waited or killed")
            \- default：协程还活着，则继续等待
    \- lua_yield(L, 0)：让出执行权限，让子协程继续执行
```

此函数做了以下事情：

- 遍历传入的参数，逐个检查是否是僵尸协程，返回第一个僵尸协程；
- 当前检查的协程如果已经终止，则继续检查其他协程；如果是最后一个，则报错返回。
- 如果没有协程终止或是僵尸协程，则让出执行权限，让子协程继续被调度。
    - yield 前并没有设置 ctx->cur_co_ctx，那么是如何进行后续的调度的呢？
        - 说明是子协程让出了执行权限（因为 IO 或其他原因），后续依赖于 Nginx 的事件模型进行调度。

### ngx.thread.kill: ngx_http_lua_uthread_kill

```
- ngx_http_lua_uthread_kill
    \- ngx_http_lua_get_req
    \- ngx_http_get_module_ctx
    \- ngx_http_lua_check_context
    \- sub_co = lua_tothread(L, 1)：获取参数
    \- luaL_argcheck：检查参数是否是协程
    \- ngx_http_lua_get_co_ctx：获取协程的上下文
    \- if (sub_coctx->parent_co_ctx != coctx)：检查当前协程是否是要 kill 协程的父协程，不是不能 kill
    \- if (sub_coctx->pending_subreqs > 0)：检查协程是否还有未处理的请求
    \- switch (sub_coctx->co_status)：检查协程状态
        \- NGX_HTTP_LUA_CO_ZOMBIE：删除协程并返回错误
            \- ngx_http_lua_del_thread
            \- lua_pushnil
            \- lua_pushliteral
        \- NGX_HTTP_LUA_CO_DEAD：直接返回错误
            \- lua_pushnil
            \- lua_pushliteral
        \- default：执行清理函数并删除协程
            \- ngx_http_lua_cleanup_pending_operation
            \- ngx_http_lua_del_thread
            \- lua_pushinteger：设置返回值
```

从这个流程中，我们可以知道，只有父协程能 kill 子协程。

## 总结

- 轻线程的使用场景？

答：主要用于并行操作，如并行发送请求到上游服务器。

- OpenResty 的协程与 Luajit 的协程是什么关系？有什么区别？

答：底层用的就是 Luajit 的协程，只是调度是 lua-nginx-module 进行。

## 参考

- https://github.com/openresty/lua-nginx-module#ngxthreadspawn
