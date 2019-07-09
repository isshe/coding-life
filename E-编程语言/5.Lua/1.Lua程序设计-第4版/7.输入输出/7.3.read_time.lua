
local function read_and_write(input_file, output_file, size)
    local ifd, err1 = io.open(input_file, "r")
    local ofd, err2 = io.open(output_file, "w")
    if not (ifd and ofd) then
        io.write("打开输入/输出文件出错: ", err1 or err2, "\n")
        os.exit()
    end
    local i = 0
    repeat
        local data = ifd:read(size)
        if data then
            ofd:write(data)
        end
        i = i + 1
        print(i)
        if size == "a" then
            io.write("len(data) = ", string.len(data), "\n")
            break
        end
    until not data
end

local function read_time(input_file, output_file)
    if not (input_file and output_file) then
        io.write("无效的输入/输出文件\n")
        os.exit()
    end
    io.write("1.按字节 2.按行 3.按块(4KB) 4.按块(8KB) 5.一次性, 请选择：")
    local select = io.read(1)
    local time1 = os.time()
    if select == "1" then
        read_and_write(input_file, output_file, 1)
    elseif select == "2" then
        print("select == \"2\"")
        read_and_write(input_file, output_file, "l")
    elseif select == "3" then
        read_and_write(input_file, output_file, 2^12)
    elseif select == "4" then
        read_and_write(input_file, output_file, 2^13)
    elseif select == "5" then
        read_and_write(input_file, output_file, "a")
    else
        io.write("无效的输入\n")
    end
    local time2 = os.time()
    local str = string.format("%s - %s = %s\n", time2, time1, time2 - time1)
    io.write(str)
end

local output_file = "/dev/null"
local input_file = ...
if not input_file then
    io.write("Usage: lua xxx.lua <input_file>\n")
    os.exit()
end
--local output_file = input_file .. ".out"
read_time(input_file, output_file)