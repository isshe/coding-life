# 核心技术点

本文档用于汇总学习过程中遇到的核心技术点。

TODO: 补充以下相关的
- lua 接口注入、lua 堆栈解读
- cosocket 相关
  - 如何监听、如何调度等

## Pipe 设计要点

- 开 2 或 3 个 pipe，分别用于 stdin/stdout/stderr —— 把标准输入、输出、错误重定向到这些 pipe 的 fd。
- 使用 exec 系列函数来执行命令
- 读或写数据(命令输入或命令输出)时，先进行一次读写，如果无法一次完成，则设置对应的读写事件，然后 yield 让出执行权；后续通过事件模块 resume 获取执行权。

详见 [OpenResty Pipe](017-pipe.md)。

## Lua 代码加载要点

- 为用户代码包裹上 "return function() ... end" 构成闭包工厂，用于生成对应闭包。
- 在注册表中对 Lua 闭包工厂进行缓存，用 key（用 ngx_http_lua_gen_chunk_cache_key 等函数生成） 或者 reference 进行查找。

详见 [OpenResty Lua 代码缓存](018-ngx_lua_cache.md) 或 [OpenResty 的 rewrite_by_lua](005-rewrite_by_lua.md)。
