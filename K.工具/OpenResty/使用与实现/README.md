
OpenResty 使用与实现
---

# 目标

由浅入深学习 OpenResty 的用法以及核心实现流程的实现方式。

# 问题

- 如何使用的？使用上有什么需要注意的？
- OpenResty 是如何加载并运行 Lua 模块的？
- OpenResty cosocket 是如何实现的？

# 目的

- 详细记录 OpenResty 的相关内容，方便日后搜索查询。
- 如若发表成博客，希望能给入门者提供一个较为完整的视角。

# 说明

- "实现"部分需对 Nginx 的模块实现有了解。
- 基于 lua-nginx-module [v0.10.21](https://github.com/openresty/lua-nginx-module/commits/v0.10.21)
- 建议先了解 Luajit 相关函数的功能，参考示例：[ngx_http_lua_inject_arg_api.md](../../Luajit/Examples/ngx_http_lua_inject_arg_api.md)

# 核心技术点

- [核心技术点汇总]()

# 参考

- 源码：https://github.com/openresty/lua-nginx-module
- https://github.com/LomoX-Offical/openresty-source-code-analysis/blob/master/lua-nginx-module
- https://github.com/tokers/lua-nginx-module-digest
- http://wangzhangtao.com/2020/12/19/%E7%AC%AC%E5%85%AD%E7%AB%A0%EF%BC%9A%E4%BB%8E%E6%BA%90%E7%A0%81%E8%A7%86%E8%A7%92%E6%B7%B1%E5%85%A5%E4%BD%BF%E7%94%A8nginx%E4%B8%8Eopenresty-18%E8%AE%B2/
- https://catbro666.github.io/categories/%E7%BC%96%E7%A8%8B%E5%BC%80%E5%8F%91/Nginx-OpenResty/
- Lua 中文手册：https://cloudwu.github.io/lua53doc/manual.html
- Lua 英文手册：https://www.lua.org/manual/5.1/manual.html
- https://www.kancloud.cn/digest/understandingnginx/202593
