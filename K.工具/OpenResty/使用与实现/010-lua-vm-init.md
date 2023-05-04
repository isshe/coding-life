# Lua VM 初始化

上一篇关于 ngx.log 文章中，跟踪相关代码时思路几近断裂，因此赶紧来补补 Lua VM 初始化相关内容。

目的：

- Lua VM 初始化的时机？（什么时候初始化）
- 了解 Lua VM 初始化做了哪些工作？为什么这么做（选）？
- Lua 虚拟机是共用一个？还是每次调用 Lua 代码起一个？
- 注入的接口和 _G 是什么关系？这些接口也在 _G 中吗？

## Lua VM 初始化时机

从上一篇文章中，我们已经拿过一个 Lua VM 初始化相关的堆栈，这里截取一部分：

```lua
#4  0x000055555570b955 in ngx_http_lua_init_vm (new_vm=0x555555891080,
    parent_vm=0x0, cycle=0x555555885740, pool=0x5555558856f0,
    lmcf=0x555555891080, log=0x55555583d280 <ngx_log>, pcln=0x0)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_util.c:3896
#5  0x00005555556fb450 in ngx_http_lua_init (cf=0x7fffffffd7e0)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_module.c:865
#6  0x00005555555f4ddc in ngx_http_block (cf=0x7fffffffd7e0,
    cmd=0x55555581a200 <ngx_http_commands>, conf=0x555555886a20)
    at src/http/ngx_http.c:310
#7  0x00005555555b9ec7 in ngx_conf_handler (cf=0x7fffffffd7e0, last=1)
    at src/core/ngx_conf_file.c:463
#8  0x00005555555b99cc in ngx_conf_parse (cf=0x7fffffffd7e0,
    filename=0x555555885958) at src/core/ngx_conf_file.c:319
#9  0x00005555555b4f87 in ngx_init_cycle (old_cycle=0x7fffffffd9b0)
    at src/core/ngx_cycle.c:284
#10 0x0000555555592fe8 in main (argc=3, argv=0x7fffffffdd58)
    at src/core/nginx.c:295
```

从这个调用栈可以看到，是在解析 lua-nginx-module 配置阶段（准确来说是 postconfiguration 阶段）初始化的 Lua VM。

```lua
#0  ngx_http_lua_init_vm (new_vm=0x1, parent_vm=0x0, cycle=0x0, pool=0x0,
    lmcf=0xffffffffffffffff, log=0x400002001, pcln=0x7ffe9ee23400)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_util.c:3882
#1  0x0000558146dea07f in ngx_http_lua_create_ctx (r=0x5581471b6120)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_util.h:305
#2  0x0000558146dea50d in ngx_http_lua_rewrite_handler (r=0x5581471b6120)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_rewriteby.c:92
#3  0x0000558146cd558e in ngx_http_core_rewrite_phase (r=0x5581471b6120,
    ph=0x5581471bcf08) at src/http/ngx_http_core_module.c:939
#4  0x0000558146cd53de in ngx_http_core_run_phases (r=0x5581471b6120)
...
```

设置 `lua_code_cache off` 指令关闭缓存后，每次请求都会重新初始化 Lua VM。

## Lua VM 初始化流程

```
- ngx_http_lua_init_vm
    \- ngx_pool_cleanup_add：添加内存池清理函数——用于清理 Lua VM（ngx_http_lua_cleanup_vm）
    \- ngx_http_lua_new_state：创建 Lua VM 实例
        \- luaL_newstate：新分配一个 lua_State 结构
        \- luaL_openlibs：打开所有标准 Lua 库到给定 state
        \- lua_getglobal(L, "package")：将全局 package 的值压入堆栈。返回该值的类型
            \- 返回值是代表类型，那为什么不直接用，而是下面进行 istable 判断？[1]
        \- if (!lua_istable(L, -1))：检查栈顶值是否是 table（package 是一个 table），不是直接报错返回。
        \- if (parent_vm)：如果有父级 VM，则使用它的 path 和 cpath，
        \- else：否则自行解析设置 path、cpath，从编译指定的默认路径和从 nginx 配置中得到相关路径
        \- ngx_http_lua_init_registry：初始化 Lua 注册表 [2]
            \- 注册一个表以可靠地锚定 lua 协程，初始大小为 32 个 KV。
            \- 为 Lua 套接字连接池表注册一个表
            \- 注册一个表以缓存用户代码
        \- ngx_http_lua_init_globals：初始化 lua 全局变量
            \- ngx_http_lua_inject_ndk_api：注入 ngx devel kit 的 API（如果有定义 NDK 宏的话）
            \- ngx_http_lua_inject_ngx_api：注入 ngx API，实际上就是创建 ngx 表和填充这个表。
                \- 例如：把 ngx.log 等于 ngx_http_lua_ngx_log
                \- 例如：ngx.HTTP_GET = NGX_HTTP_GET
    \- luaopen_ffi：加载 FFI 库，因为 cdata 需要
    \- if (lmcf->preload_hooks)：检查是否有需要预加载的 hook
        \- ngx_http_lua_probe_register_preload_package：注册第 3 方模块的预加载 hook
            \- 示例：package=ngx.upstream，loader=ngx_http_lua_upstream_create_module
    \- lua_pcall：调用 require("resty.core")，不知何意
    \- ngx_http_lua_inject_global_write_guard：注入全局写保护，使用全局变量时会得到告警提示
        \- 执行了一段 Lua 代码，设置了 _G 的元表，重载了 __newindex
```

可以看到，Lua VM 初始化主要做了以下事情：
- 新建 state 实例
- 打开标准库
- 设置搜索路径（path、cpath）
- 注册一些表
- 注入 Lua 接口、变量：重点关注，很多 Lua 接口都是在这里注入。
- 预加载第三方 hook
- 对 _G 进行写保护


那么，Lua 虚拟机是共用一个？还是每次调用 Lua 代码起一个？

- lua_code_cache 开启时，共用一个
- lua_code_cache 关闭时，每个请求一个

## 总结

1. Lua VM 初始化的时机？（什么时候初始化）

答：lua_code_cache 开启时，在配置解析阶段（准确来说是 postconfiguration 阶段）；lua_code_cache 关闭时，每次请求都会重新初始化 Lua VM，在 rewrite 等实际设置了处理程序（handler）的阶段。

1. Lua 虚拟机是共用一个？还是每次调用 Lua 代码起一个？

答：lua_code_cache 开启时，共用一个；lua_code_cache 关闭时，每个请求一个。

1. 注入的接口和 _G 是什么关系？这些接口也在 _G 中吗？

答：是的，注入的接口就在 _G 或者 _G 的元表中。

## 疑问

- [1]：lua_getglobal 能直接返回类型，为什么不直接用而是用 istable 来判断？
- [2]：注册的几个表的用途是什么？在哪里用了？（ngx_http_lua_init_registry）
