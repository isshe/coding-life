# 常见用法

## 验证自签证书是否是指定 CA 证书签发

```bash
# 使用证书文件进行验证
openssl verify -CAfile ca.crt /path/to/cert/example.crt

# 直接连接服务器进行验证
openssl s_client -connect 127.0.0.1:12346 -CAfile ca.crt
```

## 自签证书签发
