
Nginx 模块
---

Nginx 的大多数功能都是作为模块实现的，模块是 Nginx 的构建单元。

每个 Nginx 都在一个**独立**的目录中，**至少**包含两个文件：config 文件及源码文件。
在编译 Nginx 执行 configure 时，使用 `--add-module=/path/to/module`，即可把模块静态编译进 Nginx 中；使用 `--add-dynamic-module=/path/to/module` 把模块编译成动态模块。

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

- ngx_module_type: 定义模块的类型，取值有 CORE, HTTP, HTTP_FILTER, HTTP_INIT_FILTER, HTTP_AUX_FILTER, MAIL, STREAM, MISC。
    - HTTP_AUX_FILTER: 第三方过滤模块。
    - HTTP_AUX_FILTER 和 HTTP_FILTER 有什么区别？
    - 对于过滤模块，更早地初始化意味着更晚地执行。

- ngx_module_srcs: 模块源码文件列表，以空格分隔。

# 模块定义

模块中必须包含一个 ngx_module_t 类型的全局变量。

```
struct ngx_module_s {
    /* private part is omitted */
    void                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;
    ngx_int_t           (*init_master)(ngx_log_t *log);
    ngx_int_t           (*init_module)(ngx_cycle_t *cycle);
    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle);
    void                (*exit_thread)(ngx_cycle_t *cycle);
    void                (*exit_process)(ngx_cycle_t *cycle);
    void                (*exit_master)(ngx_cycle_t *cycle);
    /* stubs for future extensions are omitted */
};
```

> 此结构的详细解释可见 Nginx 源码分析系列文章的《Nginx 模块》

# 模块上下文定义

定义一个模块上下文，会在 `ngx_module_t` 引用：

```c
// 有 NGX_CORE_MODULE、NGX_HTTP_MODULE 等，下面是 NGX_CORE_MODULE：
typedef struct {
    ngx_str_t             name;
    void               *(*create_conf)(ngx_cycle_t *cycle);
    char               *(*init_conf)(ngx_cycle_t *cycle, void *conf);
} ngx_core_module_t;

// 以下是具体变量值，以 NGX_HTTP_MODULE 类型示例：
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
