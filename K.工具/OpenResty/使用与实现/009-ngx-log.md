# ngx.log

我们都知道，这个接口的功能是打印**错误日志**，那么它是如何工作的呢？我们来探究一下。

**注意，是接口 ngx.log 不是指令 log_by_lua* 。**

目的：

- 它是如何工作的呢？
  - ngx.log 是如何被定义的？
  - ngx.log 执行时，做了什么？

## 使用

- 上下文：context: init_by_lua*, init_worker_by_lua*, set_by_lua*, rewrite_by_lua*, access_by_lua*, content_by_lua*, header_filter_by_lua*, body_filter_by_lua*, log_by_lua*, ngx.timer.*, balancer_by_lua*, ssl_certificate_by_lua*, ssl_session_fetch_by_lua*, ssl_session_store_by_lua*, exit_worker_by_lua*, ssl_client_hello_by_lua*

- 语法：`ngx.log(log_level, ...)`
- 注意：
  - Lua `nil` 会被输出为字符串 "nil"；Lua 的布尔类型会输出为 "true" or "false"；`ngx.null` 被输出为 "null" 字符串；Lua table 将会调用其元表的 __tostring 函数，调用失败则抛出异常，异常也被输出到错误日志中；如果是 userdata，则输出字符串 "null"；其他类型一律抛出异常。
  - Nginx 内核中的错误消息长度有一个硬编码的 2048 字节限制。此限制包括尾部换行符和前置时间戳。如果消息大小超过此限制，Nginx 将相应地截断消息。可以通过编辑 Nginx 源代码树中 src/core/ngx_log.h 文件中的 NGX_MAX_ERROR_STR 宏定义来手动修改此限制。

## 实现

- 我的第一想法还是从使用入手，我们使用的是 Lua 接口 ngx.log。
根据此接口，找到 lib/ngx/errlog.lua 文件，不过很遗憾，这个并不是我们要找的文件。接下来怎么继续呢？

- 考虑到最终会调用 write 接口来写文件，我们可以尝试使用 GDB 来跟踪 Nginx write 相关的接口。不过也有可能调用 ngx.log 时，并不会立即写文件。不确定，但是可以试试。 （Nginx 处理 access log 时，通常会进行缓存；并且写文件是一个耗时的操作，每个请求都写一下也不合理）
通过使用 GDB 对 ngx_write_file 进行跟踪，发现并没有被调用。

- 想到 ngx.log 最终是调用了 nginx 核心的写日志函数，因此我们可以跟一下 nginx 记录日志相关的函数，从 ngx_log.c 中 找到 `ngx_log_error_core`，进行跟踪，得到：

```lua
#0  ngx_log_error_core (level=4, log=0x0, err=0, fmt=0x56030bc04f83 "%s%*s") at src/core/ngx_log.c:106
#1  0x000056030bb20886 in log_wrapper (log=0x56030bd759f0, ident=0x56030bc04ed9 "[lua] ", level=4,
    L=0x7f4f86397f50) at ../ngx_lua-0.10.21/src/ngx_http_lua_log.c:273
#2  0x000056030bb1ff37 in ngx_http_lua_ngx_log (L=0x7f4f86397f50)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_log.c:60
#3  0x00007f4f86b81b83 in lj_BC_FUNCC ()
   from /root/persional/openresty-bin/luajit/lib/libluajit-5.1.so.2
#4  0x000056030bb3e35b in ngx_http_lua_run_thread (L=0x7f4f86387380, r=0x56030bdfb300,
    ctx=0x56030bdfc048, nrets=0) at ../ngx_lua-0.10.21/src/ngx_http_lua_util.c:1185
#5  0x000056030bb46b92 in ngx_http_lua_rewrite_by_chunk (L=0x7f4f86387380, r=0x56030bdfb300)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_rewriteby.c:337
#6  0x000056030bb467ae in ngx_http_lua_rewrite_handler_inline (r=0x56030bdfb300)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_rewriteby.c:190
#7  0x000056030bb466e7 in ngx_http_lua_rewrite_handler (r=0x56030bdfb300)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_rewriteby.c:162
...
```

在 rewrite_by_lua 指令中使用的 ngx.log，因此调用栈下面部分是关于此指令的。
这就是 ngx.log 写日志的过程，不过 ngx.log 是如何与 ngx_http_lua_ngx_log 对应起来的呢？也就是使用 ngx.log 时，是如何知道要去调用 ngx_http_lua_ngx_log 的呢？通过搜索源码，得到：

```c
void
ngx_http_lua_inject_log_api(lua_State *L)
{
    ngx_http_lua_inject_log_consts(L);

    lua_pushcfunction(L, ngx_http_lua_ngx_log);
    lua_setfield(L, -2, "log");

    lua_pushcfunction(L, ngx_http_lua_print);
    lua_setglobal(L, "print");
}
```

看起来是在这里进行了关联，使用 GDB 跟踪一下 ngx_http_lua_inject_log_api，
在 worker 进程进行跟踪，发现并没有被调用，跟踪 master 进程得到：

