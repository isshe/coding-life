
# OpenResty 加载及缓存 Lua 代码

目的：

- 了解 OpenResty 是如何加载及缓存 Lua 代码的。

加载 Lua 代码之前的代码逻辑见 [005-rewrite_by_lua.md](005-rewrite_by_lua.md#ngx_http_lua_rewrite_handler执行流程)，本文接着从 `ngx_http_lua_cache_loadbuffer` 开始深入。


## ngx_http_lua_cache_loadbuffer

```lua
- ngx_http_lua_cache_loadbuffer: 加载 Lua 代码
    \- ngx_http_lua_cache_load_code: 从缓存中加载，有直接返回，没有则继续。
        \- lua_pushlightuserdata(L, ngx_http_lua_lightudata_mask(code_cache_key))
        \- lua_rawget(L, LUA_REGISTRYINDEX): 把 Lua 代码缓存表加载到栈顶
        \- if (*ref == LUA_NOREF): 通过 ref 和 key 结合来获取缓存。如果有 ref，就用 ref，如果没有，就用 key。如果用 key 也没找到，就是没缓存，后续需要加载。
        \- lua_isfunction: 这是闭包工厂函数，通过此函数来生成闭包
        \- rc = lua_pcall(L, 0, 1, 0): 正式调用闭包工厂，生成闭包，返回值是 0 表示成功，否则就是失败了。
    \- ngx_http_lua_clfactory_loadbuffer: 加载闭包工厂
    \- ngx_http_lua_cache_store_code: 存到 cache 中
```

加载 Lua 代码到栈顶。
