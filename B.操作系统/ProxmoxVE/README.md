# Proxmox VE

记录一些常用的操作，如果内容较多，则开新的文档，然后链接过去。

## 如何删除 ZFS Pool/硬盘？

DataCenter > NODE > Disks > ZFS > 选中要删除的条目 > 右上角 More > Destroy。

如果不是确定不再需要此硬盘或者知道立即就会重新初始化，则直接删除，否则去掉勾选 `Cleanup Disk` 和 `Cleanup Storage Configuration` 再删除。

## 如何恢复 ZFS Pool/硬盘？

如果删除 ZFS Pool 时，清理了硬盘，那么直接重新创建即可，没法恢复了。

以下是没有清理数据及配置的情况下的恢复步骤：

- 进入后台，SSH 进入或者是界面上点击 Console/Shell 进入
- 查看被删除的 ZFS Pool 的信息：

```bash
zpool import -D
# 或，（这个预期无法列出已删除的 Pool）
zpool import
```

- 恢复（上面哪个命令能列出目标 Pool，则使用哪个命令来恢复）

```bash
zpool import -D <POOL NAME>
# 如
zpool import -D z2tb
```
