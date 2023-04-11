
# OpenResty 加载及缓存 Lua 代码

目的：

- 了解 OpenResty 是如何加载及缓存 Lua 代码的。

加载 Lua 代码之前的代码逻辑见 [005-rewrite_by_lua.md](005-rewrite_by_lua.md#ngx_http_lua_rewrite_handler执行流程)，本文接着从 `ngx_http_lua_cache_loadbuffer` 开始深入。


## ngx_http_lua_cache_loadbuffer

```lua
- ngx_http_lua_cache_loadbuffer: 加载 Lua 代码
    \- ngx_http_lua_cache_load_code: 从缓存中加载，有直接返回，没则继续。
    \- ngx_http_lua_clfactory_loadbuffer: 加载闭包工厂
    \- ngx_http_lua_cache_store_code: 存到 cache 中
```
