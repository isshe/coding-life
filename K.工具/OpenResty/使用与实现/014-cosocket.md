# cosocket

使用 openresty 过程中，常常听说 cosocket，那么，cosocket 是什么呢？

目的：

- 学习如何使用 cosocket。
- cosocket 是什么？
- cosocket 的实现方式、原理是什么？

## 使用

ngx.socket.udp 和 ngx.socket.tcp 都是使用 cosocket 的方式实现的，由于 TCP 更常用，因此下面以 TCP 相关接口为例。

以下接口的上下文皆是：`rewrite_by_lua*`, `access_by_lua*`, `content_by_lua*`, `ngx.timer.*`, `ssl_certificate_by_lua*`, `ssl_session_fetch_by_lua*`, `ssl_client_hello_by_lua*`。

### ngx.socket.tcp

- 语法：`tcpsock = ngx.socket.tcp()`

- 作用：创建并返回一个 TCP 或 面向流的 UNIX 套接字对象。

- 注意：
    - 此 API 创建的 cosocket 对象与创建它的 Lua 处理程序具有完全相同的生命周期。所以**永远不要**将 cosocket 对象传递给任何其他 Lua 处理程序（包括 ngx.timer 回调函数），也**永远不要**在不同的 Nginx 请求之间共享 cosocket 对象。
    - 如果没有显式关闭 cosocket 对象的底层连接或把连接放回到连接池，那么连接将在以下情况下自动关闭：
        - 当前请求处理程序（handler）执行完成
        - Lua cosocket 对象被 Lua GC 回收
    - 发生致命错误时，也会自动关闭连接，读超时是唯一的不致命错误。

### tcpsock:bind

> 此接口需要更新的版本，v0.10.21 版本中还未实现。

- 语法：`ok, err = tcpsock:bind(address)`

- 作用：像标准的 proxy_bind 指令一样，此 api 使到上游服务器的传出连接源自指定的本地 IP 地址。

- 注意：
    - address 参数只能指定 IP 地址

### tcpsock:connect

- 语法：`ok, err = tcpsock:connect(host, port, options_table?)`

- 参数：
    - host：地址或者是 UNIX 域套接字文件。
    - port：端口
    - options_tables：
        - pool：内存池名称，如果不指定，则是 `<host>:<port>` 或 `<unix-socket-path>`。
        - pool_size：
            - 粒度是 worker 进程级别，不是程序级别，并且连接池创建后，大小就无法修改了。
            - 如果没有指定此参数并且 `backlog` 参数也没有指定，则不会创建连接池。
            - 如果没有指定此参数但指定 `backlog`，则连接池大小为 backlog。
            - 如果连接池内连接数量超了，则会根据最近最少使用的方式关闭连接。
        - backlog：限制指定连接池打开的连接总数。
            - 任何时候，打开的连接都不能超过 pool_size 指定的数量。
            - 如果连接池满了，就会积压到 backlog 队列中，如果队列也满了，就会返回 `nil` 并报错。
            - 如果等待时间超过 connect_timeout（可通过 settimeouts 指定），则会返回 `nil` 并报错。

- 返回值：
    - ok：成功时返回 1，失败返回 nil。
    - err：失败时的错误信息。

- 作用：尝试在不阻塞的情况下将 TCP 套接字对象连接到远程服务器或 UNIX 域套接字文件。

- 注意：
    - 在进行域名解析并连接到远端之前，这个接口始终在连接池中查找之前由此接口或 ngx.socket.connect 接口创建的空闲连接。
    - host 支持 IP 地址和域名，对于域名，将使用 Nginx 核心的动态解析器​​来无阻塞地解析域名。（Nginx 的解析器通过 resolver 指令来配置）。
    - 如果域名解析返回多个 IP，将随机选择一个 IP 进行连接。
    - 对已经连接 cosocket 对象再次调用此接口，将会首先关闭原始连接。

### tcpsock:send

- 语法：`bytes, err = tcpsock:send(data)`

- 参数：
    - data：需要发送的数据。字符串或者是一个字符串数组（table）。

- 返回值：
    - bytes：发送了的字节数，失败返回 nil。
    - err：失败时的错误信息。

- 作用：在当前连接上发送数据。

- 注意：这是一个同步操作。直到所有数据发送到系统 socket 发送缓冲区或者出错才返回。

### tcpsock:receive

- 语法：
    - `data, err, partial = tcpsock:receive(size)`
    - `data, err, partial = tcpsock:receive(pattern?)`

