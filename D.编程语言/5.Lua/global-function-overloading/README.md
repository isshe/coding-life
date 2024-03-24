
全局函数重载
---

在我个人的开源项目 [xway](https://github.com/x-few/xway) 的中有一个需求：允许用户写 Lua 来自定义一些模块，提供最大的灵活性，但是不把 Lua 模块保存成文件。要如何实现呢？怎么才能从内存中 require 一个模块？答案是重载 require 函数。
利用 Lua 的环境（Environment）特性，在某个独立的环境中重载 require 函数。同时独立的环境也允许我们进行一些限制，例如每个用户只能访问自己的资源。不过，xway 项目计划是不支持多租户，所以限制相关的实现，还未进行相关探索及验证。

下面直接通过一个例子来说明。

# 示例

```lua
local ctype_count = require "ctype.count"

local ma = [[
local _M = {}
function _M.hello()
    print("a hello")
end

return _M
]]

local mb = [[
local ma = require("ma")
local _M = {}

function _M.hello()
    ma.hello()
    print("b hello")
end

return _M
]]

local module_map = {
    ["ma"] = ma,
    ["mb"] = mb,
}

local function new_require(name)
    return loadstring(module_map[name])()
end

local new_env ={
    require = new_require,
}
setmetatable(new_env, {__index = _G})

local m1 = loadstring(mb)

setfenv(m1, new_env)
print(ctype_count())
local ret, mod = pcall(m1)
if not ret then
    print(ret)
end
print(ctype_count())

mod.hello()
```

* `ma` 和 `mb` 是两个用户自定义模块，形式是存于内存中的字符串。
* `new_require` 是我们重载的新的 require 函数，按我们的需要来编写。
* `new_env` 是一个继承于全局环境 (_G) 的新环境，用于执行自定义代码。
* `setfenv` 设置函数的环境（mb 模块被 loadstring 成函数了）
* `ctype.count` 模块的作用是用于区分是否调用了 ffi 模块，当加载了 ffi 模块时，ctype.count 将增加，因此可以通过判断 pcall 前后 ctype.count 是否变化来判断使用调用了 ffi 模块。
