# Lua 代码执行实例跟踪

> 实际跟踪 Lua 代码的执行情况（主要是出错情况）。

由于前面 2

## 异常情况

由于使用 bpftrace 得到的堆栈不符合预期，直接用 gdb 来获取：

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
