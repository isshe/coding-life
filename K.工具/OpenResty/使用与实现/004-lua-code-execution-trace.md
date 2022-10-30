# Lua 代码执行实例跟踪

> 实际跟踪 Lua 代码的执行情况（主要是出错情况）。

## 异常情况

由于使用 bpftrace 得到的堆栈不符合预期，直接用 gdb 来获取：

指令：

```
    # index a nil value
    init_by_lua 'return a.b';
```

对 nil 值进行索引。

错误输出：

```
nginx: [error] init_by_lua error: init_by_lua:1: attempt to index global 'a' (a nil value)
stack traceback:
        init_by_lua:1: in main chunk
```

命令：

```
$ gdb sbin/nginx
> set args -p /NGINX/CONFIG/PATH
> b ngx_http_lua_traceback
> run
> bt
```

从 init_by_lua 的文章中，已经知道了 ngx_http_lua_traceback 是错误处理函数，因此我们直接追踪它。

结果：

```
#0  ngx_http_lua_traceback (L=0x7ffff76f02e8) at ngx_lua-0.10.21/src/ngx_http_lua_util.c:3071
#1  0x00007ffff7eb1b83 in lj_BC_FUNCC () from luajit/lib/libluajit-5.1.so.2
#2  0x00007ffff7eb7098 in lj_err_run (L=L@entry=0x7ffff7717380) at lj_err.c:856
#3  0x00007ffff7eb72ed in err_msgv (L=L@entry=0x7ffff7717380, em=em@entry=LJ_ERR_BADOPRT) at lj_err.c:881
#4  0x00007ffff7eb7474 in lj_err_optype (L=L@entry=0x7ffff7717380, o=o@entry=0x7ffff770f380, opm=opm@entry=LJ_ERR_OPINDEX) at lj_err.c:915
#5  0x00007ffff7ebb967 in lj_meta_tget (L=0x7ffff7717380, o=0x7ffff770f380, k=0x7fffffffd2d0) at lj_meta.c:147
#6  0x00007ffff7eb2089 in lj_vmeta_tgetv () from luajit/lib/libluajit-5.1.so.2
#7  0x00007ffff7ecc1cd in lua_pcall (L=0x7ffff7717380, nargs=0, nresults=0, errfunc=<optimized out>) at lj_api.c:1145
#8  0x000055555570c0fb in ngx_http_lua_do_call (log=0x55555583d280 <ngx_log>, L=0x7ffff7717380) at ngx_lua-0.10.21/src/ngx_http_lua_util.c:4192
#9  0x000055555572bda3 in ngx_http_lua_init_by_inline (log=0x55555583d280 <ngx_log>, lmcf=0x555555890e80, L=0x7ffff7717380) at ngx_lua-0.10.21/src/ngx_http_lua_initby.c:24
#10 0x00005555556fb594 in ngx_http_lua_init (cf=0x7fffffffd7e0) at ngx_lua-0.10.21/src/ngx_http_lua_module.c:896
#11 0x00005555555f4ddc in ngx_http_block (cf=0x7fffffffd7e0, cmd=0x55555581a200 <ngx_http_commands>, conf=0x555555886a20) at src/http/ngx_http.c:310
#12 0x00005555555b9ec7 in ngx_conf_handler (cf=0x7fffffffd7e0, last=1) at src/core/ngx_conf_file.c:463
#13 0x00005555555b99cc in ngx_conf_parse (cf=0x7fffffffd7e0, filename=0x555555885958) at src/core/ngx_conf_file.c:319
#14 0x00005555555b4f87 in ngx_init_cycle (old_cycle=0x7fffffffd9b0) at src/core/ngx_cycle.c:284
#15 0x0000555555592fe8 in main (argc=3, argv=0x7fffffffdd58) at src/core/nginx.c:295
```

可以看到最终 ngx_http_lua_traceback 是被 Luajit 调用了，这是在 ngx_http_lua_do_call 中设置的。

- ngx_http_lua_traceback 函数源码：

```
int
ngx_http_lua_traceback(lua_State *L)
{
    if (!lua_isstring(L, 1)) { /* 'message' not a string? */
        return 1;  /* keep it intact */
    }

    lua_getglobal(L, "debug");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return 1;
    }

    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 2);
        return 1;
    }

    lua_pushvalue(L, 1);  /* pass error message */
    lua_pushinteger(L, 2);  /* skip this function and traceback */
    lua_call(L, 2, 1);  /* call debug.traceback */
    return 1;
}
```

根据注释可以看到，主要做了：

- 设置错误信息
- 跳过 2 个栈帧：当前 Lua 函数和栈帧 [?, 猜测]
- 调用 debug.traceback

因此，此函数看起来是获取调用栈，然后在后续打印到日志中（由 ngx_http_lua_report 调用 ngx_log_error）。

由于缺乏对 `lua_*` 相关函数的了解，还是没法继续深入，先作罢，后续再读下 luajit 源码去了解。
