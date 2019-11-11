
# 汇总

## 工具
### exiftool
Exif(exchange image file format, 交换图像文件格式)：定义了如何存储图像和音频文件的标准。
获取文件信息。
### Beautiful Soup
快速解析HTML和XML文档。

### Pillow
图片相关。可以`_getexif`。

### sqlite3
```python
import sqlite3

conn = sqlite3.connect(db)
c = conn.cursor()
c.execute("SELECT * from xxx")
for row in c:
    print(row[0], row[1])
```

### dpkt

```python
eth = dpkt.ethernet.Ethernet(buf)
ip = eth.data   # 这里是二进制数据
src = socket.inet_ntoa(ip.src)
tcp = ip.data
tcp.port
```

### geoip2
```python
from geoip2 import database

db = database.Reader(deo_db_file_mmdb)
rec = db.city(ip)
rec.subdivisions[0].names["zh-CN"]
```