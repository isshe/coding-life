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
    - err：失败时的错误信息

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

接下来，我们也已 tcp 为例，跟踪了解 cosocket 的实现方式。

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

```lua
- ngx_http_lua_socket_tcp_connect
    \-
```

从 Lua 接口 [connect](#tcpsock:connect) 可以看到，该接口有 3 个参数，分别是 host、port、options_table。


### 发送请求

### 接收响应

### 处理响应
