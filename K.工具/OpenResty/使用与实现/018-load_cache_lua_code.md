
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
        \- lua_load(L, ngx_http_lua_clfactory_getS, &ls, name): 加载 Lua Chunk 成一个函数。这个函数其实是一个闭包 "return function() ... end"，其中 "return function" 和 "end" 是 ngx_http_lua_clfactory_getS 函数固定添加的
            \- ngx_http_lua_clfactory_getS: 这个 lua_load 的 reader 参数，用于读取 lua chunk。
    \- ngx_http_lua_cache_store_code: 把闭包工厂存到 cache 中，并返回一个闭包。
        \- lua_rawget(L, LUA_REGISTRYINDEX): 获取注册表
        \- lua_setfield(L, -2, key): 把 key 和闭包工厂设置到 cache 注册表中，或者是把 key、ref、闭包工厂设置到 cache 注册表中。
        \- rc = lua_pcall(L, 0, 1, 0): 生成一个闭包放到栈顶
```

最终结果：加载 Lua 代码到栈顶。

此函数的主要过程：

- 查找缓存表中保存的闭包工厂，有就用这个闭包工厂生产一个闭包。
- 没有找到，则从代码块加载，固定加生成 "return function() ... end" 这样的代码块，加载成闭包工厂。
- 缓存得到的闭包工厂（不是具体的闭包），然后调用闭包工厂，返回一个闭包。（每次调用闭包工厂都会返回一个闭包）

这也就是 Lua 代码加载及缓存的过程。
