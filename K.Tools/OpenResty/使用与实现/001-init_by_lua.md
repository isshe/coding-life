
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
