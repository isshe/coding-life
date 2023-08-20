如何安装内核调试信息
---

原本以为是比较简单的软件安装，没想到还折腾了挺久，因此记录一下。

# Ubuntu

> 以 20.04 为例。

- 导致 GPG 秘钥

```bash
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys C8CAB6595FDFF622
```

- 添加仓库配置

```bash
codename=$(lsb_release -c | awk  '{print $2}')
sudo tee /etc/apt/sources.list.d/ddebs.list << EOF
deb http://ddebs.ubuntu.com/ ${codename}      main restricted universe multiverse
deb http://ddebs.ubuntu.com/ ${codename}-security main restricted universe multiverse
deb http://ddebs.ubuntu.com/ ${codename}-updates  main restricted universe multiverse
deb http://ddebs.ubuntu.com/ ${codename}-proposed main restricted universe multiverse
EOF
```

- 安装调试信息包

```
sudo apt-get update
sudo apt-get install linux-image-$(uname -r)-dbgsym
```

**如果没找到怎么办？直接更新内核！**

- 先找到一个有 dbgsym 的版本：

```bash
# 删除小版本号进行搜索
apt list 'linux-image-5.4.0-*-generic-dbgsym'
```

- 安装

```
sudo apt install linux-image-5.4.0-128-generic linux-headers-5.4.0-128-generic
```

**如果版本已经是最新？选择稍低的一个版本进行安装。**然后修改引导：

- 列出引导项

```bash
sudo grub-mkconfig | grep -iE "menuentry 'Ubuntu, with Linux" | awk '{print i++ " : "$1, $2, $3, $4, $5, $6, $7}'
```

结果如：

```
0 : menuentry 'Ubuntu, with Linux 5.4.0-131-generic' --class ubuntu
1 : menuentry 'Ubuntu, with Linux 5.4.0-131-generic (recovery mode)'
2 : menuentry 'Ubuntu, with Linux 5.4.0-128-generic' --class ubuntu
3 : menuentry 'Ubuntu, with Linux 5.4.0-128-generic (recovery mode)'
```

- 修改引导：（使用 5.4.0-128 启动）

```bash
# sudo vi /etc/default/grub
GRUB_DEFAULT="1>2"
```

意思：一级菜单选第 1 项，二级菜单选第 2 项（上面的序号）

- 重启

```
reboot
```

重新启动后，继续安装 dbgsym 包即可。
