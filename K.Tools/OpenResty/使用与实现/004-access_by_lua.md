Lua 代码的加载与运行
---

以 access_by_lua_file/access_by_lua_block/access_by_lua 为例。

# 定义 Nginx 配置命令

> ngx_http_lua_module.c

```c
    { ngx_string("access_by_lua_file"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_TAKE1,
      ngx_http_lua_access_by_lua,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      (void *) ngx_http_lua_access_handler_file },
```

- 配置解析：ngx_http_lua_access_by_lua
- 执行 Lua 代码：ngx_http_lua_access_handler_file
