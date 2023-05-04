# 核心技术点

本文档用于汇总学习过程中遇到的核心技术点。
记录逻辑：假设相关功能没有实现，简要描述重新实现的要点。

TODO: 补充以下相关的

- lua 堆栈解读（或许应该放到 Luajit 那边去）

## Lua 接口/数据(变量)注入

> 有些函数不需要 require 都能进行使用，这是因为 OpenResty 自动进行了相关工作。

- 在[初始化 Lua 虚拟机](010-lua-vm-init.md)时，进行 Lua 接口注入。
- 采用表（table）的形式进行组织 —— lua_createtable。
- 创建一个表，然后设置键值对，把 key（如 "get"） 和 C 函数（lua_pushcfunction）或值(lua_pushinteger 等)进行绑定。

## cosocket

- 基于 Lua 协程和 Nginx 的事件模型以及异步 I/O：Lua 接口调用 cosocket 相关函数时，底层的 listen、send 等接口采用非阻塞的方式，使这些调用立即返回。根据返回的状态，设置并监听相关读写事件，然后 yield 让出当前 Lua 协程的执行权，继续执行其他协程。当等待的事件发生后，事件处理函数 resume 唤醒对应的协程，继续执行。

详见 [OpenResty cosocket](014-cosocket.md)。

## Pipe 设计要点

- 开 2 或 3 个 pipe，分别用于 stdin/stdout/stderr —— 把标准输入、输出、错误重定向到这些 pipe 的 fd。
- 使用 exec 系列函数来执行命令
- 读或写数据(命令输入或命令输出)时，先进行一次读写，如果无法一次完成，则设置对应的读写事件，然后 yield 让出执行权；后续通过事件模块 resume 获取执行权。

详见 [OpenResty Pipe](017-pipe.md)。

## Lua 代码加载要点

- 为用户代码包裹上 "return function() ... end" 构成闭包工厂，用于生成对应闭包。
- 在注册表中对 Lua 闭包工厂进行缓存，用 key（用 ngx_http_lua_gen_chunk_cache_key 等函数生成） 或者 reference 进行查找。

详见 [OpenResty Lua 代码缓存](018-ngx_lua_cache.md) 或 [OpenResty 的 rewrite_by_lua](005-rewrite_by_lua.md)。

## shared dict 设计要点

- 基于 Nginx 的共享内存(shm) 和 slab 内存管理机制：创建一块共享内存区域，利用 slab 分配机制来进行分配。
- 访问/修改/分配共享内存时，都采用加锁的方式，保证数据一致性。
- 键值对使用红黑树和队列（LRU 队列）来进行管理。

详见 [OpenResty 共享内存](019-ngx_shared_dict.md)。