- 参数：
    - size：接收指定字节的数据，支持数字或者字符串。
    - pattern：支持 `*l` 和 `*a`，默认是行模式 `*l`。
        - `*l`：从套接字中读取**一行**文本。该行以换行 (LF) 结束，前面可以选择回车 (CR)。返回行中不包含 CR 和 LF 字符。事实上，所有 CR 字符都被该模式忽略。
        - `*a`：从套接字读取直到连接关闭。

- 返回值：
    - data：接收到的数据。出错时是 nil。
    - err：出错时的错误信息。
    - partial：出错时返回当前以接收到的数据。

- 作用：根据读取模式或大小从 socket 连接上接收数据。

- 注意：
    - 可通过 `settimeout` 指定读取超时时间。（发送时，也可用此函数指定发送超时时间）。

### tcpsock:settimeout

- 语法：`tcpsock:settimeout(time)`

- 参数：
    - time：超时时间，单位是毫秒。

- 作用：为接下来的操作设置超时时间。
    - 读取时，设置的是读取超时时间。
    - 发送时，设置的是发送超时时间。
    - 连接时，设置的是连接超时时间。

- 注意：设置 keepalive 超时需使用 `setkeepalive` 方法。

### tcpsock:close

- 语法：`ok, err = tcpsock:close()`

- 返回值：
    - ok：成功时返回 `1`，失败时返回 `nil`。
    - err：失败时的错误信息。

- 作用：关闭当前 socket 套接字。

- 注意：
    - 调用过 setkeepalive 的 socket 对象，不用再调用 close，因为 socket 对象已经关闭并且连接已经被保存到内置连接池中。
    - 没有调用 close 方法的 socket 对象（以及关联的连接）将在 Lua GC 释放 socket 对象或当前客户端 HTTP 请求处理完毕时关闭。

### 使用示例

```lua
local sock = ngx.socket.tcp()
-- bind 接口在更新的版本才有
-- local ok, err = sock:bind("172.22.49.3")
-- if not ok then
--     ngx.say("failed to bind")
--     return
-- end

local ok, err = sock:connect("172.64.138.14", 80)
if not ok then
    ngx.say("failed to connect server: ", err)
    return
end
ngx.say("successfully connected!")

local req = "GET / HTTP/1.1\r\n"
            .. "Host: ifconfig.io\r\n"
            .. "User-Agent: curl/7.68.0\r\n"
            .. "Accept: */*\r\n\r\n"

-- send
sock:settimeout(3000)
local bytes, err = sock:send(req)
if not bytes then
    ngx.say("failed to send data to server: ", err)
    return
end

ngx.say("successfully sent, bytes: ", bytes)

-- receive
sock:settimeout(3000)
local data, err, partial = sock:receive()
if not data then
    ngx.say("failed to read data from server: ", err)
    return
end

ngx.say("successfully read: ", data)

sock:close()
```

## 实现

### Lua 接口注入

毫无疑问，还是类似的注入方式

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
                                        \- ngx_http_lua_inject_socket_tcp_api
```

- ngx_http_lua_inject_socket_tcp_api 的执行流程

```lua
- ngx_http_lua_inject_socket_tcp_api
    \- lua_createtable: 创建 ngx.socket 表
    \- lua_setfield(L, -3, "tcp")：注入 ngx.socket.tcp，以及 ngx.socket.stream, ngx.socket.connect 等 ngx.socket 系列接口
    \- lua_pushlightuserdata(L, ngx_http_lua_lightudata_mask(req_socket_metatable_key))
    \- lua_createtable(L, 0 /* narr */, 6 /* nrec */)：创建元表并注入通过 ngx.req.socket() 得到的对象的相关接口
    \- lua_pushlightuserdata(L, ngx_http_lua_lightudata_mask(raw_req_socket_metatable_key))
    \- lua_createtable(L, 0 /* narr */, 7 /* nrec */)：创建元表并注入通过 ngx.req.socket(raw) 得到的对象的相关接口
    \- lua_pushlightuserdata(L, ngx_http_lua_lightudata_mask(tcp_socket_metatable_key))
    \- lua_createtable(L, 0 /* narr */, 14 /* nrec */)：创建元表并注入 tcp 对象（通过 ngx.socket.tcp 创建）的接口
    \- upstream_udata_metatable_key：还有以下这些元表，不再赘述，如有必要，直接通过这些 key 在源码中搜索，即可知道如何使用。
    \- downstream_udata_metatable_key：例如这个在 ngx.req.socket 调用时使用
    \- pool_udata_metatable_key
    \- pattern_udata_metatable_key
    \- ssl_session_metatable_key
