Nginx 模块
---

Nginx 基本上都是以模块的形式进行组织的，无论是 Nginx 核心还是第三方模块。
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

## ngx_preinit_modules

> src/core/nginx.c

```c
    if (ngx_preinit_modules() != NGX_OK) {
        return 1;
    }
```

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

预初始化阶段，对所有模块进行编号，并填充名字——从全局变量 ngx_modules 指向全局变量 ngx_module_names。

## ngx_cycle_modules

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

 从当前 cycle 的内存池中分配一块内存，把全局变量 ngx_modules 中记录的模块信息复制一份过来，用于当前 cycle，使用 cycle->modules 指针指向。
