
local function fsize(fd)
    local current_pos = fd:seek()   -- 保存当前位置
    local size = fd:seek("end")     -- 获取文件大小
    fd:seek("set", current_pos)     -- 恢复当前位置
    return size
end

return {
    fsize = fsize,
}