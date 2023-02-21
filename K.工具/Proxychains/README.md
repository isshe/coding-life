
# proxychains

用于对指定命令进行代理。

示例：

```
proxychains git pull
```

- 代码仓库：https://github.com/haad/proxychains.git

## 安装

```bash
git clone https://github.com/haad/proxychains.git
cd proxychains
./configure
make
sudo make install
```

## 配置

添加配置文件：/etc/proxychains.conf

```ini
[ProxyList]
http    192.168.50.130 7890
```

## 使用

对可执行文件进行软链（或 alias），方便使用：

```
ln -s /usr/local/bin/proxychains4 /usr/bin/pc
```

使用：

```bash
pc git pull
```
