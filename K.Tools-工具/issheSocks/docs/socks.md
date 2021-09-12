
[TOC]

socks5
---

# 工作流程
```
              [client]                  [server]
                 |  1.方法(method)选择报文   |
                 | ----------------------> |
                 |                         |
                 |  2.应答                 |
                 | <---------------------- |
                 |                         |
                 |  3.请求报文              |
                 | ----------------------> |
                 |                         |
                 |  4.应答                 |
                 | <-----------------------|
                 |                         |
                 |                         |
```
* `1和2`应该是可以进行多次的，多次协商完成后，再进行`请求`。

# 报文格式
## 方法选择报文及其应答
> 客户端 -> 服务器

```
                   +----+----------+----------+
                   |VER | NMETHODS | METHODS  |
                   +----+----------+----------+
                   | 1  |    1     | 1 to 255 |
                   +----+----------+----------+
```

> 服务器 <- 客户端

```
                         +----+--------+
                         |VER | METHOD |
                         +----+--------+
                         | 1  |   1    |
                         +----+--------+
```

* VER: 版本号，当前通常设置为`0x5`。
* NMETHODS：方法的数量。
* METHODS: 如果为`FF`，就要关闭连接。
    * X'00' NO AUTHENTICATION REQUIRED
    * X'01' GSSAPI
    * X'02' USERNAME/PASSWORD
    * X'03' to X'7F' IANA ASSIGNED
    * X'80' to X'FE' RESERVED FOR PRIVATE METHODS
    * X'FF' NO ACCEPTABLE METHODS
    

# 请求及应答
> 客户端 -> 服务器

```
        +----+-----+-------+------+----------+----------+
        |VER | CMD |  RSV  | ATYP | DST.ADDR | DST.PORT |
        +----+-----+-------+------+----------+----------+
        | 1  |  1  | X'00' |  1   | Variable |    2     |
        +----+-----+-------+------+----------+----------+
```
* VER: 协议版本，填即可`0x05`。
* CMD: 命令
    * `0x01`: CONNECT 
    * `0x02`: BIND 
    * `0x03`: UDP ASSOCIATE 
* RSV: 保留
* ATYP: 地址类型
    * 0x01: IPv4
    * 0x03: 域名
    * 0x04: IPv6
* DST.ADDR: 目的地址
* DST.PORT: 目的端口

> 服务器 -> 客户端

```
        +----+-----+-------+------+----------+----------+
        |VER | REP |  RSV  | ATYP | BND.ADDR | BND.PORT |
        +----+-----+-------+------+----------+----------+
        | 1  |  1  | X'00' |  1   | Variable |    2     |
        +----+-----+-------+------+----------+----------+
```
* VER：协议版本，`0x05`
* REP    Reply field:
    * '0x00' succeeded
    * '0x01' general SOCKS server failure
    * '0x02' connection not allowed by ruleset
    * '0x03' Network unreachable
    * '0x04' Host unreachable
    * '0x05' Connection refused
    * '0x06' TTL expired
    * '0x07' Command not supported
    * '0x08' Address type not supported
    * '0x09' to X'FF' unassigned
* RSV: 保留
* ATYP: 地址类型
    * 0x01: IPv4
    * 0x03: 域名
    * 0x04: IPv6
* BND.ADDR: server bound address
* BND.PORT: server bound port in network octet order