```

以 tcp 为例，
调用 ngx.socket.tcp() 函数创建 tcp 对象时，实际调用了 ngx_http_lua_socket_tcp 函数，
此函数中，会新建一个表，然后设置 ngx_http_lua_inject_socket_tcp_api 中创建的表（以 tcp_socket_metatable_key 标识）为元表。

接下来，我们也以 tcp 为例，跟踪了解 cosocket 的实现方式。

### 创建对象

创建对象的函数是 ngx_http_lua_socket_tcp，比较简单，主要完成以下操作：

- 获取请求
- 获取模块 ctx
- 检查 ctx 是否是可以 YIELD 的
- 创建新表
- 设置新表的元表为前面张杰中注入的元表， key：tcp_socket_metatable_key。
- 返回新表

### 创建连接

创建 TCP 连接的函数是 ngx_http_lua_socket_tcp_connect，我们来跟踪一下。

注释版代码见 [src/ngx_http_lua_socket_tcp.c](https://github.com/isshe/lua-nginx-module/blob/isshe/src/ngx_http_lua_socket_tcp.c#L964)

```lua
- ngx_http_lua_socket_tcp_connect
    \- if (lua_type(L, n) == LUA_TTABLE)：检查最后一个参数是不是 table 类型，是的话就是有选项参数（sock:connect 的最后一个参数）。
        \- lua_getfield(L, n, "pool_size")：从选项参数表中取出 pool_size，并检查合法性，需要 大于 0 或者等于 nil。
        \- lua_getfield(L, n, "backlog")：从选项参数表中取出 backlog，并检查合法性，需大于等于 0。如果只设置了 backlog，没有设置 pool_size，则把默认 pool_size 值设置成 pool_size。
        \- lua_getfield(L, n, "pool")：从选项参数表中取值 pool，如果值是数字，就转成字符串，并继续按字符串处理；如果是字符串，就设置到 tcp 对象的指定的 key 下面（SOCKET_KEY_INDEX）；如果是 nil，就从堆栈弹出 pool 相关参数；其他值报错返回。
    \- if (n == 3 && lua_isnumber(L, 3))：检查 port，不合法就退出，报错退出。
    \- lua_rawgeti(L, 1, SOCKET_CTX_INDEX)：取出 upstream_t， 如果没有就分配一个并设置到 tcp 对象的 SOCKET_CTX_INDEX；如果有了，就检查这个上游对象的有效性，有效就复用。
    \- ngx_memzero(u, sizeof(ngx_http_lua_socket_tcp_upstream_t))：把拿到的上游对象初始化一下：清空，设置相关字段。
    \- lua_rawgeti(L, 1, SOCKET_CONNECT_TIMEOUT_INDEX)：把超时参数从 TCP 对象中取出压入栈中，设置超时到上游对象的连接超时、发送超时、读取超时字段中。（TCP 对象中的超时参数是 settimeouts、settimeout 函数设置的）
    \- lua_pushlightuserdata(L, ngx_http_lua_lightudata_mask(socket_pool_key))：根据 pool 参数或者 host:port 为 key 取出对应连接池，如果有就直接用；如果没有，就创建（ngx_http_lua_socket_tcp_create_socket_pool）这个连接池。
    \- ngx_http_lua_socket_tcp_connect_helper：进入包裹函数，继续处理
        \- if (spool != NULL)：检查是否指定了连接池；指定了，就从连接池中取出 keepalive 连接，取到了就直接返回。这里会限制连接池中总连接数不能超过 连接池大小 size + backlog。如果超过了 size，但小于 size + backlog，意味着是 backlog 连接，从 backlog 队列的从中取出或创建连接操作上下文（ngx_http_lua_socket_tcp_conn_op_ctx_t）放到队列中。
    \- host.data = ngx_palloc(r->pool, host_len + 1)：分配内存存 host，用于通过 ngx_parse_url 创建 sockaddr。
    \- u->resolved = ngx_pcalloc(r->pool, sizeof(ngx_http_upstream_resolved_t))：分配内存存放域名解析相关内容，如果需要解析，就放到 u->resolved->host 字段；不用就把前面创建的 sockaddr 放到 u->resolved->sockaddr 中。
    \- if (u->resolved->sockaddr)：有地址了，直接进行连接，然后返回。
        \- ngx_http_lua_socket_resolve_retval_handler
    \- rctx = ngx_resolve_start(clcf->resolver, &temp)：进行域名解析。
    \- if (ngx_resolve_name(rctx) != NGX_OK)：正式开始。
    \- u->write_prepare_retvals = ngx_http_lua_socket_resolve_retval_handler：设置解析完成后的回调函数。

