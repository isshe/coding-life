Nginx 模块
---

Nginx 都是以模块的形式进行组织的，无论是 Nginx 核心还是第三方模块。

我们按照 Nginx 的约定，可以比较方便地定义模块，详见：http://nginx.org/en/docs/dev/development_guide.html#Modules

不过，这里我们想了解的不是 Nginx 模块的定义，而是 Nginx 本身的模块功能是如何实现的，包含了哪些内容。


# Nginx 模块结构

## struct ngx_module_s (ngx_module_t)

> src/core/ngx_module.h

```c
struct ngx_module_s {
    ngx_uint_t            ctx_index;
    ngx_uint_t            index;

    char                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            version;
    const char           *signature;

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

    uintptr_t             spare_hook0;
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};
```
> 设置实例可以见 OpenResty 系列文章中的《lua-nginx-module 模块的定义及初始化》。

- signature 及之前的字段算是私有部分，包含名称、索引、版本、签名等，可以直接使用预定义的宏 NGX_MODULE_V1 来填充。
- ctx: 存储模块的私有数据。
- commands: 定义模块的配置指令，可以在指定阶段进行配置/调用。
- type: 表明在 ctx 字段中存储的是什么数据。取值
    - NGX_CORE_MODULE: 最基础、最通用、最底层的模块类型，其他模块类型都是在它之上实现的。像 ngx_core_module、ngx_errlog_module、ngx_regex_module、ngx_thread_pool_module、ngx_openssl_module 都是核心模块。
    - NGX_EVENT_MODULE
    - NGX_HTTP_MODULE
    - NGX_MAIL_MODULE
    - NGX_STREAM_MODULE
- init_master：没有使用。
- init_module：初始化模块，配置解析成功后，调用这个函数。
- init_process：master 进程创建一个或多个工作进程，并在每个进程调用此函数。
- spare_hookx：编号 0-7，共 8 个备用钩子。
- init_thread：没有使用。
- exit_thread：没有使用。
- exit_process：当 worker 进程接收到 master 的退出或终止信号时，调用此函数。
- exit_master：master 进程退出前，调用此函数。


## struct ngx_command_s (ngx_command_t)

> src/core/ngx_conf_file.h

```c
struct ngx_command_s {
    ngx_str_t             name;
    ngx_uint_t            type;
    char               *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
    ngx_uint_t            conf;
    ngx_uint_t            offset;
    void                 *post;
};
```

定义 1 个配置指令。

- name：在配置文件中使用的指令名称，如 listen。
- type：按位的标志，指定指令接受的参数数量、类型和出现的上下文。
- set：定义一个处理程序来处理指令并将解析后的值存储到相应的配置中。
- conf：定义传递给目录处理程序的配置结构。(?)
    - 核心模块只有全局配置，并通过设置 NGX_DIRECT_CONF 标志来访问它。
    - HTTP、Stream 或 Mail 这样的模块创建了配置的层次结构。
- offset：定义模块配置结构中保存此指令值的字段的偏移量。
- post：用于向主处理程序传递数据或者定义主处理程序完成后调用的处理程序。

## ngx_core_module_t

> src/core/ngx_module.h

```c
typedef struct {
    ngx_str_t             name;
    void               *(*create_conf)(ngx_cycle_t *cycle);
    char               *(*init_conf)(ngx_cycle_t *cycle, void *conf);
} ngx_core_module_t;
```

## ngx_event_module_t

> src/event/ngx_event.h

```c
typedef struct {
    ngx_str_t              *name;

    void                 *(*create_conf)(ngx_cycle_t *cycle);
    char                 *(*init_conf)(ngx_cycle_t *cycle, void *conf);

    ngx_event_actions_t     actions;
} ngx_event_module_t;
```

## ngx_http_module_t

> src/http/ngx_http_config.h

```c
typedef struct {
    ngx_int_t   (*preconfiguration)(ngx_conf_t *cf);
    ngx_int_t   (*postconfiguration)(ngx_conf_t *cf);

    void       *(*create_main_conf)(ngx_conf_t *cf);
    char       *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void       *(*create_srv_conf)(ngx_conf_t *cf);
    char       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev, void *conf);

    void       *(*create_loc_conf)(ngx_conf_t *cf);
    char       *(*merge_loc_conf)(ngx_conf_t *cf, void *prev, void *conf);
} ngx_http_module_t;
```

## ngx_stream_module_t

> src/http/ngx_stream.h

```c
typedef struct {
    ngx_int_t                    (*preconfiguration)(ngx_conf_t *cf);
    ngx_int_t                    (*postconfiguration)(ngx_conf_t *cf);

    void                        *(*create_main_conf)(ngx_conf_t *cf);
    char                        *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void                        *(*create_srv_conf)(ngx_conf_t *cf);
    char                        *(*merge_srv_conf)(ngx_conf_t *cf, void *prev,
                                                   void *conf);
} ngx_stream_module_t;
```

## ngx_mail_module_t

> src/http/ngx_mail.h

```c
typedef struct {
    ngx_mail_protocol_t        *protocol;

    void                       *(*create_main_conf)(ngx_conf_t *cf);
    char                       *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void                       *(*create_srv_conf)(ngx_conf_t *cf);
    char                       *(*merge_srv_conf)(ngx_conf_t *cf, void *prev,
                                                  void *conf);
} ngx_mail_module_t;
```


