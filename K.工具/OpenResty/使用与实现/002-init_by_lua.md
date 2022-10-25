
# init_by_lua*

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
    \- lmcf->init_handler = (ngx_http_lua_main_conf_handler_pt) cmd->post;：设置 init_handler，也就是把 init_handler 设置为 ngx_http_lua_init_by_inline。
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
    \- if (lmcf->init_chunkname == NULL)：没设置就使用 "=init_by_lua"
    \- lua_getexdata：获取请求（request），使用了 Luajit，就调用这个接口。
    \- lua_touserdata：获取请求（request），没使用 Luajit，就调用这个接口。
    \- ngx_http_lua_cache_loadbuffer：加载 Lua 代码
        \- ngx_http_lua_cache_load_code：从缓存中加载代码
        \- ngx_http_lua_clfactory_loadbuffer：加载 Lua 脚本的闭包工厂[?]到 Lua 堆栈顶部
            \- lua_load：Luajit 加载 lua 代码函数，后续有必要挪到 Luajit 目录下
                \- lua_loadx
                    \- lj_buf_init
                    \- lj_vm_cpcall
                    \- lj_lex_cleanup
                    \- lj_gc_check
        \- ngx_http_lua_cache_store_code：保存加载的代码到缓存中
    \- ngx_http_lua_init_by_chunk：执行 Lua 代码
        \- ngx_http_get_module_ctx：获取模块上下文
        \- ngx_http_lua_create_ctx：如果获取不到，就创建
        \- ngx_http_lua_reset_ctx：获取到了，就重置[?]
        \- ngx_http_lua_new_thread：新建协程（coroutine）
        \- lua_xmove：移动代码闭包到新协程
        \- ngx_http_lua_get_globals_table：获取协程的 global 表
        \- lua_setfenv：将闭包的 env 表设置为新协程的 global 表
        \- ngx_http_lua_set_req：保存 nginx 请求到协程 global 表中
        \- ngx_http_lua_attach_co_ctx_to_L：[?]
        \- ngx_http_cleanup_add：注册请求的清理回调
        \- ngx_http_lua_run_thread：跑协程
        \- ngx_http_lua_init_run_posted_threads：[?]
        \- ngx_http_lua_finalize_request：结束请求

```

- 闭包工厂（closure factory）是什么？
  - 猜测是生成闭包函数的工厂？
  - FIXME

- 为何要重置模块上下文呢？
  - 猜测是为了避免互相影响
