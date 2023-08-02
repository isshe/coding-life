


# Failed to create /init.scope control group: Read-only file system

- 报错原因

cgroup v2 存在限制。

- 解决方案

切回 cgroup v1。

- 错误命令：

```bash
docker run --privileged=true --volume /sys/fs/cgroup:/sys/fs/cgroup:ro --entrypoint=/sbin/init --name test-systemd-rocky9  -d -t my-systemd-rockylinux:9
```

- 原始信息

```
Failed to create /init.scope control group: Read-only file system
Failed to allocate manager object: Read-only file system
[!!!!!!] Failed to allocate manager object.
Exiting PID 1...
```

- 在 /etc/default/grub 文件 GRUB_CMDLINE_LINUX 参数后面添加 systemd.unified_cgroup_hierarchy=0

```bash
# vi /etc/default/grub
GRUB_TIMEOUT=5
GRUB_DISTRIBUTOR="$(sed 's, release .*$,,g' /etc/system-release)"
GRUB_DEFAULT=saved
GRUB_DISABLE_SUBMENU=true
GRUB_TERMINAL_OUTPUT="console"
GRUB_CMDLINE_LINUX="resume=/dev/mapper/rl-swap rd.lvm.lv=rl/root rd.lvm.lv=rl/swap rhgb quiet crashkernel=1G-4G:192M,4G-64G:256M,64G-:512M systemd.unified_cgroup_hierarchy=0"
GRUB_DISABLE_RECOVERY="true"
GRUB_ENABLE_BLSCFG=true
```

- 更新引导

```bash
grub2-mkconfig
```

- 重启

```bash
reboot
```
