
# OpenResty 协程/线程

> 后续都称为协程。

通过前面的探索可以看出，ngx_http_lua_run_thread 是相当重要的一个函数，对于协程的调度，都是通过此接口进行。

目的：

- 了解 OpenResty 协程是什么？
- 是否和此函数强关联：ngx_http_lua_run_thread？
- OpenResty 的协程与 Luajit 的协程是什么关系？
- OpenResty 协程是如何创建的？是何时创建的？（排除前面探究过的轻线程和 coroutine 接口）
- OpenResty 协程会在何时执行？（排除前面探究过的轻线程和 coroutine 接口）
- 主协程和其他协程是如何区分的？
- Lua VM 和主协程是什么关系？

## 使用

> 不涉及

## 实现

### 协程的状态

```c
typedef enum {
    NGX_HTTP_LUA_CO_RUNNING   = 0, /* coroutine running */
    NGX_HTTP_LUA_CO_SUSPENDED = 1, /* coroutine suspended */
    NGX_HTTP_LUA_CO_NORMAL    = 2, /* coroutine normal */
    NGX_HTTP_LUA_CO_DEAD      = 3, /* coroutine dead */
    NGX_HTTP_LUA_CO_ZOMBIE    = 4, /* coroutine zombie */
} ngx_http_lua_co_status_t;
```

- NGX_HTTP_LUA_CO_RUNNING：当前协程正在执行
- NGX_HTTP_LUA_CO_SUSPENDED：当前协程已挂起，等待恢复
- NGX_HTTP_LUA_CO_NORMAL：正常
- NGX_HTTP_LUA_CO_DEAD：协程死亡/终止了
- NGX_HTTP_LUA_CO_ZOMBIE：是僵尸协程，等待父协程回收

### 新建协程：ngx_http_lua_new_thread

```
- ngx_http_lua_new_thread
    \- ngx_http_get_module_main_conf：获取模块配置
    \- if (L == lmcf->lua && !ngx_queue_empty(&lmcf->cached_lua_threads))：检查是否有缓存的协程，有就直接复用
    \- else
        \- lua_pushlightuserdata(L, ngx_http_lua_lightudata_mask(coroutines_key))：把 key 名放到栈顶
        \- lua_rawget(L, LUA_REGISTRYINDEX)：把注册表 L[LUA_REGISTRYINDEX][coroutines_key] 压栈
        \- co = lua_newthread(L)：创建一个协程，并放到 L 的栈顶
        \- ngx_http_lua_create_new_globals_table：为新协程创建全局表并设置 _G 指向自身
        \- ngx_http_lua_set_globals_table：设置新的全局表
        \- *ref = luaL_ref(L, -2)：为 registry[co] 创建一个引用（ref），ref 是一个唯一的整数，后续可通过 lua_rawgeti(L, t, ref) 来获取到对应的对象
        \- lua_settop(L, base)：恢复主协程的堆栈
```

此函数做了以下事情：

- 创建协程
- 设置协程的全局表
    - 新建表 t，并设置 t["_G"] = t，从而在 Lua 代码中可以通过 ["_G"] 访问到全局表
    - 新建表 mt，设置 mt["__index"] = global_table（当前的全局表）
    - 设置 mt 作为 t 的元表
    - 设置 t 为协程新的全局表
    - 这时候，即可
        - **同时访问 `t` 和 `原 global_table` 的内容**
        - **子协程可以访问父协程的内容，而子协程之间无法互相访问**
- 为协程创建`引用`，以能快速在 coroutines_key 注册表中找到协程

### 运行协程：ngx_http_lua_run_thread

根据代码注释，我们得知：
此函数用于执行一个通过 ctx->cur_co_ctx->co 指定的 Lua 协程。
它的返回值有：

- `NGX_AGAIN`：被 IO 中断。r->main->count 不变。
- `NGX_DONE`：被 IO 中断。r->main->count 已经 + 1。
- `NGX_ERROR`：出错了
- `>= 200`：HTTP 状态码

执行流程：