- ngx_http_lua_socket_resolve_retval_handler：解析结果返回了，开始正式发起连接。
    \- if (u->resolved->sockaddr)：如果有就是解析成功了，没有就是解析失败了，报错返回。
    \- rc = ngx_event_connect_peer(pc)：发起连接
    \- if (rc == NGX_ERROR)：连接出错了
        \- ngx_http_lua_socket_conn_error_retval_handler：错误处理函数
    \- if (rc == NGX_BUSY)：没有存活连接
    \- if (rc == NGX_DECLINED)：socket 错误。
        \- ngx_http_lua_socket_conn_error_retval_handler：错误处理函数
    \- /* rc == NGX_OK || rc == NGX_AGAIN */；接下来就是连接中或者连接成功的情况了。
        \- NGX_AGAIN：connect 返回 -1，错误码是 NGX_EINPROGRESS，意思是正在连接中。
    \- c->write->handler = ngx_http_lua_socket_tcp_handler：添加读写事件的处理函数
    \- if (rc == NGX_OK)：如果是连接成功了，就去掉读写事件，免得浪费 CPU 周期。
    \- /* rc == NGX_AGAIN */：还没连接成功
    \- ngx_add_timer(c->write, u->connect_timeout)：增加增加连接超时 timer。
```

从 Lua 接口 [connect](#tcpsock:connect) 可以看到，该接口有 3 个参数，分别是 host、port、options_table。
（其实是 4 个参数，第一个是 tcpsock 自身：connect(tcpsock, host, port, options_table)）

连接步骤大致总结如下：

- 检查选项参数（options_table）
- 如果没有连接池就创建连接池
- 从连接池中获取连接，没取到就直接进行连接；（后续可通过 setkeepalive 放回到连接池中）
- 通过 ngx_parse_url 解析 host，看是不是域名，不是域名会直接创建 sockaddr
- 如果是域名，就进行域名解析
- 解析完成调用 ngx_event_connect_peer 进行连接
- 如果返回 NGX_OK，就是连接成功了
- 如果返回 NGX_AGAIN，c 函数 connect 的错误码是 EINPROGRESS，表示正在连接中，设置读写事件，等待写事件即可。（连接成功后，套接字会变为可写状态，事件模块会调用写事件处理函数）

### 发送请求

接口注入：

```
- ngx_http_lua_inject_socket_tcp_api
    \- ngx_http_lua_socket_tcp_send
```

拿到注入的接口名称后，我们来跟踪一下：

```lua
- ngx_http_lua_socket_tcp_send
    \- if (u == NULL || u->peer.connection == NULL || u->write_closed)：先检查连接还在不在，不在就报错返回。
    \- if (u->body_downstream)：检查是不是想要写请求对应的 socket（ngx.req.socket），是的话，报错返回，不允许写。
    \- type = lua_type(L, 2)：获取并检查第二个参数（也就是发送的数据）的类型；计算出最大发送长度。
    \- ngx_http_lua_chain_get_free_buf：看下还有没有空间能放得下这么长的数据；没有会返回 NULL。
    \- switch (type)：把要发送的数据写入获取到的 buffer 中。
    \- if (clcf->tcp_nodelay && c->tcp_nodelay == NGX_TCP_NODELAY_UNSET)：看有没有选项需要设置。
    \- ngx_http_lua_socket_send：进行发送。
        \- b = u->request_bufs->buf：获取要发送的 buffer。
        \- n = c->send(c, b->pos, b->last - b->pos)：进行发送，如果发送失败或者阻塞了，就立即退出死循环；否则就一直发送到发送完为止。
        \- if (n == NGX_ERROR)：如果是出错，就进行错误处理，然后返回 NGX_ERROR
        \- else：否则就是 n == NGX_AGAIN，还在发送中
            \- u->write_event_handler = ngx_http_lua_socket_send_handler：设置写事件处理函数。
            \- ngx_add_timer(c->write, u->send_timeout)：增加定时器。
            \- ngx_handle_write_event(c->write, u->conf->send_lowat)：监听写事件。
    \- if (rc == NGX_ERROR)：出错了就进行错误处理并返回
        \- ngx_http_lua_socket_write_error_retval_handler
    \- if (rc == NGX_OK)：成功了直接返回
    \- /* rc == NGX_AGAIN */：还在发送中
    \- u->write_prepare_retvals = ngx_http_lua_socket_tcp_send_retval_handler：设置好回调函数，yield 出去等待可写事件发生。