# Nginx 模块变量

## ngx_modules

> objs/ngx_modules.c：此文件在执行 ./auto/configure 后生成。

```c
extern ngx_module_t  ngx_core_module;

ngx_module_t *ngx_modules[] = {
    &ngx_core_module,
    ...
}
```

## ngx_module_names

> objs/ngx_modules.c：此文件在执行 ./auto/configure 后生成。

```c
char *ngx_module_names[] = {
    "ngx_core_module",
    ...
}
```

# Nginx 模块相关函数及调用过程

```
main: core/nginx.c
  |
   \ ngx_preinit_modules: core/ngx_module.c, populate global variable "ngx_modules"
  |
   \ ngx_init_cycle: core/ngx_cycle.c,
        |
         \ ngx_cycle_modules: core/ngx_module.c, copy "ngx_modules" to cycle
        |
         \ create_conf: prioritize core modules
        |
         \ ngx_conf_parse: core/ngx_conf_file.c
            |
             \ ngx_conf_handler: core/ngx_conf_file.c
                |
                 \ cmd->set: defined by module, processes a directive and stores parsed values into the corresponding configuration
                    - such as:
                    - http/ngx_http.c, stream/ngx_stream.c, event/ngx_event.c, mail/ngx_mail.c
                    - ngx_http_block, ngx_stream_block, ngx_events_block, ngx_mail_block
                    |
                    | (http or stream module)
                     \ create_main_conf
                     \ create_srv_conf
                     \ create_loc_conf: http module only
                     \ preconfiguration
                     \ init_main_conf
                     \ ngx_http_merge_servers: http module only
                        |
                         \ merge_srv_conf
                         \ merge_loc_conf
                     \ merge_srv_conf: stream module only
                     \ postconfiguration
                    |
                    | (event module)
                     \ create_conf
                     \ init_conf
                    |
                    | (mail module)
                     \ create_main_conf
                     \ create_srv_conf
                     \ init_main_conf
                     \ merge_srv_conf
        |
         \ init_conf: core modules
        |
         \ ngx_init_modules: core/ngx_module.c
            |
             \ init_module: defined by module
    |
     \ ngx_single_process_cycle: core/nginx.c
        |
         \ init_process: defined by module
         \ exit_process: defined by module
         \ ngx_master_process_exit
            |
             \ exit_master
```

- cmd->set：处理**指令**并将解析后的值存储到相应的配置中。
    - 在 NGX_CORE_MODULE 类型的模块 “http” 的 cmd->set (也就是 ngx_http_block) 中，逐个处理 NGX_HTTP_MODULE 类型的模块的上下文；NGX_HTTP_MODULE 的 cmd 还是和其他模块的一起处理了。
    - 因为有顺序，因此是 “http” 模块的指令（cmd）处理完以后，再处理其他 NGX_HTTP_MODULE 类型模块的指令。
- 配置解析完以后，进行模块初始化 ngx_init_modules。


## ngx_preinit_modules

调用：

> src/core/nginx.c

```c
    if (ngx_preinit_modules() != NGX_OK) {
        return 1;
    }
```

定义：

> src/core/ngx_module.c

```c
ngx_int_t
ngx_preinit_modules(void)
{
    ngx_uint_t  i;

    for (i = 0; ngx_modules[i]; i++) {
        ngx_modules[i]->index = i;
        ngx_modules[i]->name = ngx_module_names[i];
    }

    ngx_modules_n = i;
    ngx_max_module = ngx_modules_n + NGX_MAX_DYNAMIC_MODULES;

    return NGX_OK;
}
```

此函数在上一篇的入口函数中调用，预初始化阶段，对所有模块进行编号，并填充名字——从全局变量 ngx_modules 指向全局变量 ngx_module_names。

## ngx_cycle_modules

调用：

> src/core/ngx_cycle.c

```c
ngx_cycle_t *
ngx_init_cycle(ngx_cycle_t *old_cycle)
{
    // ...

    if (ngx_cycle_modules(cycle) != NGX_OK) {
        ngx_destroy_pool(pool);
        return NULL;
    }

    // ...
}
```

定义：

> src/core/ngx_module.c

```c
ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * sizeof(ngx_module_t *));
    if (cycle->modules == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * sizeof(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    return NGX_OK;
}
```

在前面 ngx_preinit_modules() 时，已经填充了 ngx_modules。
在这里就从当前 cycle 的内存池中分配一块内存，把前面填充到全局变量 ngx_modules 中的模块信息复制一份，用于当前 cycle，使用 cycle->modules 指针指向。

注：每次 nginx reload 时都会新建一个 cycle，销毁旧 cycle。详见：http://nginx.org/en/docs/dev/development_guide.html#cycle


## cmd->set

原型：

```c
char *(*set)(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
```

在前面把模块信息复制到 cycle 后，便开始配置解析，配置解析时逐个调用各个模块，然后对其支持的指令进行解析存储。

```c
static ngx_int_t
ngx_conf_handler(ngx_conf_t *cf, ngx_int_t last)
{
    ...

            rv = cmd->set(cf, cmd, conf);

            if (rv == NGX_CONF_OK) {
                return NGX_OK;
            }

    ...
}
```

以上是关于模块初始化、配置解析部分。
这些步骤完成以后，是如何知道什么时候跑什么模块的呢？我们在接下来的文章中继续跟踪。
