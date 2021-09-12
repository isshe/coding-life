
local queue = {}
-- 基础类方法 new

function queue:new ()
    local obj = {
        data = {},
    }
    local mt = {
        __index = self
    }
    setmetatable(obj, mt)
    return obj
end

function queue:enque(data)
    table.insert(self.data, data)
end

function queue:deque()
    local k, v = next(self.data)
    table.remove(self.data, k)
    return v
end

function queue:first()
    local _, v = next(self.data)
    return v
end

function queue:last()
    local len = self:lenght()
    return self.data[len]
end

function queue:empty()
    if not next(self.data) then
        return true
    end

    return false
end

function queue:lenght()
    return #self.data
end

return queue