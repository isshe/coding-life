
# Lua

## 1. 变长参数
### 1.1 遍历
```lua
function xxx(...)
    for k, v in ipairs({...}) do
        -- do something
    end
 end
```

### 1.2 遍历——存在`nil`的情况
```lua
function xxx(...)
    local arg = table.pack(...)
    for i = 1, arg.n do
        -- do something
    end
 end
```

### 1.3 遍历——`select`
```lua
function xxx(...)
    for i = 1, select("#", ...) do
        x = select(i, ...)
        -- do something
    end
 end
```

## 2. table.pack和table.unpack
打包成table或者把table拆了。

## 3. 安全操作
```lua
zip = a and a.b and a.b.c and a.b.c.d
```
可替换为：
```lua
E = {}
zip = (((a or E).b or E).c or E).d
```

## 4.
