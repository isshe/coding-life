
# ethtool

```bash
# 查看 udp 的哈希算法
ethtool --show-ntuple eth0 rx-flow-hash udp4

# 查看 tcp 的哈希算法
ethtool --show-ntuple eth0 rx-flow-hash tcp4

# 修改 udp 的的哈希算法
ethtool --config-ntuple eth0 rx-flow-hash udp4 sdfn

# 查看网卡的接收队列的收报情况
ethtool -S eth0 | grep rx_ucast_packets

# 查看网卡的发送队列的收报情况
ethtool -S eth0 | grep tx_ucast_packets

# 查看网卡的通道配置
ethtool -l eth0

# 配置网卡的接收通道的数量
ethtool -L eth0 combined 30
```
