# ngx.sleep


在 OpenResty 中，我们可以通过 ngx.sleep() 让出执行权进行“睡眠/挂起”，让当前请求暂停一段时间（单位为秒）后醒来。

目的：

- 了解 ngx.sleep 的使用。
- 了解 ngx.sleep 的实现。

## 使用

ngx.sleep 的使用非常简单：

```lua
ngx.sleep(0.001)
```

睡眠时间的精度是毫秒（0.001 秒）。

## 实现

ngx.sleep 也是通过 C 代码注入的方式，对应的处理函数是：ngx_http_lua_ngx_sleep，我们直接来跟踪这个函数的实现。

```lua
- ngx_http_lua_ngx_sleep
    \- delay = (ngx_int_t) (luaL_checknumber(L, 1) * 1000)：将参数转换成毫秒
    \- coctx->sleep.handler = ngx_http_lua_sleep_handler：填充 coctx->sleep 事件
    \- ngx_add_timer(&coctx->sleep, (ngx_msec_t) delay)：添加定时器
    \- return lua_yield(L, 0)：让出执行权
```

可以看到，时间到了以后，会调用 ngx_http_lua_sleep_handler：

```lua
- ngx_http_lua_sleep_handler
    \- ctx->cur_co_ctx = coctx：设置当前协程
    \- if (ctx->entered_content_phase)：如果不是 access、rewrite 阶段
        \- ngx_http_lua_sleep_resume：直接 resume
            \- vm = ngx_http_lua_get_lua_vm(r, ctx)：获取 Lua 虚拟机
            \- rc = ngx_http_lua_run_thread(vm, r, ctx, 0)：继续跑协程
    \- else：否则如果是 access、rewrite 阶段，就设置回调，直接进入核心处理流程
        \- ctx->resume_handler = ngx_http_lua_sleep_resume：设置 resume 回调
        \- ngx_http_core_run_phases(r)：然后继续回到核心处理逻辑
    \- ngx_http_run_posted_requests：执行后续的请求
```

在这个函数中，会恢复被暂停的 Lua 协程。

## 总结

ngx.sleep 的实现比较简单，一句话总结：
设置好处理函数和定时器，然后让出执行权，时间到了触发事件调用设置好的处理函数来恢复执行。
