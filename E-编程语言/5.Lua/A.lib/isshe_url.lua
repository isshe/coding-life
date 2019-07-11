
-- 转码到URL。注意特殊字符"="等
local function escape(s)
    local str = string.gsub(s, "[&=+%%%c]", function(c)
            return string.format("%%%02X", string.byte(c))
    end)
    str = string.gsub(str, " ", "+")
    return str
end

-- 从URL转码
local function unescape(s)
    local str = string.gsub(s, "+", " ")
    str = string.gsub(str, "%%(%x%x)", function(h)
            return string.char(tonumber(h, 16))
    end)

    return str
end

-- 解码参数
local function decode(s)
    local cgi = {}
    for k, v in string.gmatch(s, "([^&=]+)=([^&=]+)") do
        k = unescape(k)
        v = unescape(v)
        cgi[k] = v
    end

    return cgi
end

-- 编码参数
local function encode(t)
    local b = {}
    for k, v in pairs(t) do
        b[#b + 1] = string.format("%s=%s", escape(k), escape(v))
    end

    return table.concat(b, "&")
end

local function test()
    local js = require("cjson.safe")

    local s = "name=al&query=a%2Bb+%3D+c&q=yes+or+no"
    local ns = unescape(s)
    local ens = escape(ns)
    io.write("原字符串：", s, "\n")
    io.write("解码后 = ", ns, "\n")
    io.write("解码后再编码 = ", ens, "\n")
    io.write(s, " != ", ens, "\n")
    local t = decode(s)
    print(js.encode(t))
    print(encode(t))
end

test()

return {
    decode = decode,
    encode = encode,
}