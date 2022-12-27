
# 轻量级线程

与前一个文档中介绍的协程不同，轻量级线程不需要自行 resume、yield 来调度协程，因此使用起来更方便，有并行操作时，通常会使用此方式。

> 看是否和此函数强关联：ngx_http_lua_run_thread；
> 如果此文章无法完成对 ngx_http_lua_run_thread 的理解，则另外文章。

目的：

- 轻线程如何使用？
- 轻线程的使用场景？
- 什么是轻线程？
- openresty 的协程与 Luajit 的协程是什么关系？有什么区别？

## 介绍

轻线程是一种由 lua-nginx-module 调度的**特殊的** Lua 协程。`rewrite/access/content_by_lua*` 的 Lua 代码块就是在 lua-nginx-module 自动创建的**样板“轻线程”**中执行，这种样板“轻线程”也称为“入口线程（entry thread）”。

默认情况下，rewrite_by_lua 等 Nginx 处理程序只有在遇到以下情况时才会终止：

- 入口线程和所有用户轻线程都终止了。
- 任意一个轻线程调用了 ngx.exit、ngx.exec、ngx.redirect、ngx.req.set_uri(uri, true)。
- 入口线程触发了 Lua 错误。

不过，用户的轻线程触发了 Lua 错误而终止，并不会导致其他轻线程也终止。

TODO：看到 https://github.com/openresty/lua-nginx-module 的（Due to the limitation in the Nginx subrequest model, it is not allowed to abort a running Nginx subrequest in general.）

## 使用

### ngx.thread.spawn

- 上下文：rewrite_by_lua*, access_by_lua*, content_by_lua*, ngx.timer.*, ssl_certificate_by_lua*, ssl_session_fetch_by_lua*, ssl_client_hello_by_lua*
- 语法：`co = ngx.thread.spawn(func, arg1, arg2, ...)`
    - 参数：
        - func: 是启动协程后需要执行的函数，
        - arg1/arg2...: 是 func 的参数。
    - 返回值：
        - co: 一个 Lua 协程对象
- 作用：创建一个协程，并使用指定参数**立即**执行 func，直到 func 函数返回或出错或遇到 I/O 参数被 yield 了。ngx.thread.spawn 返回后，新创建的“轻线程”通常会在各种 I/O 事件中保持异步运行。



## 实现
