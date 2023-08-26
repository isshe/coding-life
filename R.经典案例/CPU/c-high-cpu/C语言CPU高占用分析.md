# C 语言 CPU 高占用分析

**切去整理 Xray 和 Edge，完了再继续这个。**

需包含：

- 示例代码
- 分析并得到具体调用栈等信息
- 生成相关图表（火焰图、占用图等）的**完整**步骤
  - xray 的火焰图生成包含了不少自定义的脚本，这里需记录开源的工具的步骤！对比 xray 的优化。
- 完成工具链，最终实现一个命令就能跟踪指定进程，生成火焰图。（也就是实现一个完整 ON CPU 工具）

我们以编写 OpenResty 第三方模块为例。

```bash
git clone https://github.com/openresty/openresty.git
cd openresty
./util/mirror-tarballs
```

```bash
cd lua-resty-highcpu
make conf

# 测试跑起来
make wrk

# 生成火焰图

# 进行分析
```
