[TOC]

# 记录一些常用的操作

## 获取文件名
```lua
local function filename_get(path)
    return path:match( "([^/]+)/?$" )
end
```

## 获取路径
```lua
local function filepath_get(path)
    return path:match("^(.+)/.+/?$")
end
```
