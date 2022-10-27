# init_by_lua*

> 相关代码逻辑出自 lua-nginx-module v0.10.21 版本

## 用法

`init_by_lua*` 系列指令会在配置加载阶段(loading-config)执行，上下文是 "http"。
init_by_lua 已不建议使用，使用 init_by_lua_block 和 init_by_lua_file 代替。

- 语法：

```bash
init_by_lua 'lua-script-str'
init_by_lua_block { lua-script }
init_by_lua_file <path-to-lua-script-file>
```

- 示例：
```
init_by_lua 'print("Hello World")'

init_by_lua_block {
    print("Hello World")
}

init_by_lua_file /usr/local/openresty/lua/init.lua
```

## 实现

> 以 init_by_lua 为例

### 指令定义

```
    { ngx_string("init_by_lua"),
      NGX_HTTP_MAIN_CONF|NGX_CONF_TAKE1,
      ngx_http_lua_init_by_lua,
      NGX_HTTP_MAIN_CONF_OFFSET,
      0,
      (void *) ngx_http_lua_init_by_inline },
```

可以看到，在解析到 init_by_lua 指令时，会调用 ngx_http_lua_init_by_lua 函数进行处理，那么

- ngx_http_lua_init_by_lua 这个函数中会执行 Lua 代码吗？
- ngx_http_lua_init_by_inline 函数又是做什么用？

### ngx_http_lua_init_by_lua 执行流程

```
- ngx_http_lua_init_by_lua
    \- if (cmd->post == NULL)：检查是否有 handler，没有直接报错。
    \- if (lmcf->init_handler)：检查是否已经配置过 init_handler 了，配置过直接返回。
    \- lmcf->init_handler = (ngx_http_lua_main_conf_handler_pt) cmd->post：设置 init_handler，也就是把 init_handler 设置为 ngx_http_lua_init_by_inline。
    \- ngx_http_lua_rebase_path：Lua 代码在文件中，则调用此函数获取 Lua 文件的绝对路径。
    \- ngx_http_lua_gen_chunk_name：是 Lua 代码块，则调用此函数生成 chunk name。
    \- lmcf->init_src：设置 Lua 文件路径或 Lua 代码到此变量中
```

可以看到，此函数中只是设置了 init_handler，并没有执行对应的 Lua 代码，那么 Lua 代码应该是在 ngx_http_lua_init_by_inline 中执行了。

### ngx_http_lua_init_by_inline 执行流程

上篇文章《[模块初始化](001-module-init.md)》 的 “ngx_http_lua_init 执行流程” 中，已经有提到 ngx_http_lua_init_by_inline 如何被调用，
这里我们直接贴过来。

- ngx_http_lua_init_by_inline 的调用栈

```
- ngx_cycle_post_init：在 main 函数中调用
    \- post_init(ngx_http_lua_post_init_handler)
        \- init_handler(ngx_http_lua_init_by_inline)
```

- ngx_http_lua_init_by_inline 的执行流程

```
- ngx_http_lua_init_by_inline
    \- luaL_loadbuffer：加载 Lua 代码，这是 luajit 的接口
        \- luaL_loadbufferx：这部分后续放到 Luajit 目录下
        \- lua_loadx
            \- lj_buf_init
            \- lj_vm_cpcall
            \- lj_lex_cleanup
            \- lj_gc_check
    \- ngx_http_lua_do_call：执行代码
        \- lua_gettop：获取函数在堆栈的索引/位置[?]
        \- lua_insert：索引压栈
        \- lua_pushcfunction： traceback 压栈函数/异常处理函数（值：ngx_http_lua_traceback），出错时调用。
        \- ngx_http_lua_pcre_malloc_init：PCRE 内存分配函数及内存池修改，改为使用 cycle 的内存池（ngx_cycle->pool）
        \- lua_pcall：执行 Lua 代码
        \- ngx_http_lua_pcre_malloc_done：代码执行完了，还原回去
        \- lua_remove：索引出栈
    \- ngx_http_lua_report：
        \- if (status && !lua_isnil(L, -1))：状态不为 0（出错了）并且 Lua 类型不是 nil
            \- lua_tostring：把错误消息取出来
            \- ngx_log_error：打印到 nginx log 中
        \- lua_gc：强制执行一次完整的垃圾收集
```

这个函数主要做了以下事情：

- 从缓冲区加载 Lua 代码
- 执行 Lua 代码
- 出错则打印错误日志
- 代码执行完进行垃圾回收

到目前为止，我们得到了一个比较上层的 Lua 代码执行流程。
更详细的 Lua 代码执行细节（如 lua_loadx、lua_pcall 等函数的行为），后续再来探索。
