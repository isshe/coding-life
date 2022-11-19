
# Lua VM 初始化

上一篇关于 ngx.log 文章中，跟踪相关代码时思路几近断裂，因此赶紧来补补 Lua VM 初始化相关内容。

目的：

- Lua VM 初始化的时机？（什么时候初始化）
- 了解 Lua VM 初始化做了哪些工作？为什么这么做（选）？
- Lua 虚拟机是共用一个？还是每次调用 Lua 代码起一个？

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

显然，是在解析 lua-nginx-module 配置阶段（准确来说是 postconfiguration 阶段）。

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
    \- luaopen_ffi：加载 FFI 库，因为 cdata 需要
    \- if (lmcf->preload_hooks)：检查是否有需要预加载的 hook
        \- ngx_http_lua_probe_register_preload_package：注册第 3 方模块的预加载 hook
            \- 示例：package=ngx.upstream，loader=ngx_http_lua_upstream_create_module
        \- lua_pushcfunction
        \- lua_setfield
    \- lua_pcall：调用 require("resty.core")，不知何意
    \- ngx_http_lua_inject_global_write_guard：注入全局写保护，使用全局变量时会得到告警提示
        \- 执行了一段 Lua 代码，设置了 _G 的元表，重载了 __newindex
```