```lua
#0  ngx_http_lua_inject_log_api (L=0x7ffff7717380)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_log.c:281
#1  0x0000555555705855 in ngx_http_lua_inject_ngx_api (L=0x7ffff7717380,
    lmcf=0x555555891080, log=0x55555583d280 <ngx_log>)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_util.c:840
#2  0x00005555557057c4 in ngx_http_lua_init_globals (L=0x7ffff7717380,
    cycle=0x555555885740, lmcf=0x555555891080,
    log=0x55555583d280 <ngx_log>)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_util.c:822
#3  0x0000555555704647 in ngx_http_lua_new_state (parent_vm=0x0,
    cycle=0x555555885740, lmcf=0x555555891080,
    log=0x55555583d280 <ngx_log>)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_util.c:326
#4  0x000055555570b955 in ngx_http_lua_init_vm (new_vm=0x555555891080,
    parent_vm=0x0, cycle=0x555555885740, pool=0x5555558856f0,
    lmcf=0x555555891080, log=0x55555583d280 <ngx_log>, pcln=0x0)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_util.c:3896
#5  0x00005555556fb450 in ngx_http_lua_init (cf=0x7fffffffd7e0)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_module.c:865
#6  0x00005555555f4ddc in ngx_http_block (cf=0x7fffffffd7e0,
    cmd=0x55555581a200 <ngx_http_commands>, conf=0x555555886a20)
    at src/http/ngx_http.c:310
#7  0x00005555555b9ec7 in ngx_conf_handler (cf=0x7fffffffd7e0, last=1)
    at src/core/ngx_conf_file.c:463
#8  0x00005555555b99cc in ngx_conf_parse (cf=0x7fffffffd7e0,
    filename=0x555555885958) at src/core/ngx_conf_file.c:319
#9  0x00005555555b4f87 in ngx_init_cycle (old_cycle=0x7fffffffd9b0)
    at src/core/ngx_cycle.c:284
#10 0x0000555555592fe8 in main (argc=3, argv=0x7fffffffdd58)
    at src/core/nginx.c:295
```

得到了调用栈，接下来了解一下栈中各个函数的作用。
从前面的文章中，我们了解到 ngx_http_block 是进行 HTTP 块的配置解析，因此我们从这里开始跟踪。

```
static char *
ngx_http_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ...
        if (module->postconfiguration) {
            if (module->postconfiguration(cf) != NGX_OK) {
                return NGX_CONF_ERROR;
            }
        }
    ...
}
```

`src/http/ngx_http.c:310` 对应的是 postconfiguration 这个语句，也就是在进行 lua-nginx-module postconfiguration 时（见[001-module-init](001-module-init.md)），进行的日志相关内容的绑定/初始化——注入日志 API。postconfiguration 进行了哪些操作我们后续在 [010-lua-vm-init](010-lua-vm-init.md) 进行探究，现在先只关注 Log 相关的内容：


```c
void
ngx_http_lua_inject_log_api(lua_State *L)
{
    ngx_http_lua_inject_log_consts(L);

    lua_pushcfunction(L, ngx_http_lua_ngx_log);
    lua_setfield(L, -2, "log");

    lua_pushcfunction(L, ngx_http_lua_print);
    lua_setglobal(L, "print");
}
```

这部分代码便是注入：ngx.log 以及 ngx.print。
那么 ngx_http_lua_inject_log_consts 是什么呢？

```
static void
ngx_http_lua_inject_log_consts(lua_State *L)
{
    /* {{{ nginx log level constants */
    lua_pushinteger(L, NGX_LOG_STDERR);
    lua_setfield(L, -2, "STDERR");

    lua_pushinteger(L, NGX_LOG_EMERG);
    lua_setfield(L, -2, "EMERG");

    ...

    lua_pushinteger(L, NGX_LOG_DEBUG);
    lua_setfield(L, -2, "DEBUG");
    /* }}} */
}
```

ngx_http_lua_inject_log_consts 注册了一些**日志等级**的常量。
`ngx.log` 的定义流程我们了解了，接下来继续看下 ngx.log 执行时做了什么。

### ngx.log(ngx_http_lua_ngx_log) 的执行流程

```c
- ngx_http_lua_ngx_log：nginx 日志功能的包裹函数。
    \- ngx_http_lua_get_req：从 lua_State 中获取请求
    \- luaL_checkint：获取 log level 的值
    \- lua_remove： 从栈中删除 log level
    \- log_wrapper
        \- if (level > log->log_level)：检查日志级别，级别不够则直接返回
        \- nargs = lua_gettop(L)：获取参数数量
        \- type = lua_type(L, i)：获取每个参数的类型，计算格式化参数需要的大小，格式化参数。（不同类型的处理见上文“使用”章节的注意部分）
        \- ngx_log_error：调用 nginx 核心提供的日志函数打印日志
```

> 如果前面的方法行不通，接下来就会通过源码搜索相关关键字或者是通过搜索引擎搜索介绍相关内容的文章。

