local function input_sort_output(...)
    local input_file, output_file = ...
    local lines = {}

    for line in io.lines(input_file) do
        table.insert(lines, line)
    end

    table.sort(lines)

    local ofd = io.stdout
    if output_file then
        local temp_ofd = io.open(output_file, "r")
        if temp_ofd then
            -- 文件存在
            local res
            repeat
                io.write("确定要覆盖输出文件吗？(Y/N) ")
                res = io.read(1)
            until (res == "Y" or res == "N")
            if res == "Y" then
                io.close(temp_ofd)
                ofd = io.open(output_file, "w")
            end
        else
            ofd = io.open(output_file, "w+")
        end
    end

    for _, line in ipairs(lines) do
        ofd:write(line, "\n")
    end
end

input_sort_output(...)