```

经过前面 connect 的整理，send 就简单了很多，都是一样的套路：返回 NGX_AGAIN 就 yield，继续加入到事件循环中，等待下次事件触发，继续处理，都完成以后，在 resume 回去 lua 代码。

步骤大致总结如下：

- 计算发送的长度；
- 复制数据到指定的 buffer 中；
- 进行发送，如果一直发送成功，就一直发送；否则就退出循环。
- 如果是出错了，就进行错误处理，然后退出。
- 如果是阻塞了，就设置写处理函数并监听写事件，yield 出去等待下次可写。

### 接收响应

接口注入：

```
- ngx_http_lua_inject_socket_tcp_api
    \- ngx_http_lua_socket_tcp_receive
```

原理和前面发送类似，大致步骤如下：

- 处理参数，判断是数字还是字符串；数字表示要接收的字节数量，是字符串表示模式，是 `*a` 还是 `*l`。根据不同的模式设置不同的 input_filter。如果没有指定第二个参数，就是默认的行模式。
- 检查缓冲区有没有空间，没有就重新获取一个
- 开始读取，出错或者成功，就直接返回。
- 如果是 NGX_AGAIN，就设置好处理函数，yiele 了继续等待事件发生。

### 关闭连接

如果请求没有忙于读、写、连接，则调用 ngx_http_lua_socket_tcp_finalize 关闭连接。

## 总结

### 1.Nginx 是如何进行域名解析的？ ngx_resolve_start、ngx_resolve_name，是异步的吗，会等待解析结果吗？

答：详见 [Nginx 是如何进行 DNS 解析的？](../../Nginx/Nginx源码分析/7-nginx-dns-resolve.md)。

### 2.如何进行连接的？

答：进行非阻塞连接，返回 rc == -1 && err == NGX_EINPROGRESS 时，表示连接正在进行中。openresty 设置好读写事件，在 ngx_http_lua_socket_connected_handler 中检查连接实际是否成功。

更多参考 [I/O多路复用与非阻塞连接](../../../B.操作系统/Linux/Application/7.IO多路复用/IO多路复用与非阻塞连接/README.md)

### 3.TCP 连接进行中（NGX_AGAIN）时，yield 出去后，会在哪里恢复？

答：

- https://github.com/isshe/lua-nginx-module/blob/isshe/src/ngx_http_lua_socket_tcp.c#L840
- 此时 Lua 正在调用 connect。
- yield 以后，继续返回到 ngx_http_lua_run_thread，最后回到事件循环中，等待事件发生，重新 resume。


```c
// yield 后也就是 lua_resume 返回了
rv = lua_resume(orig_coctx->co, nrets);
```

- 事件发生（c->write 写事件）后，调用 ngx_http_lua_socket_tcp_handler。

连接成功后的调用栈：

```lua
#0  ngx_http_lua_run_thread (L=0x55efffa60640 <cached_syslog_time+800>, r=0x55efff9d0280,
    ctx=0x55efff7be452 <ngx_sprintf+192>, nrets=32766)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_util.c:1112
#1  0x000055efff945315 in ngx_http_lua_socket_tcp_resume_helper (r=0x55f001716120,
    socket_op=0) at ../ngx_lua-0.10.21/src/ngx_http_lua_socket_tcp.c:5991
#2  0x000055efff945082 in ngx_http_lua_socket_tcp_conn_resume (r=0x55f001716120)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_socket_tcp.c:5897
#3  0x000055efff92e8bf in ngx_http_lua_content_wev_handler (r=0x55f001716120)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_contentby.c:152
#4  0x000055efff93f72a in ngx_http_lua_socket_handle_conn_success (r=0x55f001716120,
    u=0x7f868af3adc8) at ../ngx_lua-0.10.21/src/ngx_http_lua_socket_tcp.c:3451
#5  0x000055efff9401b6 in ngx_http_lua_socket_connected_handler (r=0x55f001716120,
    u=0x7f868af3adc8) at ../ngx_lua-0.10.21/src/ngx_http_lua_socket_tcp.c:3719
#6  0x000055efff93efd0 in ngx_http_lua_socket_tcp_handler (ev=0x55f0017a73b0)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_socket_tcp.c:3239
...
```

### 4.什么是 cosocket 呢？ cosocket 是如何实现的呢？

答：cosocket 是一种基于协程的高性能、低延迟的 I/O 模型。cosocket 基于事件驱动和异步 I/O，使用 Nginx 事件驱动框架监听套接字事件，通过异步 I/O 读写套接字数据，实现面向协议编程（解析/处理协议）的目的。

- 协程：利用 yield、resume 对协程进行调度。
- 事件驱动：发生事件是，调用事先设置好的处理程序。
- 异步 IO：不进行阻塞调用，条件没就绪也立即返回。根据不同的返回值，进行处理。如果返回值是 NGX_AGAIN 意味满足，需要继续监听事件，并且 yield 当前的 Lua 调用，等待条件满足再返回。
