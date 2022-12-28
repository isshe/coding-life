
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

## 总结

- 轻线程的使用场景？

答：主要用于并行操作，如并行发送请求到上游服务器。

- OpenResty 的协程与 Luajit 的协程是什么关系？有什么区别？

## 参考

- https://github.com/openresty/lua-nginx-module#ngxthreadspawn
