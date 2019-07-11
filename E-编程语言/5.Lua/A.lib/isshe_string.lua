

local function trim_gsub(s)
    --return string.match(s, "^%s*(.-)%s*$")
    --return string.gsub(s, "^%s*(.-)%s*$", "%1")
    --s = string.gsub(s, "^%s*(%S?.*)", "%1")
    --return string.gsub(s, "(.-%S?)%s*$", "%1")
    s = string.gsub(s, "^%s*(%S.*)", "%1")
    s = s and string.gsub(s, "(.*%S)%s*$", "%1")
    return s
end

-- ok的！
local function trim_match(s)
    s = string.match(s, "^%s*(%S.*)")
    s = s and string.match(s, "(.*%S)%s*$") or ""   -- 如果全是空白字符，则不会运行这个match
    return s
end

local function do_test(str)
    local t1 = os.clock()
    local res = trim_gsub(str)
    local t2 = os.clock()
    print(t2 - t1, string.len(res))
end

local function test()

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
end

test()