```
- ngx_http_lua_run_thread
    \- lua_atpanic(L, ngx_http_lua_atpanic)：设置 Lua VM 的出错处理程序
    \- if (setjmp(ngx_http_lua_exception) == 0)：第一次调用 setjmp
        \- ngx_http_lua_pcre_malloc_init：设置成 lua-nginx-module 自身的 pcre 内存分配/释放函数
        \- lua_resume：恢复 ctx->cur_co_ctx 协程的执行
        \- ngx_http_lua_pcre_malloc_done：恢复成原来的分配/释放函数
        \- switch (rv)：resume 结果
            \- case LUA_YIELD：协程主动 yield 的
                \- if (r->uri_changed)：如果 URI 变了，则进行跳转
                    \- ngx_http_lua_handle_rewrite_jump
                \- if (ctx->exited)：如果需要退出（ngx.exit），则执行对应的处理程序
                    \- ngx_http_lua_handle_exit
                \- if (ctx->exec_uri.len)：如果调用了 ngx.exec，则执行对应的处理程序
                \- switch (ctx->co_op)：检查是进行了什么操作导致 yield 的
                    \- case NGX_HTTP_LUA_USER_CORO_NOP：ngx.socket 和 ngx.sleep 导致。表示不再有协程需要处理了，跳出这一次循环，等待下一次的读写时间，或者定时器到期
                        \- ctx->cur_co_ctx = NULL：接下来没有需要执行的协程了，等待 event 调度吧
                    \- case NGX_HTTP_LUA_USER_THREAD_RESUME：ngx.thread.spawn 导致，ctx->cur_co_ctx 已经在接口里面设置过了，不用再设置了。
                    \- case NGX_HTTP_LUA_USER_CORO_RESUME：coroutine.resume 导致，ctx->cur_co_ctx 已经在接口里面设置过了，不用再设置了。
                    \- default（NGX_HTTP_LUA_USER_CORO_YIELD）：coroutine.yield 导致
                        \- next_coctx = ctx->cur_co_ctx->parent_co_ctx：父协程作为下一次要执行的协程
                        \- ctx->cur_co_ctx = next_coctx：设置成下一次要执行的协程
            \- case 0：协程终止了
                \- ngx_http_lua_cleanup_pending_operation：清理 pending 的操作
                \- ctx->cur_co_ctx->co_status = NGX_HTTP_LUA_CO_DEAD：设置状态为 DEAD
                \- if (ctx->cur_co_ctx->zombie_child_threads)：如果有僵尸子协程，则进行清理
                    \- ngx_http_lua_cleanup_zombie_child_uthreads
                \- if (ngx_http_lua_is_entry_thread(ctx))：如果是入口协程，则删除协程
                    \- ngx_http_lua_del_thread
                \- if (ctx->cur_co_ctx->is_uthread)：如果是用户协程
                    \- if (ngx_http_lua_coroutine_alive(parent_coctx))：父协程存活
                        \- if (ctx->cur_co_ctx->waited_by_parent)：是否正在被父协程 wait
                        \- ngx_http_lua_post_zombie_thread：没有被父协程 wait，加入到僵尸协程中
                    \- else：父协程也不存在了
                        \- ngx_http_lua_del_thread：删除协程
                \- else：协程属于存在父协程的子协程
                    \- ctx->cur_co_ctx = next_coctx：把父协程设置为接下来要执行的协程
            \- case LUA_ERRRUN：运行时错误
            \- case LUA_ERRSYNTAX：Lua 代码存在语法错误
            \- case LUA_ERRMEM：内存分配错误
            \- case LUA_ERRERR：错误处理程序出错
            \- default：未知错误
        \- ngx_http_lua_cleanup_pending_operation：清理 pending 操作
        \- ctx->cur_co_ctx->co_status = NGX_HTTP_LUA_CO_DEAD：当前协程状态设置为 DEAD
        \- if (orig_coctx->is_uthread || orig_coctx->is_wrap || ngx_http_lua_is_entry_thread(ctx))
            \- ngx_http_lua_thread_traceback：如果协程是入口协程或者是 spawn 出来的、或者是 wrap 出来的，则打印堆栈
        \- ...
```

此函数主要做了以下事情：

