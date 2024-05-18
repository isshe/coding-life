# OpenResty 中 HTTP 模块的 `*_by_lua*` 及阶段

目的：

- 了解都有哪些阶段：`*_by_lua*`。
- 了解各个阶段的执行时机，各阶段有什么限制：如是否可以 yield，原因是什么。

所有的 `*_by_lua*`：

> 执行阶段：指的是对应的 Nginx 的阶段

- `init_by_lua*`：可用于加载配置、初始化全局变量等
  - 上下文：无
  - 执行阶段：初始化阶段是在 master 进程中
  - 设置执行阶段的位置：无
  - 是否可以 yield：否，初始化阶段，无需 yield。

- `init_worker_by_lua*`：可用于加载 Lua 模块等
  - 上下文：NGX_HTTP_LUA_CONTEXT_INIT_WORKER
  - 执行阶段：初始化 worker 进程阶段
  - 设置执行阶段的位置：无
  - 是否可以 yield：否，初始化阶段，无需 yield。

- `ssl_client_hello_by_lua*`：可以用于设置 TLS 算法，HTTP 协议版本 (1.1/2/3)。
  - 上下文：NGX_HTTP_LUA_CONTEXT_SSL_CLIENT_HELLO
  - 执行阶段：客户端发来 Client Hello 时后，处理 Client Hello 消息前
  - 设置执行阶段的位置：SSL_CTX_set_client_hello_cb
  - 是否可以 yield：是

- `ssl_session_fetch_by_lua*`：根据客户端提供的 Session ID 查找并恢复会话。
  - 上下文：NGX_HTTP_LUA_CONTEXT_SSL_SESS_FETCH
  - 执行阶段：通常会在 `ssl_certificate_by_lua*` 前调用
  - 设置执行阶段的位置：SSL_CTX_sess_set_get_cb
  - 是否可以 yield：是
  - **注意：使用 TLS session tickets 时，不会执行这个回调。**

- `ssl_certificate_by_lua*`：可用于动态设置服务器证书。
  - 上下文：NGX_HTTP_LUA_CONTEXT_SSL_CERT
  - 执行阶段：客户端发来 Client Hello 后，发送证书前
  - 设置执行阶段的位置：SSL_CTX_set_cert_cb
  - 是否可以 yield：是

- `ssl_session_store_by_lua*`：可用于基于 Session ID 保存 SSL Session。
  - 上下文：NGX_HTTP_LUA_CONTEXT_SSL_SESS_STORE
  - 执行阶段：SSL/TLS 握手完成后
  - 设置执行阶段的位置：SSL_CTX_sess_set_new_cb
  - 是否可以 yield：否，该阶段的目的是将握手过程中得到的会话信息存储到共享存储区。不能 yield，但是仍然可以使用 ngx.timer.at 创建 0 延迟的计时器，以将 SSL 会话数据异步保存到外部服务（redis、memcached）。
  - **注意：使用 TLS session tickets 时，不会执行这个回调。**

- `set_by_lua*`：可用于定义变量。
  - 上下文：NGX_HTTP_LUA_CONTEXT_SET
  - 执行阶段：rewrite 阶段
  - 设置执行阶段的位置：通过 NDK 设置
  - 是否可以 yield：否

- `rewrite_by_lua*`：可用于实现 URL 重写和参数修改等。
  - 上下文：NGX_HTTP_LUA_CONTEXT_REWRITE
  - 执行阶段：rewrite 阶段
  - 设置执行阶段的位置：在 ngx_http_lua_init 中加入到  rewrite 阶段 handlers 中
  - 是否可以 yield：是

- `access_by_lua*`：可用于访问控制，如 IP 黑白名单、权限鉴定等
  - 上下文：NGX_HTTP_LUA_CONTEXT_ACCESS
  - 执行阶段：access 阶段
  - 设置执行阶段的位置：在 ngx_http_lua_init 中加入到 access 阶段 handlers 中
  - 是否可以 yield：是

- `content_by_lua*`：可用于实现动态生成内容等
  - 上下文：NGX_HTTP_LUA_CONTEXT_CONTENT
  - 执行阶段：content 阶段
  - 设置执行阶段的位置：在 ngx_http_lua_content_by_lua 配置解析函数中直接注册 location content handler
  - 是否可以 yield：是

- `balancer_by_lua*`：可用于自定义负载均衡
  - 上下文：NGX_HTTP_LUA_CONTEXT_BALANCER
  - 执行阶段：content 阶段
  - 设置执行阶段的位置：ngx_http_lua_balancer_by_lua 中重设 ngx_http_upstream_module 的 init_upstream 处理函数，进而重设 ngx_http_lua_balancer_get_peer，在里面会调用设置好的 cmd->post 执行 Lua 代码
  - 是否可以 yield：否

- `header_filter_by_lua*`：可用于增删改响应头部
  - 上下文：NGX_HTTP_LUA_CONTEXT_HEADER_FILTER
  - 执行阶段：output-header-filter（响应头过滤）
  - 设置执行阶段的位置：在 ngx_http_lua_init 中加入到相关 filter 链中
  - 是否可以 yield：否。需要进行异步操作使用 ngx.timer.at。以下 API 都被禁用了：
    - Output API functions (e.g., ngx.say and ngx.send_headers)
    - Control API functions (e.g., ngx.redirect and ngx.exec)
    - Subrequest API functions (e.g., ngx.location.capture and ngx. location.capture_multi)
    - Cosocket API functions (e.g., ngx.socket.tcp and ngx.req.socket).

- `body_filter_by_lua*`：可用于修改响应体
  - 上下文：NGX_HTTP_LUA_CONTEXT_BODY_FILTER
  - 执行阶段：output-body-filter（响应体过滤）
  - 设置执行阶段的位置：在 ngx_http_lua_init 中加入到相关 filter 链中
  - 是否可以 yield：否。禁用的 API 情况与 `header_filter_by_lua*` 一样。需要进行异步操作使用 ngx.timer.at。

- `log_by_lua*`：可用于记录日志到文件或发送到 redis 等服务器中
  - 上下文：NGX_HTTP_LUA_CONTEXT_LOG
  - 执行阶段：log 阶段
  - 设置执行阶段的位置：在 ngx_http_lua_init 中加入到 Log 阶段 handlers 中
  - 是否可以 yield：否。禁用的 API 情况与 `header_filter_by_lua*` 一样。需要进行异步操作使用 ngx.timer.at。

- `exit_worker_by_lua*`：可用于 worker 进程退出清理，worker 进程退出前执行
  - 上下文：NGX_HTTP_LUA_CONTEXT_EXIT_WORKER
  - 执行阶段：无
  - 设置执行阶段的位置：无
  - 是否可以 yield：否

- `ngx.timer`
  - 上下文：NGX_HTTP_LUA_CONTEXT_TIMER
  - 是否可以 yield：是
