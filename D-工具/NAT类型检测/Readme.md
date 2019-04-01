
[TOC]

# NAT类型检测工具
* 当前对称型和端口限制型无法区分。因为UDP发送给两个服务器的映射端口一样，导致对称型被识别为端口限制型。


# 1.前置依赖
* luasocket: luarocks install luasocket
* lua-cjson: luarocks install lua-cjson

# 2.使用方法
* 修改stun_server3.lua里面的IP+其他服务器IP，然后运行第一个服务器。
* 修改stun_server3.lua里面的IP+其他服务器IP，然后运行第二个服务器。
* 修改stun_client3.lua里面的客户端IP，服务器IP（两个）
* 运行stun_client3.lua，进行测试。

# 3.测试方法
* 测试流程
![测试流程](./nat_type_check.png)

# X.参考
* https://zh.wikipedia.org/wiki/STUN