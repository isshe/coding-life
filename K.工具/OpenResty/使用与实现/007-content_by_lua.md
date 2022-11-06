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

### ngx_http_lua_content_handler_inline 被调用位置

### ngx_http_lua_content_handler_inline 执行过程
