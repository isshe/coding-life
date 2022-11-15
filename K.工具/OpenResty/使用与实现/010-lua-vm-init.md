
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

## Lua VM 初始化流程

```

```
