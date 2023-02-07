
# Rocky Linux 静态 IP 配置

- 修改配置

**注意：文件名和接口名相关**

```bash
$ vi /etc/sysconfig/network-scripts/ifcfg-enp1s0

# BOOTPROTO=dhcp 改为
BOOTPROTO=static

# 增加配置：
IPADDR=192.168.122.22
PREFIX=24
GATEWAY=192.168.122.1
DNS1=192.168.122.1
NETMASK=255.255.255.0
```

- 重启接口

```
sudo nmcli connection reload
sudo ifdown enp1s0
sudo ifup enp1s0
```

- 检查 NetworkManager 状态

```
sudo systemctl status NetworkManager
```
