
local function seek_last_n_lines(fd, n)
    -- 跳到文件尾部
    fd:seek("end", -1)  -- 指向最后一个字符

    for i = 1, n, 1 do
        local cur
        repeat
            fd:seek("cur", -1)
            local ch = fd:read(1)   -- 读会移动文件指针
            fd:seek("cur", -1)      -- 恢复读文件是移动的文件指针
            cur = fd:seek()
            --print(ch, string.byte(ch), cur)
        until ch == '\n' or cur == 0

        if cur and cur == 0 then
            --print(cur)
            break
        end
        if i == n then
            fd:seek("cur", 2)   -- 多了前一行的`x\n`这两个字符，因此去掉
        end
    end
end

local function print_last_n_lines(fd)
    for line in fd:lines() do
        io.write(line, "\n")
    end
end

local function main(...)
    local input_file, n = ...
    n = tonumber(n)
    if not (input_file and n) then
        io.write("Usage: lua xxx.lua <input_file> <num (last_n_line)>\n")
        os.exit()
    end
    local fd, err = io.open(input_file)
    if not fd then
        io.write("io.open error: ", err or "", "\n")
        os.exit()
    end

    seek_last_n_lines(fd, n)
    print_last_n_lines(fd)
end

main(...)