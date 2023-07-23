# C 语言 CPU 高占用分析

需包含：

- 示例代码
- 分析并得到具体调用栈等信息
- 生成相关图表（火焰图、占用图等）的步骤

我们以编写 OpenResty 第三方模块为例。

```bash
git clone https://github.com/openresty/openresty.git
cd openresty
./util/mirror-tarballs
```

```bash
cd lua-resty-highcpu
make config

# 测试跑起来
make wrk

# 进行分析

# 生成火焰图
```
