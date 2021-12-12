

-- trim 去除字符串前后的空白字符
local function trim(s)
    s = string.match(s, "^%s*(%S.*)")
    s = s and string.match(s, "(.*%S)%s*$") or ""   -- 如果全是空白字符，则不会运行这个match
    return s
end

local function split(s, p)
    local t = {}

    if s == "" or p == "" then
        return t
    end

    local pt = string.format("[^%s]+", p)
    string.gsub(s, pt, function(w)
            table.insert(t, w)
    end)

    return t
end

local function utf8_reverse(s)
    s = s:gsub(utf8.charpattern, function (c)
        return #c > 1 and c:reverse()   -- 翻转单个utf8字符
    end)

    return s:reverse()  -- 整个字符串翻转
end

local function do_test(str)
    local t1 = os.clock()
    local res = trim(str)
    local t2 = os.clock()
    print(t2 - t1, string.len(res), string.len(str))
end

local function test()
    local js = require("cjson.safe")
    --[[
    local str = string.rep(" ", 1024*1024*100)
    do_test(str)

    local str = string.rep("a", 1024*1024*100)
    do_test(str)

    str = string.rep(" ", 1024*1024*50) ..string.rep("a", 1) ..string.rep(" ", 1024*1024*50)
    do_test(str)

    str = string.rep("a", 1024*1024*50) ..string.rep(" ", 1) ..string.rep("a", 1024*1024*50)
    do_test(str)

    str = string.rep("a", 1) .. string.rep(" ", 1024*50) .. string.rep("a", 1)
    do_test(str)
]]

    local str = "a b c d e f g h i "
    local t = split(str, " ")
    print(js.encode(t))

    local str = ""
    local t = split(str, "")
    print(js.encode(t))

    local str = "中国"
    for p, c in utf8.codes(str) do
        print(p, c)
    end
    print(utf8_reverse(str))
    print(utf8.codepoint(str, 4))
    print(utf8.offset(str, 2))

end

test()

return {
    trim = trim,
    split = split,
    utf8_reverse = utf8_reverse,
}