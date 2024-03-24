DNS
---

DNS - 域名系统（Domain Name System）。
* 是一个将`域名`和`IP地址`进行映射的`分布式数据库`。
* DNS 是应用层协议，使用 TCP 和 UDP 的 53 端口。
* 每一级域名长度的限制是 63 个字符，域名总长度不能超过 253 个字符。

# 作用
* 将对人友好的域名转换为对计算机友好的 IP 地址。

# 原理/过程
域名层级
```
主机名.次级域名.顶级域名.根域名
host.sld.tld.root
www.isshe.xyz.root
abc.isshe.xyz.root
```
分级查询：从根域名开始，依次查询每一级域名的 NS 记录。
* 从`根域名服务器`查询`顶级域名服务器`的 NS 记录和 A 记录；
    * `根域名服务器`是总所周知的，不用查。
* 从`顶级域名服务器`查询`次级域名服务器`的 NS 记录和 A 记录；
* 从`次级域名服务器`查询`主机名`的 A 记录；

分级查询示例：`www.isshe.xyz`
> dig +trace www.isshe.xyz 进行查看

* 列出根域名服务器：
```
.                       227343  IN      NS      e.root-servers.net.
.                       227343  IN      NS      k.root-servers.net.
.                       227343  IN      NS      i.root-servers.net.
.                       227343  IN      NS      d.root-servers.net.
.                       227343  IN      NS      j.root-servers.net.
.                       227343  IN      NS      m.root-servers.net.
.                       227343  IN      NS      b.root-servers.net.
.                       227343  IN      NS      c.root-servers.net.
.                       227343  IN      NS      h.root-servers.net.
.                       227343  IN      NS      f.root-servers.net.
.                       227343  IN      NS      l.root-servers.net.
.                       227343  IN      NS      a.root-servers.net.
.                       227343  IN      NS      g.root-servers.net.
```
* 查询`xyz.`的结果：
```
xyz.                    172800  IN      NS      x.nic.xyz.
xyz.                    172800  IN      NS      y.nic.xyz.
xyz.                    172800  IN      NS      z.nic.xyz.
xyz.                    172800  IN      NS      generationxyz.nic.xyz.
...
;; Received 669 bytes from 192.58.128.30#53(j.root-servers.net) in 651 ms
```
* 查询`isshe.xyz.`的结果：
```
isshe.xyz.              3600    IN      NS      f1g1ns1.dnspod.net.
isshe.xyz.              3600    IN      NS      f1g1ns2.dnspod.net.
...
;; Received 581 bytes from 194.169.218.42#53(x.nic.xyz) in 245 ms
```
* 查询`www.isshe.xyz.`的结果：
```
www.isshe.xyz.          600     IN      CNAME   isshe.coding.me.
isshe.xyz.              86400   IN      NS      f1g1ns1.dnspod.net.
isshe.xyz.              86400   IN      NS      f1g1ns2.dnspod.net.
;; Received 135 bytes from 14.215.155.156#53(f1g1ns1.dnspod.net) in 10 ms
```

# 协议格式
```
0                   16                  32 0                   16                 32
+-------------------+-------------------+  +---------------------------------------+
| Transaction ID    | Flags             |  / Name                                  /
+-------------------+-------------------+  /                                       /
| Questions         | Answer RRs        |  +-------------------+-------------------+
+-------------------+-------------------+  | Type              | Class             |
| Authority RRs     | Additional RRs    |  +-------------------+-------------------+
+-------------------+-------------------+ /
/ Question                              /
+---------------------------------------+
/ Answers                               /
+---------------------------------------+
/ Authoritaty                           /
+---------------------------------------+
/ Additional                            /
+---------------------------------------+
```
## Transaction ID
* 会话 ID，DNS 报文的 ID 标识。
* 请求和应答报文的这个字段相同。

## Flags
* 标志：
```
+----+--------+----+----+----+----+--------+-------+
| QR | opcode | AA | TC | RD | RA | (zero) | rcode |
+----+--------+----+----+----+----+--------+-------+
  1      4      1    1    1    1      3        4
```
* QR: 查询/响应标记，0 查询，1 响应。
* opcode：0 标准查询，1 反向查询，2 服务器状态请求。
* AA：授权回答。
* TC：可截断的。
* RD：期望递归。
* RA：可用递归。
* rcode：表示返回码。0 没有差错，2 服务器错误，3 名字差错

