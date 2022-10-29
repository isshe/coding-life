# init_worker_by_lua*

## 用法

`init_worker_by_lua*` 系列指令会在 worker 进程启动阶段(starting-worker) 执行，上下文是 "http"。
如果没有启用 master 进程则此指令的 Lua 代码将在 init_by_lua* 指令的 Lua 代码之后运行。
- 启用 master 进程：非 master-worker 的模式

通常用于启动定时器执行每 worker 相关的内容，或者后端健康检查等。

- 语法：

与 `init_by_lua*` 类似，不再赘述。

## 实现

通过前面的探索，我们已经知道指令的解析会在配置解析阶段，而对应的 Lua 代码会在后续才执行。
因此，我们接下来目的很明确：

- 解析 init_worker_by_lua_file 指令时，做了什么？
- 在什么时候实际执行了 Lua 代码？如何执行的？

我们以 init_worker_by_lua_file 指令为例。
先看指令定义：

```
    { ngx_string("init_worker_by_lua_file"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE1,
      ngx_http_lua_init_worker_by_lua,
      NGX_HTTP_MAIN_CONF_OFFSET,
      0,
      (void *) ngx_http_lua_init_worker_by_file },
```

- ngx_http_lua_init_worker_by_lua：指令解析时调用的函数。
- ngx_http_lua_init_worker_by_file：指令结构的 post 字段的值。

解析指令时，会调用 ngx_http_lua_init_worker_by_lua，我们来跟踪一下。

### ngx_http_lua_init_worker_by_lua 执行流程

```
- ngx_http_lua_init_worker_by_lua
    \- if (cmd->post == NULL)：如果缺少回调函数，就直接报错返回。
    \- if (lmcf->init_worker_handler)：如果已经设置过回调了，就直接返回。
    \- lmcf->init_worker_handler = (ngx_http_lua_main_conf_handler_pt) cmd->post;：把回调设置到 init_worker_handler 上。
    \- if (cmd->post == ngx_http_lua_init_worker_by_file)：如果是文件，init_worker_src 中则填入绝对路径。否则填入配置里面的代码。
        \- ngx_http_lua_rebase_path：获取文件绝对路径
```

这个函数做的事情很简单，就 2 件：

- 设置好回调函数。
- 设置好 Lua 代码或代码路径。

### ngx_http_lua_init_worker_by_file 的执行位置

根据上面我们知道会调用 init_worker_handler，因此可以反向跟踪代码来得到调用栈。
不过这里我直接使用 bpftrace 来获取：（bpftrace 相关内容见：[M.方法论/阅读源码的方法.md](../../../M.方法论/阅读源码的方法.md)）

- 命令

```bash
sudo bpftrace -e 'uprobe:/opt/openresty/nginx/sbin/nginx:ngx_http_lua_init_worker_by_file {printf("%s\n", ustack());}'
```

然后配置好 nginx.conf 并启动 openresty，即可得到调用栈。

- 调用栈

```
ngx_http_lua_init_worker_by_file+0
ngx_worker_process_init+2009
ngx_worker_process_cycle+69
ngx_spawn_process+1860
ngx_start_worker_processes+126
ngx_master_process_cycle+756
main+1538
__libc_start_main+243
```

有点奇怪，这里为什么会显示 ngx_worker_process_init 直接调用了 ngx_http_lua_init_worker_by_file 呢？
ngx_worker_process_init 是 nginx 框架里面的函数，而 ngx_http_lua_init_worker_by_file 是 lua-nginx-module 模块里面的函数。

前面是把 ngx_http_lua_init_worker_by_file 赋值给了 ngx_http_lua_main_conf_t 结构（变量 lmcf）的 init_worker_handler 字段，
而 ngx_http_lua_main_conf_t 看起来是 lua-nginx-module 的结构，nginx 框架怎么会知道呢？

跟踪代码发现

```
- ngx_worker_process_init
    \- init_process(ngx_http_lua_init_worker)：这是我们在定义模块时候，给 init_process 字段赋值了 ngx_http_lua_init_worker。
        \- init_worker_handler(ngx_http_lua_init_worker_by_file)
```

实际是这样。

但是为什么少了呢？我们后续有机会再来探究。

到这里我们了解到 Lua 代码是在产生 worker 进程后，对 worker 进程进行初始化时被调用了。

### ngx_http_lua_init_worker_by_file 执行流程

```
- ngx_http_lua_init_worker_by_file
    \- luaL_loadfile
    \- ngx_http_lua_do_call
    \- ngx_http_lua_report
```

和 init_by_lua 指令的区别在于 luaL_loadbuffer 换成了 luaL_loadfile。
luaL_loadfile 也就是读文件，然后加载代码；
这些 Luajit 相关的函数，我们后续再探究。

## TODO

- 探究 “为什么调用栈少了一帧”。


## 参考

- https://github.com/openresty/lua-nginx-module#init_worker_by_lua
- https://github.com/openresty/lua-nginx-module#init_worker_by_lua_block
- https://github.com/openresty/lua-nginx-module#init_worker_by_lua
