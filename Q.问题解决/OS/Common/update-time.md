# 时间同步

当发现系统时间不对时，可以采用以下措施。

## 从硬件时间同步

- 用 `timedatectl` 命令检查时间，确认 RTC 时间是正确的（也可以用 hwclock 和 date 来确认）

```bash
      Local time: 日 2023-09-17 22:31:57 CST
  Universal time: 日 2023-09-17 14:31:57 UTC
        RTC time: 日 2023-09-17 14:39:32
       Time zone: Asia/Shanghai (CST, +0800)
     NTP enabled: n/a
NTP synchronized: no
 RTC in local TZ: no
      DST active: n/a
```

- 从硬件时间同步到系统时间

```bash
hwclock -s
# 或
hwclock --hctosys
```

更多 hwclock 内容可以用 `hwclock --help` 获取


## 从互联网同步

```bash
# 不同系统可以用不同的软件进行
# yum install utp ntpdate
sudo ntpdate time.nist.gov
# rockylinux 9（未验证）
chronyc -a makestep
```

## 手动调整

```bash
sudo date -s "2023-09-17 10:30:00"
```

## 设置时区

```bash
# examples
sudo timedatectl set-timezone UTC
sudo timedatectl set-timezone Asia/Shanghai
```
