# content_by_lua*

`content_by_lua*` 充当一个 content 阶段的处理程序，对**每个请求**执行指定的 Lua 代码，代码会在独立的全局环境（沙箱）中执行。

## 用法

- 上下文: location, location if
  - **注意，不能定义在 http、server 块内了。**
- 阶段: content
- 语法：
    - 与 `init_by_lua*` 类似，不再赘述。
- **注意：这个指令不要和其他 content handler 指令（如 proxy_pass）在同一个 location 同时使用。**

## 实现

在《[模块初始化](001-module-init.md)》 的 “ngx_http_lua_init 执行流程” 中，
我们了解到 content handler 并没有像 rewrite handler 一样在函数 ngx_http_lua_init 中被放到 `cmcf->phases` 数组中，所以接下来我们来探究一下为什么。
因此，我们此行的目的：

- 了解指令是如何被解析的。（猜测与前面的阶段没有什么差异）
- 了解 content handler 是被如何设置的，**为何**于其他阶段的 handler 不同。
- 了解是何时执行了 Lua 代码，如何执行的。（猜测与前面的阶段没有什么差异）

### 指令定义

```c
    /* content_by_lua "<inline script>" */
    { ngx_string("content_by_lua"),
      NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_TAKE1,
      ngx_http_lua_content_by_lua,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      (void *) ngx_http_lua_content_handler_inline },
```

### ngx_http_lua_content_by_lua 执行过程

```c
- ngx_http_lua_content_by_lua
    \- if (cmd->post == NULL)：检查是否有指定处理程序，没有直接报错返回。
    \- if (llcf->content_handler)：检查是否已经设置了处理程序，设置了报错返回。
    \- if (value[1].len == 0)：检查配置块的大小，为 0 表示无效的 Lua 代码块，直接报错返回。
    \- if (cmd->post == ngx_http_lua_content_handler_inline)：检查 Lua 代码是文件还是其他
        \- ngx_http_lua_gen_chunk_name：不是文件，生成 chunkname 和 cache key
        \- ngx_http_lua_gen_chunk_cache_key
    \- else：是文件
        \- ngx_http_compile_complex_value：解析文件路径，是否包含变量之类的（不建议使用变量，容易有安全问题）
        \- ngx_http_lua_gen_file_cache_key：生成 cache key
    \- llcf->content_handler = (ngx_http_handler_pt) cmd->post：设置处理函数——执行 Lua 代码的函数
    \- lmcf->requires_capture_filter = 1
    \- clcf->handler = ngx_http_lua_content_handler
```

可以看到，流程与其他阶段的指令没有太多区别，只是：

- clcf->handler：这个是什么呢？有什么作用呢？什么时候执行的呢？

根据之前的经验，看 ngx_http_lua_content_handler 名字就能猜到，此函数中毫无疑问会调用 ngx_http_lua_content_handler_inline，也就是 cmd->post。
也就是，content handler 的设置，不是像 access handler 那样直接放到一个数组（cmcf->phases）中。
解释了 clcf->handler 是什么，那么它是什么时候执行的呢？

### ngx_http_lua_content_handler_inline 被调用位置

我们再来用 gdb 看下 ngx_http_lua_content_handler_inline（ngx_http_lua_content_handler）是如何被调用的。

命令：

```bash
$ gdb -p PID
> b ngx_http_lua_content_handler_inline
$ curl localhost    # 其他 shell
> c
> bt
```

得到：

