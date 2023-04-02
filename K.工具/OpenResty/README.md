[TOC]

Readme
---

# 配置指令

|指令|参数|默认值|建议值|功能描述|上下文|备注|
|---|---|---|---|---|---|---|
|lua_use_default_type|on/off|on|on|响应时，是否在 Content-type 中使用默认的类型|||
|lua_malloc_trim|num|1000||指定处理 num 个请求后调用libc的 malloc_trim，归还空闲内存给系统，以最小化内存占用。0 禁用。|||
|lua_need_request_body|on/off|off|off|要求 openresty 在处理前强制读取请求体数据|||
|lua_http10_buffering|on/off|on|off|启用禁用 HTTP 1.0 的缓冲机制|||
|rewrite_by_lua_on_postpone|on/off|off|off|是否让 rewrite_by_lua 在 rewrite 阶段的最后执行|||
|access_by_lua_on_postpone|on/off|off|off|是否让 access_by_lua 在 access 阶段的最后执行|||
|lua_transform_underscores_in_response_headers|on/off|on|on|是否把 Lua 代码里响应头名字的`_`转换成`-`|||
|lua_check_client_abort|on/off|off||客户端意外断连检测|||
|cosocket|||||||
|lua_socket_log_errors|on/off|on|on|当 cosocket 对象发生错误时，是否记录日志|||
|lua_socket_send_lowat||0||发送数据低水位阈值，超过后才发送|||
|lua_socket_buffer_size||4/8KB||指定 cosocket 对象接收数据的缓冲区大小|||
|lua_socket_connect_timeout|time|60s||连接后端的超时时间，可适当减小|||
|lua_socket_send_timeout|time|60s||发送数据的超时时间，可适当减小|||
|lua_socket_read_timeout||60s||接收数据的超时时间，可适当减小|||
|lua_socket_pool_size|num|30||cosocket 内存池大小，可适当增大|||
|lua_socket_keepalive_timeout|time|60s||cosocket空闲时间|||
|共享内存|||||||
|lua_shared_dict|dict size|||定义名为 dict大小为 size 的共享内存|http||
|定时器|||||||
|lua_max_pending_timers|num|1024||最大待运行的定时任务|||
|lua_max_running_timers||256||最大正在运行的定时任务|||
||||||||
||||||||
||||||||
||||||||
||||||||
||||||||
||||||||



# 访问后端的方式

* 子请求：location.capture

* 协程套接字：cosocket

## location.capture

基于 Nginx 平台内部的子请求机制，需要配合 Nginx 反向代理模块（如 ngx_proxy/ngx_redis2 等）“间接地”访问后端服务，接口参数较多，调用成本略高。

接口说明：

```lua
ngx.location.capture(uri, options)
阶段：rewrite_by_lua, access_by_lua, content_by_lua
options:
-- method: 子请求的方法
-- args: 子请求 URI 参数
-- body: 子请求 body 数据，必须是 lua 字符串
-- ctx: 子请求使用的 ngx.ctx 临时数据
-- vars: 子请求可能用到的变量，存储在表里
-- copy_all_vars:
-- share_all_vars:
-- always_forward_body:
```

# 疑问

* ngx.sleep()及其他 cosocket 相关的函数不能用在 init_by_lua/init_worker_by_lua/set_by_lua/header_filter_by_lua/body_filter_by_lua/log_by_lua 的原因是什么?

详见 [ngx.sleep](使用与实现/016-ngx-sleep.md)。

* 为何不建议频繁使用`ngx.var `?
因为每次调用都会有一点额外的开销：内部分配少量内存。因此不建议过度使用，应尽量使用等价的功能接口。
如必须使用，则使用 local 化暂存，避免多次调用。
部分功能可以使用 `ngx.req`代替。

* 为何不建议频繁使用 `ngx.ctx`?
ngx.ctx 的成本较高，应当尽量少用，只存放少量必要的数据，避免滥用。

* 如果在 access、rewrite 阶段 ngx.req.set_method，那么 content 阶段的 proxy_pass 的 method 会改变吗？
    * set_rui 等其他 set 函数呢？是否会改变 proxy_pass 到上游的内容？

* 什么时候需要手动 ngx.send_headers？

通常不需要手动执行。

ngx.send_headers 的执行阶段是 “rewrite_by_lua*, access_by_lua*, content_by_lua*”。

* 为什么要限制挂起计时器及运行计时器的数量？
为了避免在服务器中累积并耗尽系统资源，导致Nginx服务器崩溃等极端后果，Nginx工作进程中的“挂起计时器”数量和“运行计时器”数量都有内置的限制。

**为何成本高？**
