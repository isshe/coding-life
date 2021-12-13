[TOC]

ICMP
---

* ICMP - 互联网控制消息协议（Internet Control Message Protocol）
* ICMP是互联网协议族的核心协议之一。
* IMCP用于发送控制消息，提供可能发生在通信环境中的各种问题反馈。
* ICMP消息都是直接封装在一个IP数据包中的，因此，和UDP一样，ICMP是不可靠的。

# 原理及技术细节
* 每个路由器在转发数据报的时候都会把IP包头中的TTL值减1。

# 报文结构
ICMP报头从IP报头的第160位(20字节)开始：（有可选部分另算）
```
  8bit   8bit   8bit   8bit
+------+------+------+------+
| Type | Code |  Checksum   |
+------+------+------+------+
|     ID      |  Sequence   |
+------+------+------+------+
```
* Type: ICMP报文类型。
* Code：进一步划分ICMP的类型；该字段用来查找产生错误的原因。
    * 例如，ICMP的目标不可达类型可以把这个位设为1至15等来表示不同的意思。
* Checksum：校验和。
* ID：ID/标识，在Echo Reply类型的消息中需要返回这个字段。
* Sequence：序号，在Echo Reply类型的消息中需要返回这个字段。

# 报文类型
|Type|Code|Status|描述|查询|差错|备注|
|---|---|---|---|---|---|---|
|0：Echo响应|0||Echo响应|✓||ping中使用|
|1||未分配|||✓|保留|
|2||未分配|||✓|保留|
|3：目的不可达|0|| 目标网络不可达 ||✓||
||1|| 目标主机不可达 ||✓||
||2|| 目标协议不可达 ||✓||
||3|| 目标端口不可达 ||✓||
||4|| 要求分段并(但)设置DF标记 ||✓||
||5|| 源路由失败 ||✓||
||6|| 未知的目标网络 ||✓||
||7|| 未知的目标主机 ||✓||
||8|| 源主机隔离（作废不用） ||✓||
||9|| 禁止访问的网络 ||✓||
||10|| 禁止访问的主机 ||✓||
||11|| 对特定的TOS 网络不可达 ||✓||
||12|| 对特定的TOS 主机不可达 ||✓||
||13|| 由于过滤 网络流量被禁止 ||✓||
||14|| 主机越权 ||✓||
||15|| 优先权终止生效 ||✓||
|4：源端关闭|0|弃用| 源端关闭（拥塞控制） ||✓||
|5：重定向|0|| 重定向网络 ||✓||
||1|| 重定向主机 ||✓||
||2|| 基于TOS的网络重定向 ||✓||
||3|| 基于TOS的主机重定向 ||✓||
|6||弃用| 备用主机地址 ||||
|7||未分配| 保留 ||||
|8|0|| Echo请求 |✓|||
|9|0|| 路由通告 |✓|||
|10|0|| 路由器的发现/选择/请求 |✓|||
|11：超时|0|| TTL超时 ||✓||
||1|| 分片重组超时 ||✓||
|12：参数问题-IP头部错误|0|| IP报文首部参数错误 ||✓||
||1|| 丢失必要选项 ||✓||
||2|| 不支持的长度 ||||
|13|0|| 时间戳请求 |✓|||
|14|0|| 时间戳应答 |✓|||
|15|0|| 信息请求 |✓|||
|16|0|| 信息应答 |✓|||
|17|0|| 地址掩码请求 |✓|||
|18|0|| 地址掩码应答 |✓|||
|19||保留| 因安全原因保留 ||||
|20~29||保留| 保留用于稳健性实验 ||||
|30~39||弃用|  ||||
|40||| [Photuris](https://zh.wikipedia.org/w/index.php?title=Photuris_(protocol)&action=edit&redlink=1), Security failures |||？？？|
|41||| 用于实验性移动协议，如Seamoby[RFC4065] ||||
|42~255||保留|  ||||
|235||实验性| RFC3692 ||||
|254||实验性| RFC3692 ||||
||||  ||||



# 部分报文结构示例

## 3：目标不可达
目的地不可达由主机或其入站网关生成，以通知客户端由于某种原因目的地不可达。
```
0         8          16         24         32
+---------+----------+----------+----------+
| Type=3  | Code=... |      Checksum       |
+---------+----------+----------+----------+
|       未使用        |      下一跳MTU       |
+------------------------------------------+
|     IP报头和原始数据报数据的前8个字节         |
+------------------------------------------+
```

## 11: 超时
超时由网关生成，以便通知`源`，数据报在TTL=0时被丢弃了。
```
0         8          16         32
+---------+----------+----------+
| Type=11 | Code=0/1 | Checksum |
+---------+----------+----------+
|            未使用              |
+-------------------------------+
| IP报头和原始数据报数据的前8个字节  |
+-------------------------------+
```




# 相关

* RFC 792
