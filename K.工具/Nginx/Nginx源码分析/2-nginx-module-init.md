Nginx 模块初始化
---

Nginx 都是以模块的形式进行组织的，无论是 Nginx 核心还是第三方模块。

我们按照 Nginx 的约定，可以比较方便地定义模块，详见：http://nginx.org/en/docs/dev/development_guide.html#Modules

不过，现在我们想了解的不是 Nginx 模块的定义/使用，而是 Nginx 本身的模块功能是如何实现的、包含了哪些内容，后续的系列文章将围绕这个目的展开。

而这篇文章的目的是解答以下问题：

- Nginx 配置是如何解析的？
- Nginx 模块是如何初始化的？

# Nginx 模块结构

以下是涉及的模块相关的数据结构：（不完全，仅后续 “Nginx 模块初始化调用过程” 中涉及的）

- [ngx_module_t](../Nginx的数据结构/ngx_module_t.md)
- [ngx_command_t](../Nginx的数据结构/ngx_command_t.md)
- [ngx_core_module_t](../Nginx的数据结构/ngx_core_module_t.md)
- [ngx_event_module_t](../Nginx的数据结构/ngx_event_module_t.md)
- [ngx_http_module_t](../Nginx的数据结构/ngx_http_module_t.md)
- [ngx_stream_module_t](../Nginx的数据结构/ngx_stream_module_t.md)
- [ngx_mail_module_t](../Nginx的数据结构/ngx_mail_module_t.md)


# Nginx 模块变量

以下是涉及到的模块变量。

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

# Nginx 模块初始化执行流程

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
             \ ngx_conf_handler: core/ngx_conf_file.c,
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
                     \ ngx_conf_parse: continue parsing the configuration in the http{}/stream{} block. (!!!)
                        |
                         \ ngx_http_core_server
                            \ ngx_conf_parse
                               \ ngx_http_core_listen
                               \ ngx_http_core_location
                                  \ ngx_conf_parse
                                     \ ...
                               \ ...
                     \ init_main_conf
                     \ ngx_http_merge_servers: http module only
                        |
                         \ merge_srv_conf
                         \ merge_loc_conf
                     \ merge_srv_conf: stream module only
                     \ postconfiguration：调用 postconfiguration 函数，到这里整个 http block 内的配置指令已经解析完了
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
     \ ngx_cycle_post_init
    |
     \ ngx_single_process_cycle: core/nginx.c
        |
         \ init_process: defined by module
         \ exit_process: defined by module
         \ ngx_master_process_exit
            |
             \ exit_master
```

- 配置解析时，每获取完一项配置，就遍历模块，比较配置指令名称，匹配后调用相应的 cmd->set 函数。
  - 以 HTTP 模块示例：
    - 解析到 http 指令时，执行 ngx_http_block 函数，函数内部继续调用 ngx_conf_parse 对 `http { ... }` 块内的配置进行解析；
    - 解析到 server 指令时，执行 ngx_http_core_server 函数，函数内部继续调用 ngx_conf_parse 对 ` server { ... }` 块内的配置进行解析；
    - 解析到 listen 指令时，执行 ngx_http_core_listen 函数进行端口监听(ngx_http_add_listen)等动作。
- cmd->set：处理**指令**并将解析后的值存储到相应的配置中。
    - 在 NGX_CORE_MODULE 类型的模块 “http” 的 cmd->set (也就是 ngx_http_block) 中，逐个处理 NGX_HTTP_MODULE 类型的模块的上下文；NGX_HTTP_MODULE 的 cmd 还是和其他模块的一起处理了。
    - 因为有顺序，因此是 “http” 模块的指令（cmd）处理完以后，再处理其他 NGX_HTTP_MODULE 类型模块的指令。
- 配置解析完以后，进行模块初始化 ngx_init_modules。

接下来对其中的关键调用过程进行解析。

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

以上是关于模块初始化、配置解析的调用过程。
一句话总结：程序启动后会进行一系列初始化，并调用 ngx_conf_parse 来解析配置文件，遇到配置指令就执行对应指令的 set 回调，逐级深入。
