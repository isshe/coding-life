
Nginx 模块开发
---

每个 Nginx 都在一个独立的目录中，至少包含两个文件：config 文件及源码文件。

# config 文件定义

config 文件实际上就是一个 POSIX shell 脚本。
示例：

```
ngx_module_type=CORE
ngx_module_name=ngx_foo_module
ngx_module_srcs="$ngx_addon_dir/ngx_foo_module.c"

. auto/module

ngx_addon_name=$ngx_module_name
```

- ngx_module_type: 定义模块的类型，取值有：CORE, HTTP, HTTP_FILTER, HTTP_INIT_FILTER, HTTP_AUX_FILTER, MAIL, STREAM, MISC。
    - HTTP_AUX_FILTER: 第三方过滤模块。
    - HTTP_AUX_FILTER 和 HTTP_FILTER 有什么区别？

# 模块定义

```c
ngx_http_module_t ngx_http_lua_module_ctx = {
    NULL,                             /*  preconfiguration */
    ngx_http_lua_init,                /*  postconfiguration */

    ngx_http_lua_create_main_conf,    /*  create main configuration */
    ngx_http_lua_init_main_conf,      /*  init main configuration */

    NULL,                             /*  create server configuration */
    NULL,                             /*  merge server configuration */

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
    NULL,                       /*  exit process */
    NULL,                       /*  exit master */
    NGX_MODULE_V1_PADDING
};
```

# 指令定义

```
typedef struct ngx_command_s     ngx_command_t;
struct ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    void                 *post;
};
```

# 参考

- http://nginx.org/en/docs/dev/development_guide.html#Modules
- https://www.nginx.com/resources/wiki/extending/new_config/
