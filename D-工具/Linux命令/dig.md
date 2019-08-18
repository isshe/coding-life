
[TOC]


dig
---

# 用法

# 选项

# 示例
## 分级查询
> dig +trace www.isshe.xyz

## 单独查看每一级域名的NS记录
> dig ns xyz
> dig ns isshe.xyz

## 简化输出结果
> dig +short ns isshe.xyz

## 反向查询
> dig -x xxx.xxx.xxx.xxx

## 指定记录类型
```
dig a github.com
dig ns github.com
dig mx github.com
```

# 疑问

# 参考