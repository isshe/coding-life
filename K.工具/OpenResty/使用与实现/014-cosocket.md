# cosocket

使用 openresty 过程中，常常听说 cosocket，那么，cosocket 是什么呢？

目的：

- 学习如何使用 cosocket。
- 了解 cosocket 的实现方式。

## 使用

ngx.socket.udp 和 ngx.socket.tcp 都是使用 cosocket 的方式实现的，由于 TCP 更常用，因此下面以 TCP 相关接口为例。

### ngx.socket.tcp

- 语法：`tcpsock = ngx.socket.tcp()`

- 上下文：rewrite_by_lua*, access_by_lua*, content_by_lua*, ngx.timer.*, ssl_certificate_by_lua*, ssl_session_fetch_by_lua*, ssl_client_hello_by_lua*

- 作用：创建并返回一个 TCP 或 面向流的 UNIX 套接字对象。

- 注意：
    - 此 API 创建的 cosocket 对象与创建它的 Lua 处理程序具有完全相同的生命周期。所以**永远不要**将 cosocket 对象传递给任何其他 Lua 处理程序（包括 ngx.timer 回调函数），也**永远不要**在不同的 Nginx 请求之间共享 cosocket 对象。
    - 如果没有显式关闭 cosocket 对象的底层连接或把连接放回到连接池，那么连接将在以下情况下自动关闭：
        - 当前请求处理程序（handler）执行完成
        - Lua cosocket 对象被 Lua GC 回收
    - 发生致命错误时，也会自动关闭连接，读超时是唯一的不致命错误。

### tcpsock:bind

- 语法：`ok, err = tcpsock:bind(address)`

- 上下文：rewrite_by_lua*, access_by_lua*, content_by_lua*, ngx.timer.*, ssl_certificate_by_lua*,ssl_session_fetch_by_lua*,ssl_client_hello_by_lua*

- 作用：像标准的 proxy_bind 指令一样，此 api 使到上游服务器的传出连接源自指定的本地 IP 地址。

- 注意：
    - address 参数只能指定 IP 地址

### tcpsock:connect

- 语法：`ok, err = tcpsock:connect(host, port, options_table?)`

- 参数：
    - options_tables：TODO

- 返回值：
    - ok：成功时返回 1，失败返回 nil。
    - err：失败时的错误信息。

- 上下文：rewrite_by_lua*, access_by_lua*, content_by_lua*, ngx.timer.*, ssl_certificate_by_lua*, ssl_session_fetch_by_lua*, ssl_client_hello_by_lua*

- 作用：尝试在不阻塞的情况下将 TCP 套接字对象连接到远程服务器或 UNIX 域套接字文件。

- 注意：
    - 在进行域名解析并连接到远端之前，这个接口始终在连接池中查找之前由此接口或 ngx.socket.connect 接口创建的空闲连接。
    - host 支持 IP 地址和域名，对于域名，将使用 Nginx 核心的动态解析器​​来无阻塞地解析域名。（Nginx 的解析器通过 resolver 指令来配置）。
    - 如果域名解析返回多个 IP，将随机选择一个 IP 进行连接。
    - 对已经连接 cosocket 对象再次调用此接口，将会首先关闭原始连接。

> TODO：补充前面的参数部分。
> 对应：An optional Lua table can be specified as the last argument to this method to specify various connect options:

### 使用示例

```lua
local sock = ngx.socket.tcp()
-- assume "192.168.1.10" is the local ip address
local ok, err = sock:bind("192.168.1.10")
if not ok then
    ngx.say("failed to bind")
    return
end

local ok, err = sock:connect("192.168.1.67", 80)
if not ok then
    ngx.say("failed to connect server: ", err)
    return
end
ngx.say("successfully connected!")

-- send

-- receive

sock:close()
```

