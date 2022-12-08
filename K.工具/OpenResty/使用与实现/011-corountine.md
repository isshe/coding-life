
# 协程

lua-nginx-module 提供了 coroutine 系列 Lua 接口，用于操作协程。

这个与 ngx_http_lua_new_thread 相关的 lua-nginx-module 内部使用的协程不同，切勿搞混了。

目的：

- coroutine Lua 接口的使用？
- coroutine 是如何实现的？
- 与 Luajit 的协程是什么关系？

## 使用

## 实现

在前一篇中，我们了解到 lua-nginx-module 主要是通过注入的方式注册到 Lua VM 中，coroutine 也不例外，我们来看下。

```c
```
