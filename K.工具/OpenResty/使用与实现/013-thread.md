
# 协程

> 看是否和此函数强关联：ngx_http_lua_run_thread；
> 如果此文章无法完成对 ngx_http_lua_run_thread 的理解，则另外文章。

通过前面的探索可以看出，ngx_http_lua_run_thread 是相当重要的一个函数，对于协程的调度，都是通过此接口进行。

目的：

- 了解 openresty 协程
- openresty 的协程与 Luajit 的协程是什么关系？
- openresty 协程是如何创建的？是何时创建的？（排除前面探究过的轻线程和 coroutine 接口）
- openresty 协程会在何时执行？（排除前面探究过的轻线程和 coroutine 接口）

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

- NGX_HTTP_LUA_CO_RUNNING：协程正在执行
- NGX_HTTP_LUA_CO_SUSPENDED：协程暂停、挂起了
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
- 为协程创建引用，以能快速在 coroutines_key 注册表中找到协程

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
        \- ngx_http_lua_pcre_malloc_init：初始化 pcre
```


