# nmap

## 获取使用的 TLS 算法

```bash
nmap -sV --script ssl-enum-ciphers -p 443 test.com
```
