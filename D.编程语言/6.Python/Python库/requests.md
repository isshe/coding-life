[TOC]

requests
---

# 作用
发送 HTTP 请求。

# 安装
```
pip install requests
```

# GET 请求
```python
import requests

res = requests.get(url)     # 发送请求
# print(res.text)           # 获取请求，得到的是 json 格式
# print(res.headers)
# print(res.status_code)
return res.status_code
```

# POST 请求
```python
import requests

res = requests.post(url, data=data)
# print(res.text)           # 获取请求，得到的是 json 格式
# print(res.headers)
# print(res.status_code)
return res.status_code
```