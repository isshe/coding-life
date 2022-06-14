# lua-nginx-module 模块的定义及初始化

## 实现

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
