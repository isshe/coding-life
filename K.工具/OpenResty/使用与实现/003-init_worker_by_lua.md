# init_worker_by_lua*

## 用法

`init_worker_by_lua*` 系列指令会在 worker 进程启动阶段(starting-worker) 执行，上下文是 "http"。
如果没有启用 master 进程则此指令的 Lua 代码将在 init_by_lua* 指令的 Lua 代码之后运行。
- 启用 master 进程：非 master-worker 的模式

通常用于启动定时器执行每 worker 相关的内容，或者后端健康检查等。

- 语法：

与 `init_by_lua*` 类似，不再赘述。

## 实现

> 使用 init_worker_by_lua_file 作为示例


## 参考

- https://github.com/openresty/lua-nginx-module#init_worker_by_lua
- https://github.com/openresty/lua-nginx-module#init_worker_by_lua_block
- https://github.com/openresty/lua-nginx-module#init_worker_by_lua
