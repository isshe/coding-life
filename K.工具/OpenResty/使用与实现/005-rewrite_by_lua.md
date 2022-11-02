# rewrite_by_lua*

`rewrite_by_lua*` 充当一个 rewrite 阶段的处理程序，对**每个请求**执行指定的 Lua 代码，代码会在独立的全局环境（沙箱）中执行。

## 用法

- 上下文: http, server, location, location if
- 阶段: rewrite tail
  - 执行阶段晚于标准 ngx_http_rewrite_module 模块。
- 语法：
    - 与 `init_by_lua*` 类似，不再赘述。
- 注意：
  - 如果 rewrite_by_lua* 指定的 Lua 代码中通过 ngx.exit(code) 退出，如果 code = ngx.OK（非 ngx.HTTP_OK），则会继续执行!

## 实现

> 以 rewrite_by_lua_block 为例

开始前，还是如以往一样，先明确此行的目的：

- 解析指令时，做了什么？猜测和前面两个没有太大差别。
- 在什么时候实际执行了 Lua 代码？如何执行的？每个请求都会执行这个，想必处理方法应该不同以往。
- 还提到“全局环境（沙箱）”，是什么呢？

指令定义：

```
    { ngx_string("rewrite_by_lua_block"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_BLOCK|NGX_CONF_NOARGS,
      ngx_http_lua_rewrite_by_lua_block,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      (void *) ngx_http_lua_rewrite_handler_inline },
```

- ngx_http_lua_rewrite_by_lua_block：配置解析时执行的函数。
- ngx_http_lua_rewrite_handler_inline：实际执行 Lua 代码的函数。

还是围绕 3 点展开：

- 指令解析函数 ngx_http_lua_rewrite_by_lua_block 做了什么？
- 处理程序（cmd->post, ngx_http_lua_rewrite_handler_inline）在什么时候执行的？
- 处理程序（cmd->post, ngx_http_lua_rewrite_handler_inline）做了什么？

### ngx_http_lua_rewrite_by_lua_block 执行流程

```
- cf->handler = ngx_http_lua_rewrite_by_lua;：直接设置处理程序，看起来是直接复用了，和 rewrite_by_lua 使用的同一个函数。
    \- ngx_http_lua_conf_lua_block_parse
        \- if (cf->conf_file->file.fd != NGX_INVALID_FILE)：检查一下配置文件的文件描述符，有效，则认为是在配置文件中使用的指令。无效，则认为是通过 Nginx 参数(-g)调用的。
        \- ngx_http_lua_conf_read_lua_token：解析配置
        \- rv = (*cf->handler)(cf, cmd, cf->handler_conf)：调用 ngx_http_lua_rewrite_by_lua 设置 rewrite_handler
            \- if (cmd->post == NULL)：没有设置执行 Lua 代码的函数，则返回
            \- if (llcf->rewrite_handler)：已经设置过处理程序了，也直接返回
            \- if (cmd->post == ngx_http_lua_rewrite_handler_inline)：Lua 代码是在内存中，表示用的 rewrite_by_lua_block 或 rewrite_by_lua 指令 。
                \- ngx_http_lua_gen_chunk_name：生成代码块名称，示例：=rewrite_by_lua(nginx.conf:50)
                \- ngx_http_lua_gen_chunk_cache_key：生成代码块缓存用的 key，示例：rewrite_by_lua_nhli_6f30fa99b87d7f63b59c913687f45f65
            \- else: Lua 代码在文件中，用的 rewrite_by_lua_file 指令（拓展一下）
                \- ngx_http_compile_complex_value：文件名中可能有变量，场景：从请求 URL 中取一部分作为文件名。
                \- if (llcf->rewrite_src.lengths == NULL)：文件名中没有变量
                    \- ngx_http_lua_gen_file_cache_key：生成 cache key（注意文件名如果有变量，则不进行 cache）
            \- llcf->rewrite_handler = (ngx_http_handler_pt) cmd->post;：设置回调，也就是 ngx_http_lua_rewrite_handler_inline
            \- ngx_http_conf_get_module_main_conf：获取配置，然后设置一些必要的标记，如 requires_rewrite
```

### ngx_http_lua_rewrite_handler_inline 的调用位置

根据前面的执行流程，我们知道处理程序是被设置在了 `rewrite_handler` 中，通过搜索，我们得到

```
- ngx_http_lua_rewrite_handler
    \- rewrite_handler
```

那么 `ngx_http_lua_rewrite_handler` 是通过在哪里调用了呢？
通过搜索 `ngx_http_lua_rewrite_handler`，在 `ngx_http_lua_init` 函数中找到以下代码：

> ngx_http_lua_init 相关内容可见 [001-module-init.md)](001-module-init.md) 或 [002-init_by_lua.md](002-init_by_lua.md)

```
    if (lmcf->requires_rewrite) {
        h = ngx_array_push(&cmcf->phases[NGX_HTTP_REWRITE_PHASE].handlers);
        if (h == NULL) {
            return NGX_ERROR;
        }

        *h = ngx_http_lua_rewrite_handler;
    }
```

可见，`ngx_http_lua_rewrite_handler` 被添加到了 `REWRITE` 阶段的处理程序数组中。

接下来我们直接使用 gdb 获取调用栈：

> $ gdb -p PID
> > b ngx_http_lua_rewrite_handler
> > bt


```lua
#0  ngx_http_lua_rewrite_handler (r=0x0) at ../ngx_lua-0.10.21/src/ngx_http_lua_rewriteby.c:26
#1  0x0000564c0906158e in ngx_http_core_rewrite_phase (r=0x564c09c74750, ph=0x564c09c9bea0)
    at src/http/ngx_http_core_module.c:939
#2  0x0000564c090613de in ngx_http_core_run_phases (r=0x564c09c74750) at src/http/ngx_http_core_module.c:885
#3  0x0000564c09061347 in ngx_http_handler (r=0x564c09c74750) at src/http/ngx_http_core_module.c:868
#4  0x0000564c09072044 in ngx_http_process_request (r=0x564c09c74750) at src/http/ngx_http_request.c:2120
#5  0x0000564c09070710 in ngx_http_process_request_headers (rev=0x564c09cb2f00) at src/http/ngx_http_request.c:1498
#6  0x0000564c0906fa6a in ngx_http_process_request_line (rev=0x564c09cb2f00) at src/http/ngx_http_request.c:1165
#7  0x0000564c0906e061 in ngx_http_wait_request_handler (rev=0x564c09cb2f00) at src/http/ngx_http_request.c:503
#8  0x0000564c0904a223 in ngx_epoll_process_events (cycle=0x564c09c70740, timer=60000, flags=1)
    at src/event/modules/ngx_epoll_module.c:901
#9  0x0000564c09035ee2 in ngx_process_events_and_timers (cycle=0x564c09c70740) at src/event/ngx_event.c:257
#10 0x0000564c09047402 in ngx_worker_process_cycle (cycle=0x564c09c70740, data=0x0)
    at src/os/unix/ngx_process_cycle.c:793
...
```

读事件到达 worker 进程后，相应的事件处理模块（不同系统应当是使用了不同的事件模块，例如 Linux 使用 epoll，Unix 使用 kqueue ）调用读事件处理程序对请求进行读取并解析，然后调用各个阶段的处理程序。

（请求的处理过程及阶段切换过程，在后续的 Nginx 系列文章中进行补充。）

### ngx_http_lua_rewrite_handler 执行流程

```

```
