# log_by_lua*

在日志处理阶段执行 Lua 源代码，不会替换当前的访问日志，并在打印日志之前运行。

## 用法

- 上下文：http, server, location, location if
- 阶段：log
- 语法：
    - 与 `init_by_lua*` 类似，不再赘述。
- 注意：
  - 以下 API 无法在此指令中使用：
    - Output API functions (e.g., ngx.say and ngx.send_headers)
    - Control API functions (e.g., ngx.exit)
    - Subrequest API functions (e.g., ngx.location.capture and ngx.location.capture_multi)
    - Cosocket API functions (e.g., ngx.socket.tcp and ngx.req.socket).

## 实现

> 与 [005-rewrite_by_lua](005-rewrite_by_lua.md) 类似，不再赘述。

> header_filter_by_lua*、 body_filter_by_lua* 等都是一样的套路，就不再赘述了。
