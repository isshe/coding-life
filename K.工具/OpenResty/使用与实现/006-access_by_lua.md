# access_by_lua*

`access_by_lua*` 充当一个 access 阶段的处理程序，对**每个请求**执行指定的 Lua 代码，代码会在独立的全局环境（沙箱）中执行。

## 用法

- 上下文: http, server, location, location if
- 阶段: access tail
  - **注意：执行阶段晚于标准 ngx_http_access_module 模块。**
- 语法：
    - 与 `init_by_lua*` 类似，不再赘述。

## 实现

实现逻辑与 `rewrite_by_lua*` 别无二致，不再赘述。
