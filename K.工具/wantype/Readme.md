[TOC]

# wantype
上网类型检测。

## 原理
发送发现包，探测pppoe服务器和dhcp服务器。

## 使用场景
* 上网类型推荐
* 默认上网类型设置

## 使用方法
```bash
wantype eth0
wantype eth0 -t 10
```