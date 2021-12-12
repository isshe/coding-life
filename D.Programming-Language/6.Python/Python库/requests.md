[TOC]

requests
---

# 作用
发送HTTP请求。

# 安装
```
pip install requests
```

# GET请求
```python
import requests

res = requests.get(url)     # 发送请求
# print(res.text)           # 获取请求，得到的是json格式
# print(res.headers)
# print(res.status_code)
return res.status_code
```

# POST请求
```python
import requests

res = requests.post(url, data=data)
# print(res.text)           # 获取请求，得到的是json格式
# print(res.headers)
# print(res.status_code)
return res.status_code
```