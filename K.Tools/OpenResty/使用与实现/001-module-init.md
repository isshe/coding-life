# lua-nginx-module 模块的定义及初始化

## 实现

lua_nginx_module 其实就是一个 Nginx 模块，定义及实现都需要安装 Nginx 模块的要求。

- 定义个全局的模块变量：

> ngx_http_lua_module.c
> 此结构的详细解释可见 Nginx 源码分析系列文章的 《Nginx 模块》


```
ngx_module_t ngx_http_lua_module = {
    NGX_MODULE_V1,
    &ngx_http_lua_module_ctx,   /*  module context */
    ngx_http_lua_cmds,          /*  module directives */
    NGX_HTTP_MODULE,            /*  module type */
    NULL,                       /*  init master */
    NULL,                       /*  init module */
    ngx_http_lua_init_worker,   /*  init process */
    NULL,                       /*  init thread */
    NULL,                       /*  exit thread */
    ngx_http_lua_exit_worker,   /*  exit process */
    NULL,                       /*  exit master */
    NGX_MODULE_V1_PADDING
};
```

- 定义一个模块上下文

```
static ngx_http_module_t ngx_http_lua_module_ctx = {
    NULL,                             /*  preconfiguration */
    ngx_http_lua_init,                /*  postconfiguration */

    ngx_http_lua_create_main_conf,    /*  create main configuration */
    ngx_http_lua_init_main_conf,      /*  init main configuration */

    ngx_http_lua_create_srv_conf,     /*  create server configuration */
    ngx_http_lua_merge_srv_conf,      /*  merge server configuration */

    ngx_http_lua_create_loc_conf,     /*  create location configuration */
    ngx_http_lua_merge_loc_conf       /*  merge location configuration */
};
```

- 定义指令

```
static ngx_command_t ngx_http_lua_cmds[] = {

    { ngx_string("lua_load_resty_core"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_FLAG,
      ngx_http_lua_load_resty_core,
      NGX_HTTP_MAIN_CONF_OFFSET,
      0,
      NULL },

    ...
}
```

