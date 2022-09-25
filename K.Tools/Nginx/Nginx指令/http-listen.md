listen
---

```
Syntax:
    listen address[:port] [default_server] [ssl] [http2 | spdy] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];

    listen port [default_server] [ssl] [http2 | spdy] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];

    listen unix:path [default_server] [ssl] [http2 | spdy] [proxy_protocol] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
Default:
    listen *:80 | *:8000;
Context:
    server
```

- default_server/default：指定 server 作为 `address:port` 默认 server，没有指定则是第一个 server。
- ssl：SSL 模式。
- http2/spdy：HTTP2/SPDY 模式，通常 ssl 也要同时指定。
- proxy_protocol：启用代理协议，主要用于获取真实的客户端 IP。
- setfib=number：设置关联的路由表，当前仅在 FREEBSD 可用。
- fastopen=number：启用快速打开并限制未完成的三次握手的连接队列的最大长度。不要打开除非服务器可以处理多次接收带有数据的同一个 SYN 数据包。
- backlog=number：待处理连接队列的最大长度
- rcvbuf=size：设置接收缓冲区的大小（SO_RCVBUF）
- sndbuf=size：设置发送缓冲区的大小（SO_SNDBUF）
- accept_filter=filter
- deferred：使用延迟的 accept()（TCP_DEFER_ACCEPT 套接字选项）。数据到来时再 accept，而不是接收到 ACK 后就 accept，能避免过早进行相关初始化，并且数据包从 3+1 变成 2+1，减少延迟。
  - 3+1：3 次握手 + 1 次数据包。
  - 2+1：2 次握手 + 1 次数据包。
- bind：对给定的 address:port 进行单独的 bind() 调用。常用于将相同端口 bind 到不同地址。
- ipv6only=on|off：只接受 IPv6 的连接，还是同时接受 IPv6 和 IPv4 的连接。
- reuseport：为每个工作进程创建一个单独的监听套接字（SO_REUSEPORT/SO_REUSEPORT_LB），让内核自动做负载均衡，将请求平均地让多个进程进行处理。主要用于提高性能。

参考

- http://nginx.org/en/docs/http/ngx_http_core_module.html#listen
- https://www.cnblogs.com/schips/p/12553321.html#:~:text=SO_REUSEPORT%20%E6%A6%82%E8%BF%B0,%E5%A4%9A%E4%B8%AA%E7%BA%BF%E7%A8%8B%E8%BF%9B%E8%A1%8C%E5%A4%84%E7%90%86%E3%80%82
