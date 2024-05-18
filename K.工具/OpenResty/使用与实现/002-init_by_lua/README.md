# init_by_lua*

> 相关代码逻辑出自 lua-nginx-module v0.10.21 版本

当 Nginx 收到 HUP 信号并开始重新加载配置文件时，Lua VM 会被重新创建，并且 `init_by_lua*` 会在新的 Lua VM 上再次运行。
如果关闭 lua_code_cache 指令（默认打开），`init_by_lua*` 处理程序（Lua 代码）将在每个请求上运行，因为在这种特殊模式下，需要为每个请求创建一个独立的 Lua VM。

## 用法

> init_by_lua 已不建议使用，使用 init_by_lua_block 和 init_by_lua_file 代替。

- 使用场景：
  - 在 `init_by_lua*` 中，加载 Lua 模块，如 `require "cjson"`，再在其他阶段（如 content_by_lua*）使用：再次 require，这次 require 会很快，直接从 package.loaded 中获取返回。

- 执行阶段：loading-config，配置加载阶段
- 上下文：http
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

```c
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

- ngx_http_lua_init_by_inline 如何被调用？

详见上篇文章《[模块初始化](001-module-init.md)》的“ngx_http_lua_init 执行流程”

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

- 调用 luaL_loadbuffer 函数从缓冲区加载 Lua 代码
- 调用 ngx_http_lua_do_call 设置处理函数，并执行 Lua 代码
- 调用 ngx_http_lua_report 检查执行状态，出错则打印错误日志，并进行垃圾回收

到目前为止，我们得到了一个比较上层的 Lua 代码执行流程。
更详细的 Lua 代码执行细节（如 lua_loadx、lua_pcall 等函数的行为），后续再来探索。


## TODO

- 学习执行 Lua 代码相关的内容。
