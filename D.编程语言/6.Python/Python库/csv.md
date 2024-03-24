[TOC]

csv
---

# 作用
读写 csv 文件。

# 安装
无需安装，python 内置库。

# 使用
```
# 打开/创建文件
fd = open(filename, "w")

# 写入头部
fieldnames=["GET", "POST", "PAYLOAD"]
cw = csv.DictWriter(fd, fieldnames=fieldnames)
cw.writeheader()     #将fieldnames写入标题行

# 写入行
value = "xxx"
value = value.encode('utf-8')
cw.writerow({'GET': 404, 'POST': 404, 'PAYLOAD': value})

# 关闭文件
fd.close()
```