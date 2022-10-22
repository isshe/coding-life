lua-nginx-module 模块的定义及初始化
---

lua_nginx_module 其实就是一个 Nginx 模块，定义及实现都需要按照 Nginx 模块的要求。

# 定义

- 定义个全局的模块变量：

> ngx_http_lua_module.c
> 此结构的详细解释可见 Nginx 数据结构 [ngx_module_t](../../Nginx/Nginx的数据结构/ngx_module_t.md)

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

可以看到被定义成了一个 HTTP 模块（NGX_HTTP_MODULE, ngx_http_module_t）。

main conf 见：[ngx_http_lua_main_conf_t](../OpenResty的数据结构/ngx_http_lua_main_conf_t.md)
srv_conf 见：[ngx_http_lua_srv_conf_t](../OpenResty的数据结构/ngx_http_lua_srv_conf_t.md)
loc_conf 见：[ngx_http_lua_loc_conf_t](../OpenResty的数据结构/ngx_http_lua_loc_conf_t.md)

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


# 初始化

## 配置初始化

Nginx 模块的初始化流程见：[Nginx 模块初始化](../../Nginx/Nginx源码分析/2-nginx-module-init.md)

简而言之就是：
- 解析到相关配置指令（ngx_http_lua_cmds 数组中定义，init_by_lua），就执行指令的处理函数。
  - xxx_by_lua 等指令通常是设置对应的 handler，然后在各个阶段再去执行对应的 Lua 代码，而不是立即执行这些 Lua 代码。
  - xxx_by_lua 指令的具体处理见后续的文章。
- 调用 ngx_http_lua_init 这个 postconfiguration 函数
  - init_by_lua 设置的 handler 在 postconfiguration 。

我们来看下 ngx_http_lua_init 这个函数做了什么。

### ngx_http_lua_init 调用流程

```
- ngx_http_lua_init
    \- ngx_array_push：判断是否需要介入 rewrite、access、log 阶段的处理，如果需要，就设置对应的 handler。
    \- ngx_http_lua_header_filter_init：如果需要介入 header_fitler 阶段，则会调用此函数把相关处理函数设置到调用链中。
    \- ngx_http_lua_body_filter_init：如果需要介入 body_fitler 阶段，则会调用此函数把相关处理函数设置到调用链中。
    \- ngx_pool_cleanup_add：添加内存池清理函数
    \- ngx_http_lua_pipe_init：初始化一颗红黑树，用于管道处理，见 [pipe](./015-pipe.md)。
    \- ngx_http_lua_init_vm：如果没有初始化 Lua VM，则初始化。lua_State 设置在 lmcf->lua 中。
    \- ngx_cycle_set_post_init：设置 handler（ngx_http_lua_post_init_handler），此 handler 将在 ngx_cycle_post_init 中被调用。

...

- ngx_cycle_post_init：在 main 函数中调用
    \- ngx_http_lua_post_init_handler
        \- ngx_http_lua_init_by_inline：调用 Lua 代码
```

可以看到，ngx_http_lua_post_init_handler 中会调用 init_by_lua 指令设置的 handler（ngx_http_lua_init_by_inline），此 handler 中再执行 Lua 代码。

## 进程初始化

从前面的模块定义可以看到 OpenResty 只指定了 ngx_http_lua_init_worker 和 ngx_http_lua_exit_worker 两个 worker 进程的处理函数。
配置解析完成后，接下来会 fork 出子进程，然后就会调用 ngx_http_lua_init_worker 函数。
用头发想都能知道，init_worker_by_lua 的代码将会在这里被执行。我们来具体看看。

```c
- init_worker_by_lua
```
