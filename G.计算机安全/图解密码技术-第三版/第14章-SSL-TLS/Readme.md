[TOC]

SSL/TLS为了更安全的通信


# 什么是 SSL/TLS？
* 是世界上应用最广泛的密码通信方法。
* SSL: Secure Socket Layer.
* TLS: Transport Layer Security.
  * 相当于 SSL 的后续版本。
  * 由 (下层)TLS 记录协议 (TLS record protocol) 和 (上层)TLS 握手协议 (TLS handshake protocol) 这两层协议叠加而成。

# TLS 记录协议
TLS 记录协议负责：消息的压缩、加密、数据的认证。

# TLS 握手协议
* TLS 握手协议：加密之外的其他各种操作。
  * 握手协议
  * 密码规格变更协议
  * 警告协议
  * 应用数据协议