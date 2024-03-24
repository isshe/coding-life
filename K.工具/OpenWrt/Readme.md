# OpenWrt

​有关详细说明可以到[Openwrt 官网](https://openwrt.org)查看

[TOC]

## OpenWrt 启动过程

> uboot --> kernel --> 用户第一个进程（/etc/preinit） --> /sbin/init --> /sbin/procd*

`procd` 最终会成为用户空间第一个进程（PID=1）。
主要工作：watchdog 的初始化、热插拔的监听（比如：按键）、启动 ubusd、执行 `/etc/preinit`（初始化设备）、执行 `/etc/rc.d` 下的脚本。

## OpenWrt 进程间通信

openwrt 间的进程通信是先通过向 `ubusd`（通信协议：unix socket）进程注册 RPC 方法供外部进程调用。我们可以通过 `ubus` 命令去调用已注册的 RPC 方法：

> ubus -v list //查看已注册的 RPC 接口
> ubus call network restart //调用 netifd 注册的 restart 方法

## OpenWrt 配置

OpenWrt 配置文件保存在 `/etc/config` 目录，可以通过文本编辑和 `uci` 去修改配置，`uci` 语法如下：

```bash
# 配置格式：
> config section
> ​	option key 'value'
> ​	......
> 命令：
> uci get </etc/config下的配置文件名>.section.key # 获取key的值
> uci set </etc/config下的配置文件名>.section.key=value # 设置key的值value
> uci commit </etc/config下的配置文件名> # 提交，**每次提交都需要加上配置文件名，不是的话可能会影响到其他配置的提交**
```

## OpenWrt 网络相关

网络配置文件是 `/etc/config/network`（交换片及网口相关配置）；
无线配置文件`/etc/config/wireless`；
网络的管理进程是`netifd`，主要负责生效网络、监听网络变化（会调用/etc/hotplug.d 下的脚本）、根据网络配置调用 `/lib/netifd/proto` 下的脚本拉起对应进程（dhcp 协议会拉起 udhcp，pppoe 会拉起 pppd ...）。

网络配置解析：

以 7621 交换片 mt7530 为例：
	port6 口跟 CPU 相连
	switch0 和 eth0 是交换片的别名

```
config switch
	option name 'switch0'
	option reset '1'
	option enable_vlan '1'	//使能vlan，如果需要配置vlan，需要打开

config switch_vlan
	option device 'switch0'
	option vlan '5'
	option vid '5'
	option ports '4 6t'		//在port4、port6口设置vlan5标记，带‘t’代表出去是带vlan tag标签的，								port6带‘t’是为了让CPU能设别是哪个网络接口，最终会生成eth0.5的网络接口
config interface 'wan0'
	option ifname 'eth0.5'
	option metric '1'
	option proto 'dhcp'		//将eth0.5配置成dhcp协议（动态获取ip）,跃点数为1，别名为wan0
```

查看网络状态相关：

```markdown
1、查看 eth0.5 接口状态（link 状态、ip、是否丢包等）
命令：ifconfig eth0.5

2、查看桥接
命令：brctl show

3、查看路由
命令：route（ip route）

4、查看交换片
1）查看有多少交换片
命令：swconfig list

2）查看交换片端口的 link 状态
命令：swconfig dev switch0 show|grep link

3）重启交换片
命令：swconfig dev switch0 set reset

4）查看 swconfig 帮助
命令：swconfig dev switch0 help

5、通过 ubus 查看网络状态
1）查看接口状态
命令：ubus call network.interface.wan0 status
2）up/down 接口
命令：ubus call network.interface.wan0 up/down

	注：由于Openwrt的网络是通过netifd来管理网络的，所以最好通过netifd提供的接口去修改网络，防止netifd没感知到产生冲突造成不必要的网络问题。
```

在排查网络问题时的一些体会：

```markdown
1、先检查物理连接是否正常
2、排除上级设备和下级设备的问题
3、查看网络配置是否正常，之前出现过网络配置丢失的问题（在系统第一次启动），这跟 uci commit 没有加上配置文件名有关，不加文件名是全局提交，会影响到其他正在修改的配置
4、查看路由是否正常，查看桥接是否正常
5、如果是设备获取不到 ip，可以先看看 udhcp 和 netifd 有没挂掉（一般 netifd 有问题会在/tmp 有相关的 core 文件），看看错误日志之类的（logread），再者可以抓一下包（dhcp 协议一般抓 67or68 端口）分析一下
6、如果没有给下级设备分配 ip，可以看看 dnsmasq 进程是否有问题，看看/etc/config/dhcp 配置，再者可以抓一下 dhcp 包
7、如果是网络不通也有可能是 firewall、auth 拦截
8、查看网络接口状态和交换片状态是否正常
	1）如果发现交换片接口不收包，先看看有没日志之类的（dmsg/logread）,没有可以试着重启一下交换片看看
9、最后什么日志都没有，重启各种进程交换片都不行，重启设备也不行，那很可能硬件有问题

	注：
	总而言之，就是先看看配置是否正常，进程是否正常，硬件是否正常
```

个别案例分析：

```markdown
1、将 wan 口设置成静态 ip 插拔网线导致默认路由丢失
正常路由：
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
default         192.168.2.1     0.0.0.0         UG    1      0        0 eth0.5
172.16.0.0      192.168.2.1     255.255.255.0   UG    1      0        0 eth0.5
172.17.0.0      *               255.255.0.0     U     1      0        0 eth0.3
172.116.0.0     *               255.255.0.0     U     1      0        0 eth0.1
192.168.2.0     *               255.255.255.0   U     1      0        0 eth0.5

丢失默认路由：
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
172.17.0.0      *               255.255.0.0     U     1      0        0 eth0.3
172.116.0.0     *               255.255.0.0     U     1      0        0 eth0.1
192.168.2.0     *               255.255.255.0   U     0      0        0 eth0.5

问题原因：
设备有一个交换片端口检测脚本 switch_ports_status 在端口 down 掉时会进行 ifconfig down 操作，端口 up 后会进行 ifconfig up 操作，而添加默认路由是 netifd 里进行的，netifd 在静态 ip 的时候并没感知到 up，所以没有触发添加默认路由的操作，导致默认路由丢失。

解决方法：
通过 netifd 对外提供的 rpc 接口对网口进行 down/up 就可以触发 netifd 添加默认路由：
ubus call network.interface.wan0 down/up
```

```markdown
2、网络配置概率性丢失的问题
问题原因：
在系统第一次起来，会进行各种配置的初始化，都是通过 uci 进行操作；而在网络配置在进行配置的途中刚好 nproto 脚本执行了 uci commit（全局提交）导致网络配置部分丢失。
解决方法：
在进行配置提交时最好加上配置的文件名，只提交自己修改的部分：uci commit nproto
```

```markdown
3、系统保留配置升级后系统第一次起来 dnsmasq 进程没起来的问题
问题原因：
首先/etc/rc.d 下的脚本是已启动顺序命名的软连接：
	/etc/rc.d/S19dnsmasq -> ../init.d/dnsmasq
我们自己维护了一份 dnsmasq 启动脚本，修改了 dnsmasq 的启动顺序与系统自带的发生了冲突。在系统第一次启动时，/etc/rc.d 的第一个启动脚本中 boot 会执行/etc/uci.default 下的脚本，我们也在里面加入了我们自己的脚本 95-base-config，这个脚本会把我们自己的 dnsmasq 脚本替换掉原有的 dnsmasq 脚本，但由于启动顺序改变了，在/etc/rc.d 下的 dnsmasq 的名字发生了变化，以至找不到原来的文件，导致 dnsmasq 在第一起起来时没有被运行。

解决方法：
将系统的 dnsmasq 的启动顺序改成与我们的 dnsmasq 的启动顺序一致
```

## OpenWrt 添加新型号（以 7621 为例）

```markdown
1、添加 config 配置项（make menuconfig 可以看到相关配置项）
在 target/linux/ramips/image/mt7621.mk 加入如下示例：
	define Device/mt7621_slt868
      DTS := MT7621_SLT868
      IMAGE_SIZE := $(ralink_default_fw_size_16M)
      DEVICE_TITLE := MT7621 SLT868 Device
	endef
	TARGET_DEVICES += mt7621_slt868
2、在 target/linux/ramips/dts 创建相关的文件
3、target/linux/ramips/base-files/lib/ramips.sh 设备的名称
4、target/linux/ramips/base-files/lib/upgrade/platform.sh 设置更新固件格式
5、target/linux/ramips/base-files/etc/board.d/01_leds 设置 led
6、target/linux/ramips/base-files/etc/diag.sh 设置状态指示灯
7、target/linux/ramips/base-files/etc/board.d/02_network 设置网络

	注：
	如果make menuconfig看不到配置项，执行一下以下命令：
	rm tmp/ -rf
	touch target/linux/ramips/Makefile
```
