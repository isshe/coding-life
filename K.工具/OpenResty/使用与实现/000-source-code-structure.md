# Lua Nginx Module 的源码结构

> 常常提醒自己使用总分思想，这不，发现缺少了一些更为全局的内容，现补充如下。
> 以 0.10.21 版本为例

目的

- 大致了解 openresty 是个什么、实现了哪些功能。

```
ngx_lua-0.10.21
├── README.markdown
├── src
│   ├── api
│   │   └── ngx_http_lua_api.h
│   ├── ddebug.h：用于调试的头文件
 - 各个阶段是处理程序，用于支持 `*_by_lua*` 系列指令
│   ├── ngx_http_lua_initby.c
│   ├── ngx_http_lua_initby.h
│   ├── ngx_http_lua_initworkerby.c
│   ├── ngx_http_lua_initworkerby.h
│   ├── ngx_http_lua_ssl_certby.c
│   ├── ngx_http_lua_ssl_certby.h
│   ├── ngx_http_lua_ssl_client_helloby.c
│   ├── ngx_http_lua_ssl_client_helloby.h
│   ├── ngx_http_lua_setby.c
│   ├── ngx_http_lua_setby.h
│   ├── ngx_http_lua_ssl_session_fetchby.c
│   ├── ngx_http_lua_ssl_session_fetchby.h
│   ├── ngx_http_lua_ssl_session_storeby.c
│   ├── ngx_http_lua_ssl_session_storeby.h
│   ├── ngx_http_lua_rewriteby.c
│   ├── ngx_http_lua_rewriteby.h
│   ├── ngx_http_lua_accessby.c
│   ├── ngx_http_lua_accessby.h
│   ├── ngx_http_lua_contentby.c
│   ├── ngx_http_lua_contentby.h
│   ├── ngx_http_lua_bodyfilterby.c
│   ├── ngx_http_lua_bodyfilterby.h
│   ├── ngx_http_lua_exitworkerby.c
│   ├── ngx_http_lua_exitworkerby.h
│   ├── ngx_http_lua_headerfilterby.c
│   ├── ngx_http_lua_headerfilterby.h
│   ├── ngx_http_lua_logby.c
│   ├── ngx_http_lua_logby.h
 - 进程间通讯
│   ├── ngx_http_lua_pipe.c
│   ├── ngx_http_lua_pipe.h
│   ├── ngx_http_lua_semaphore.c
│   ├── ngx_http_lua_semaphore.h
│   ├── ngx_http_lua_shdict.c
│   ├── ngx_http_lua_shdict.h
 - socket 套接字：网络编程
│   ├── ngx_http_lua_socket_tcp.c
│   ├── ngx_http_lua_socket_tcp.h
│   ├── ngx_http_lua_socket_udp.c
│   ├── ngx_http_lua_socket_udp.h
- 其他待分类
│   ├── ngx_http_lua_api.c
│   ├── ngx_http_lua_args.c
│   ├── ngx_http_lua_args.h
│   ├── ngx_http_lua_balancer.c
│   ├── ngx_http_lua_balancer.h
│   ├── ngx_http_lua_cache.c
│   ├── ngx_http_lua_cache.h
│   ├── ngx_http_lua_capturefilter.c
│   ├── ngx_http_lua_capturefilter.h
│   ├── ngx_http_lua_clfactory.c
│   ├── ngx_http_lua_clfactory.h
│   ├── ngx_http_lua_common.h
│   ├── ngx_http_lua_config.c
│   ├── ngx_http_lua_config.h
│   ├── ngx_http_lua_consts.c
│   ├── ngx_http_lua_consts.h
│   ├── ngx_http_lua_control.c
│   ├── ngx_http_lua_control.h
│   ├── ngx_http_lua_coroutine.c
│   ├── ngx_http_lua_coroutine.h
│   ├── ngx_http_lua_ctx.c
│   ├── ngx_http_lua_ctx.h
│   ├── ngx_http_lua_directive.c
│   ├── ngx_http_lua_directive.h
│   ├── ngx_http_lua_exception.c
│   ├── ngx_http_lua_exception.h
│   ├── ngx_http_lua_headers.c
│   ├── ngx_http_lua_headers.h
│   ├── ngx_http_lua_headers_in.c
│   ├── ngx_http_lua_headers_in.h
│   ├── ngx_http_lua_headers_out.c
│   ├── ngx_http_lua_headers_out.h
│   ├── ngx_http_lua_input_filters.c
│   ├── ngx_http_lua_input_filters.h
│   ├── ngx_http_lua_lex.c
│   ├── ngx_http_lua_lex.h
│   ├── ngx_http_lua_log.c
│   ├── ngx_http_lua_log.h
│   ├── ngx_http_lua_log_ringbuf.c
│   ├── ngx_http_lua_log_ringbuf.h
│   ├── ngx_http_lua_misc.c
│   ├── ngx_http_lua_misc.h
│   ├── ngx_http_lua_module.c
│   ├── ngx_http_lua_ndk.c
│   ├── ngx_http_lua_ndk.h
│   ├── ngx_http_lua_output.c
│   ├── ngx_http_lua_output.h
│   ├── ngx_http_lua_pcrefix.c
│   ├── ngx_http_lua_pcrefix.h
│   ├── ngx_http_lua_phase.c
│   ├── ngx_http_lua_probe.h
│   ├── ngx_http_lua_regex.c
│   ├── ngx_http_lua_req_body.c
│   ├── ngx_http_lua_req_body.h
│   ├── ngx_http_lua_req_method.c
│   ├── ngx_http_lua_script.c
│   ├── ngx_http_lua_script.h
│   ├── ngx_http_lua_sleep.c
│   ├── ngx_http_lua_sleep.h
│   ├── ngx_http_lua_ssl.c
│   ├── ngx_http_lua_ssl.h
│   ├── ngx_http_lua_ssl_ocsp.c
│   ├── ngx_http_lua_string.c
│   ├── ngx_http_lua_string.h
│   ├── ngx_http_lua_subrequest.c
│   ├── ngx_http_lua_subrequest.h
│   ├── ngx_http_lua_time.c
│   ├── ngx_http_lua_timer.c
│   ├── ngx_http_lua_timer.h
│   ├── ngx_http_lua_uri.c
│   ├── ngx_http_lua_uri.h
│   ├── ngx_http_lua_uthread.c
│   ├── ngx_http_lua_uthread.h
│   ├── ngx_http_lua_util.c
│   ├── ngx_http_lua_util.h
│   ├── ngx_http_lua_variable.c
│   ├── ngx_http_lua_worker.c
│   ├── ngx_http_lua_worker_thread.c
│   └── ngx_http_lua_worker_thread.h
├── t：测试用例
├── util：小工具
└── valgrind.suppress
```