- 设置错误处理程序，当 Lua 代码执行出错时，调用该处理程序
    - 疑问：什么时候/什么样的错误会调用此函数呢？
- resume 起 ctx->cur_co_ctx 协程，根据执行结果进行后续的处理
    - 协程 yield 了，则判断 yield 的类型，进行不同的行为。
    - 协程 DEAD 了，则进行相关清理操作或者放到僵尸协程列表中

简而言之，就是唤醒协程，并根据执行结果进行不同的处理。

### 错误处理：ngx_http_lua_atpanic

```c
- ngx_http_lua_atpanic
    \- if (lua_type(L, -1) == LUA_TSTRING)：如果有错误信息，则使用，否则使用 “unknown reason”
    \- ngx_log_stderr：打印错误信息到 nginx 错误日志中
    \- ngx_quit = 1：把进程的状态设置成优雅退出（退出后 master 会重新拉起新的进程）
    \- NGX_LUA_EXCEPTION_THROW：实际就是 longjmp(ngx_http_lua_exception, (x))，ngx_http_lua_exception 是 setjum 最近保存的执行环境。
```

此函数作为新的错误处理函数，会覆盖默认的 Lua panic 程序。
它的作用是：
- 输出 Lua VM 奔溃原因到 Nginx 错误日志中
- 跳到 setjmp 处恢复执行
    - 疑问：所以 ngx_quit 生效了么？最终是恢复执行还是退出了呢？

## 总结

- OpenResty 协程是什么？

答：OpenResty 中的协程是一种轻量级的线程，用于实现在 OpenResty 应用中的并发。协程可以在不阻塞其他请求的情况下执行多个任务，从而提高应用程序的效率和吞吐量。协程是在 Lua 脚本中实现的，可以在 OpenResty 中同时处理多个请求，每个请求都有自己的协程。

- 是否和此函数强关联：ngx_http_lua_run_thread？

答：是，这个函数就是执行/调度 OpenResty 协程的函数，会参与协程的调度。（如继续执行 post thread、设置 ctx->cur_co_ctx 以调度下一个协程）

- OpenResty 的协程与 Luajit 的协程是什么关系？

答：基于 Luajit 的协程，配合 Nginx 事件模型进行工作。

- OpenResty 协程是如何创建的？是何时创建的？（排除前面探究过的轻线程和 coroutine 接口）

答：access、rewrite、content 等阶段执行 Lua 代码前。header filter、body filter 等阶段不会新建协程来执行 Lua 代码，而是直接调用 Luajit 接口进行执行。

- OpenResty 协程会在何时执行？（排除前面探究过的轻线程和 coroutine 接口）

答：access、rewrite、content 阶段执行 Lua 代码时、ngx.sleep、timer、tcp.socket 时。

- 何时会调用错误处理程序 ngx_http_lua_atpanic？panic 后 ngx_quit 生效了么？最终是恢复执行还是退出了呢？

答：Lua VM 崩溃时。注意语法错误、索引 nil 变量等错误时，不会调用。（或许内存不足时会调用，暂未构造出场景）

- 主协程和其他协程是如何区分的？

答：OpenResty 中的主协程和其他协程是通过创建协程的方式区分的。
1. 主协程：每个请求的初始协程就是主协程，它负责处理请求的主流程，并处理请求和响应。
2. 其他协程：主协程可以通过调用 ngx.thread.spawn() 等方式创建其他协程，这些协程可以在独立环境中运行，不会影响主协程的正常工作。
主协程和其他协程之间通过数据共享、信号量等方式进行通信，以实现多线程的并行处理能力。

- Lua VM 和主协程是什么关系？

答：OpenResty 中的 Lua VM 和主协程是密切相关的。
1. Lua VM：是 OpenResty 运行 Lua 代码的运行环境，它负责编译和执行 Lua 代码，并管理内存分配和回收。
2. 主协程：是 OpenResty 中处理请求的核心协程，它在 Lua VM 中运行，负责读取请求信息、执行 Lua 脚本、生成响应等。
因此，可以说：主协程是在 Lua VM 环境中运行的，它依赖于 Lua VM 提供的运行环境和资源。**每次请求都会创建一个独立的主协程，独立运行，互不影响。**