```lua
#0  ngx_http_lua_content_handler_inline (r=0x0) at ../ngx_lua-0.10.21/src/ngx_http_lua_contentby.c:291
#1  0x000056030bb45ae0 in ngx_http_lua_content_handler (r=0x56030bd7b7e0)
    at ../ngx_lua-0.10.21/src/ngx_http_lua_contentby.c:222
#2  0x000056030ba324f5 in ngx_http_core_content_phase (r=0x56030bd7b7e0, ph=0x56030bdbc100)
    at src/http/ngx_http_core_module.c:1269
#3  0x000056030ba313de in ngx_http_core_run_phases (r=0x56030bd7b7e0)
    at src/http/ngx_http_core_module.c:885
#4  0x000056030ba31347 in ngx_http_handler (r=0x56030bd7b7e0) at src/http/ngx_http_core_module.c:868
#5  0x000056030ba42044 in ngx_http_process_request (r=0x56030bd7b7e0)
    at src/http/ngx_http_request.c:2120
#6  0x000056030ba40710 in ngx_http_process_request_headers (rev=0x56030bdc54c0)
    at src/http/ngx_http_request.c:1498
#7  0x000056030ba3fa6a in ngx_http_process_request_line (rev=0x56030bdc54c0)
    at src/http/ngx_http_request.c:1165
#8  0x000056030ba3e061 in ngx_http_wait_request_handler (rev=0x56030bdc54c0)
    at src/http/ngx_http_request.c:503
#9  0x000056030ba1a223 in ngx_epoll_process_events (cycle=0x56030bd77740, timer=60000, flags=1)
    at src/event/modules/ngx_epoll_module.c:901
#10 0x000056030ba05ee2 in ngx_process_events_and_timers (cycle=0x56030bd77740)
    at src/event/ngx_event.c:257
#11 0x000056030ba17402 in ngx_worker_process_cycle (cycle=0x56030bd77740, data=0x0)
    at src/os/unix/ngx_process_cycle.c:793
#12 0x000056030ba1361a in ngx_spawn_process (cycle=0x56030bd77740,
    proc=0x56030ba17311 <ngx_worker_process_cycle>, data=0x0, name=0x56030bbeaf7c "worker process",
    respawn=-3) at src/os/unix/ngx_process.c:199
#13 0x000056030ba1606a in ngx_start_worker_processes (cycle=0x56030bd77740, n=1, type=-3)
    at src/os/unix/ngx_process_cycle.c:382
#14 0x000056030ba15511 in ngx_master_process_cycle (cycle=0x56030bd77740)
    at src/os/unix/ngx_process_cycle.c:135
#15 0x000056030b9cb3b9 in main (argc=3, argv=0x7ffce805c458) at src/core/nginx.c:386
```

与 [005-rewrite_by_lua.md](005-rewrite_by_lua.md) 中获取到的调用栈对比，发现并没有什么差别。

> 什么时候执行的问题也解答了：和 rewrite/access 阶段的处理程序一样，都是在相同的地方执行。（都是通过 phase 索引进行控制）

都是在以下代码中进行调用：


```c
void
ngx_http_core_run_phases(ngx_http_request_t *r)
{
    ngx_int_t                   rc;
    ngx_http_phase_handler_t   *ph;
    ngx_http_core_main_conf_t  *cmcf;

    cmcf = ngx_http_get_module_main_conf(r, ngx_http_core_module);

    ph = cmcf->phase_engine.handlers;

    while (ph[r->phase_handler].checker) {

        rc = ph[r->phase_handler].checker(r, &ph[r->phase_handler]);

        if (rc == NGX_OK) {
            return;
        }
    }
}
```

也就是

> rc = ph[r->phase_handler].checker(r, &ph[r->phase_handler]);

这一句。可以看出调用方式和其他阶段的 handler 都是一样的。
那么就有疑问了，clcf->handler 是怎么被设置到这里（ph[r->phase_handler].checker）来的呢？
通过搜索跟踪 clcf->handler，我们得到：

```lua
\- ngx_http_core_find_config_phase
    \- ngx_http_update_location_config
        \- r->content_handler = clcf->handler;
```

再结合前面的“ngx_http_lua_content_by_lua 执行过程”，得到调用流程 —— 和 GDB 获取的一致：

```lua
- ngx_http_core_content_phase
    \- r->content_handler：ngx_http_lua_content_handler（clcf->handler）
        \- llcf->content_handler：ngx_http_lua_content_handler_inline
```

更多关于 Nginx 阶段的内容，我们在 Nginx 系列文章《6-nginx-phase.md》中进行探索。

### ngx_http_lua_content_handler_inline 执行过程

与 ngx_http_lua_access_handler_inline、ngx_http_lua_rewrite_handler_inline 没差别。

```lua
- ngx_http_lua_content_handler_inline
    \- ngx_http_get_module_loc_conf：获取 location 配置
    \- ngx_http_lua_get_lua_vm：获取 Lua VM
    \- ngx_http_lua_cache_loadbuffer：加载 Lua 代码
    \- ngx_http_lua_content_by_chunk：执行 Lua 代码
```
