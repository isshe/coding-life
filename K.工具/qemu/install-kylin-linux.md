# 在 M1 Mac Mini 上安装银河麒麟服务器 ARM64 版本

本教程介绍如何在 M1 Mac Mini 上通过 QEMU 虚拟机安装银河麒麟服务器 ARM64 版本。

## 准备工作

### 1. 安装 QEMU

由于 Homebrew 安装的 QEMU 可能存在兼容性问题，建议自行编译安装：

```bash
# 下载并解压 QEMU 源码
wget https://download.qemu.org/qemu-9.1.1.tar.xz
tar xf qemu-9.1.1.tar.xz
cd qemu-9.1.1

# 配置和安装
./configure --prefix=/opt/qemu-9.1.1-bin
make -j6
sudo make install
```

### 2. 准备系统镜像

1. 从银河麒麟官网下载 ARM64 架构的服务器版本：
   - 下载文件：`Kylin-Server-V10-SP3-2403-Release-20240426-arm64.iso`
   - 建议下载完成后校验 ISO 文件的完整性

2. 创建虚拟磁盘：
```bash
qemu-img create -f qcow2 kylin-v10-sp3-aarch64.qcow2 50G
```

## 启动虚拟机

### 1. 设置 UEFI 固件路径

```bash
bios=/opt/qemu-9.1.1-bin/share/qemu/edk2-aarch64-code.fd
```

### 2. QEMU 启动命令

```bash
qemu-system-aarch64 \
    -m 4096 \
    -cpu cortex-a72 \
    -M virt,gic-version=3,highmem=on \
    -smp 2,cores=2,threads=1,sockets=1 \
    -bios $bios \
    -device virtio-net-pci \
    -device nec-usb-xhci \
    -device usb-kbd \
    -device usb-tablet \
    -device virtio-gpu-pci \
    -drive file=./Kylin-Server-V10-SP3-2403-Release-20240426-arm64.iso,format=raw,if=none,id=cdrom,media=cdrom \
    -device virtio-scsi-pci \
    -device scsi-cd,drive=cdrom \
    -drive file=kylin-v10-sp3-aarch64.qcow2,format=qcow2,if=none,id=hd0 \
    -device virtio-blk-pci,drive=hd0 \
    -serial telnet::4468,server,nowait \
    -vnc :2
```

关键参数说明：

- `-m 4096`: 分配 4GB 内存（可根据实际需求调整）
- `-cpu cortex-a72`: 使用 ARM Cortex-A72 CPU 架构
- `-M virt,gic-version=3,highmem=on`: 配置虚拟机类型为 virt，启用 GICv3 和高内存支持
- `-smp 2,cores=2,threads=1,sockets=1`: 配置双核 CPU
- `-vnc :2`: 启用 VNC 服务器（访问端口 5902）
- `-serial telnet::4468`: 启用串口访问（用于文本模式安装）

注意：本配置未使用硬件加速（-accel hvf），因为启用 macOS 硬件加速后可能导致系统引导循环：

```bash
# 如果系统稳定，可尝试添加以下参数启用硬件加速
-accel hvf
```

### 3. 添加引导参数

1. 通过 VNC 客户端连接（端口 5902），(MACOS 可以 Jump Desktop VNC 客户端，系统自带的客户端会要求输入密码)
2. 在 UEFI 引导菜单按 'e' 键
3. 添加以下引导参数：

```
inst.debug=1 inst.text console=ttyAMA0,115200 inst.nokill
```

## 系统安装

添加引导参数后，安装界面将从 VNC 图形界面切换至串口终端模式。使用以下命令连接到串口终端：

```bash
telnet localhost 4468
```

在串口终端中，您可以完成所有必要的系统配置：
- 磁盘分区和挂载点设置
- 创建管理员账号
- 设置 root 密码
- 配置系统选项

安装程序中：
- `[x]` 表示已完成或无需配置的选项
- `[!]` 表示必须配置的选项

安装完会出现以下提示：

```
Installation complete. Press ENTER to quit: [ 1858.602878][  0] hrtimer: interrupt took 119746992 ns
anaconda halting due to nokill flag.
The system will be rebooted when you press Ctrl-Alt-Delete.
```

此界面中退出而不是重启，因此切换到启动命令，直接 Ctrl+C 中断启动命令。