## Question
```
0                   16                 32
+---------------------------------------+
/ Name                                  /
/                                       /
+-------------------+-------------------+
| Type              | Class             |
+-------------------+-------------------+
```
* Name：查询的域名名称。按 Lable 划分 (isshe.xyz，为 2 个 lable)，以`'\0'`结尾；可能为奇数个字节。
* Type：查询类型。A：主机。
* Class：查询的协议类。IN：internet。

## Answer/Authority/Additional
```
0                   16                 32
+---------------------------------------+
/ Name                                  /
/                                       /
+-------------------+-------------------+
| Type              | Class             |
+-------------------+-------------------+
| TTL                                   |
+---------------------------------------+
| RdLength          | Rdata             /
+-------------------+                   /
|                                       /
+---------------------------------------+
```
* Name: 域名。
* Type：类型。
* Class：Rdata 的类。
* TTL：资源记录的生存时间。0 表示只能被传输，不能被缓存。（无符号整型）
* RdLenght：Rdata 的长度。
* Rdata：资源数据，表示记录。格式和 Type、Class 有关。如 Type=A、Class=IN，Rdata 就是一个 IP 地址。

## RRs
Questions：指明 Question 的数量。
Answer RRs：指明 Answer 的数量。
Authority RRs：指明 Antuority 的数量。
Additional RRs：指明 Additional 的数量。

# 记录类型
> 详见[DNS 记录类型列表](https://zh.wikipedia.org/wiki/DNS%E8%AE%B0%E5%BD%95%E7%B1%BB%E5%9E%8B%E5%88%97%E8%A1%A8)

常见记录类型：
* A：主机记录（Address）。
* NS：域名服务器记录（Name Server）。
    * 返回保存下一级域名信息的服务器地址。该记录只能设置为域名，不能设置为 IP 地址。
* CNAME：规范名称记录，当前查询域名是另一个域名的跳转。（即别名记录，指向某个 A 记录）
* AAAA：IPv6 主机记录。
* SRV：服务位置记录。
* MX: 邮件记录 (Mail eXchange)，返回接收电子邮件的服务器地址。
* PTR：逆向查询记录（Pointer Record），从 IP 查域名。

# 报文示例
## 报文示例 - 查询
```
               +---------------------------------------------------+
    Header     | OPCODE=SQUERY                                     |
               +---------------------------------------------------+
    Question   | QNAME=SRI-NIC.ARPA., QCLASS=IN, QTYPE=A           |
               +---------------------------------------------------+
    Answer     | <empty>                                           |
               +---------------------------------------------------+
    Authority  | <empty>                                           |
               +---------------------------------------------------+
    Additional | <empty>                                           |
               +---------------------------------------------------+
```

## 报文示例 - 响应
```
               +---------------------------------------------------+
    Header     | OPCODE=SQUERY, RESPONSE, AA                       |
               +---------------------------------------------------+
    Question   | QNAME=SRI-NIC.ARPA., QCLASS=IN, QTYPE=A           |
               +---------------------------------------------------+
    Answer     | SRI-NIC.ARPA. 86400 IN A 26.0.0.73                |
               |               86400 IN A 10.0.0.51                |
               +---------------------------------------------------+
    Authority  | <empty>                                           |
               +---------------------------------------------------+
    Additional | <empty>                                           |
               +---------------------------------------------------+
```

# 疑问

# 相关
* dig 命令：可用于 DNS 相关操作。
* host 命令：dig 的简化版本。
* whois 命令：查看域名注册情况。
* nslookup 命令：可互动式查询域名记录。
* [DNS 原理入门 - 阮一峰](https://www.ruanyifeng.com/blog/2016/06/dns.html)
* [wiki](https://en.wikipedia.org/wiki/Domain_Name_System#DNS_message_format)
* [wiki 中文](https://zh.wikipedia.org/wiki/DNS记录类型列表)

