# OpenResty 中的 `*_by_lua*`

目的：

- 了解都有哪些阶段：`*_by_lua*`。
- 了解各个阶段的执行时机。

所有的 `*_by_lua*`：

|名称|作用|执行阶段|设置执行阶段位置|备注|
| --- | --- | --- | --- | --- |
| `init_by_lua*` | 可用于加载配置、初始化全局变量等 | 初始化阶段是在 master 进程中 | |
| `init_worker_by_lua*` | 可用于加载 Lua 模块等 | 初始化 worker 进程阶段 | |
| `ssl_client_hello_by_lua*` | |客户端发来 Client Hello 时后，处理Client Hello 消息前 | SSL_CTX_set_client_hello_cb |
| `ssl_session_fetch_by_lua*` | 根据客户端提供的 Session ID 查找并恢复会话 | 通常会在 `ssl_certificate_by_lua*` 前调用 | SSL_CTX_sess_set_get_cb | 使用 TLS session tickets 时，不会执行这个回调 |
| `ssl_certificate_by_lua*` | 可用于动态设置服务器证书 | 客户端发来 Client Hello 后，发送证书前 | SSL_CTX_set_cert_cb | |
| `ssl_session_store_by_lua*` | 可用于基于 Session ID 保存 SSL Session | SSL/TLS 握手完成后 | SSL_CTX_sess_set_new_cb | |
| `set_by_lua*` | 可用于定义变量 | rewrite 阶段 | 通过 NDK 设置 | |
| `rewrite_by_lua*` | 可用于实现 URL 重写和参数修改等 | rewrite 阶段 | 在 ngx_http_lua_init 中加入到  rewrite 阶段 handlers 中 | |
| `access_by_lua*` | 可用于访问控制，如 IP 黑白名单、权限鉴定等 | access 阶段 | 在 ngx_http_lua_init 中加入到 access 阶段 handlers 中 | |
| `content_by_lua*` | 可用于实现动态生成内容等 | content 阶段 | 在 ngx_http_lua_content_by_lua 配置解析函数中直接注册 location content handler| |
| `balancer_by_lua*` | 可用于自定义负载均衡 | content 阶段 | ngx_http_lua_balancer_by_lua 中重设 ngx_http_upstream_module 的 init_upstream 处理函数，进而重设 ngx_http_lua_balancer_get_peer，在里面会调用设置好的 cmd->post 执行 Lua 代码 | |
| `header_filter_by_lua*` | 可用于增删改响应头部 | content 阶段 | 在 ngx_http_lua_init 中加入到相关 filter 链中 | |
| `body_filter_by_lua*` | 可用于修改响应体 | content 阶段 | 在 ngx_http_lua_init 中加入到相关 filter 链中 | |
| `log_by_lua*` | 可用于记录日志到文件或发送到 redis 等服务器中 | log 阶段 | 在 ngx_http_lua_init 中加入到 Log 阶段 handlers 中 | |
| `exit_worker_by_lua*` | 可用于 worker 进程退出清理，worker 进程退出前执行。
