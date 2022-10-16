[TOC]

route
---

# 删除路由

```
# 删除默认路由
route del defaul gw 192.168.122.0

# 删除指定接口的路由
route del -net 192.168.122.0 netmask 255.255.255.0 dev virbr0
```
