lua-nginx-module 模块的定义及初始化
---

lua_nginx_module 其实就是一个 Nginx 模块，定义及实现都需要按照 Nginx 模块的要求。

这篇文章的目的是什么呢？

- 了解 lua-nginx-module 模块相关数据结构的定义。
- 了解 lua-nginx-module 模块大致的初始化流程，对 lua-nginx-module 模块有一个总体的概念，然后我们在后续的文章中，去细化追究相关指令的细节。
    - 总分结构

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
- 解析到相关配置指令（ngx_http_lua_cmds 数组中定义，如 init_by_lua），就执行指令的处理函数。
  - xxx_by_lua 指令的具体处理见后续的文章。
- 调用 ngx_http_lua_init 这个 postconfiguration 函数（详见上文的 “模块上下文 ngx_http_lua_module_ctx”）
  - init_by_lua 设置的 handler 在 postconfiguration 。

我们来看下 ngx_http_lua_init 这个函数做了什么。

### ngx_http_lua_init 执行流程

```
- ngx_http_lua_init
    \- ngx_array_push：判断是否需要介入 rewrite、access、log 阶段的处理，如果需要，就设置对应的 handler。
        \- 如：ngx_array_push(&cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers);
    \- ngx_http_lua_header_filter_init：如果需要介入 header_fitler 阶段，则会调用此函数把相关处理函数设置到调用链中。
    \- ngx_http_lua_body_filter_init：如果需要介入 body_fitler 阶段，则会调用此函数把相关处理函数设置到调用链中。
    \- ngx_pool_cleanup_add：添加内存池清理函数
    \- ngx_http_lua_pipe_init：初始化一颗红黑树，用于管道处理，见 [pipe](./015-pipe.md)。
    \- ngx_http_lua_init_vm：如果没有初始化 Lua VM，则初始化。lua_State 设置在 lmcf->lua 中。
    \- ngx_cycle_set_post_init：设置 handler（ngx_http_lua_post_init_handler），此 handler 将在 ngx_cycle_post_init 中被调用。
        \- cycle->post_init_handlers[module.index] = post_init: post_init 即是 ngx_http_lua_post_init_handler。

...

- ngx_cycle_post_init：在 main 函数中调用
    \- post_init(ngx_http_lua_post_init_handler)
        \- init_handler(ngx_http_lua_init_by_inline)
```

可以看到，配置解析完后（postconfiguration），也会设置 handler 为 ngx_http_lua_post_init_handler，但没有实际调用。
后续对于 ngx_http_lua_post_init_handler 的调用在 ngx_cycle_post_init 中，这是为什么呢？
- 为什么在 postconfiguration 时相关 block 配置已经解析完了，为什么还不能调用 ngx_http_lua_post_init_handler，而是放到 ngx_http_lua_post_init_handler？[1]
    - 猜测是缺了什么。继续探索，看后续是否有答案。
    - FIXME
- 相关 Lua 代码（如 init_by_lua 指令设置的）会在哪里执行呢？[2]

## 进程初始化

从前面的模块定义可以看到 OpenResty 只指定了 ngx_http_lua_init_worker 和 ngx_http_lua_exit_worker 两个 worker 进程的处理函数。
配置解析完成后，会 fork 出子进程，然后调用 ngx_http_lua_init_worker 函数，我们来具体看看。

```c
- ngx_http_lua_init_worker
    \- ngx_http_cycle_get_module_main_conf：获取主配置
    \- ngx_http_lua_pipe_add_signal_handler：添加信号处理函数，pipe 见 [pipe](./015-pipe.md)
    \- init_worker_handler：执行对应的 Lua 代码
```

init_worker_handler 从哪里来的呢？是 init_worker_by_lua 指令设置的，这个我们在后续的文章中再去验证一下。
如果是 init_worker_by_lua 指令的 Lua 代码是在 init_worker_handler 中执行；
可想而知，init_by_lua 指令设置的代码，大概率在 init_handler 中执行了。那这也就是问题 [2] 的答案了，我们也在后续的文章中再去验证。

> 这个函数中还伪造了 cycle 、连接、请求等很多东西，是为什么呢？有何作用？
目前了解到是所有 Lua 代码都要依赖请求（request），因此需要进行相关的 fake。
