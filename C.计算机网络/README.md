# 计算机网络

* 主要学习`网络协议`。
* 先理论/原理；再应用；后实现；

## 协议列表

应用层

* BGP DHCP DNS FTP HTTP IMAP LDAP MGCP NNTP NTP POP ONC/RPC RTP RTSP RIP SIP SMTP SNMP SSH Telnet XMPP 更多...

传输层

* TCP UDP TLS／SSL DCCP SCTP RSVP 更多...

网络层

* IP IPv4 IPv6 ICMP ICMPv6 ECN IGMP OSPF IPsec 更多...

链接层

* ARP NDP Tunnels L2TP PPP MAC Ethernet DSL ISDN FDDI 更多...

# 无线网络常用调试命令

## iwconfig

* 查看启动的无线网络信息

```shell
iwconfig
```

## brctl

* brctl: 桥控制

```shell
brctl show
```

## swconfig

* 交换片控制

```shell
swconfig dev switch0 show
```

## arp

* 查看arp信息

```
arp
```
