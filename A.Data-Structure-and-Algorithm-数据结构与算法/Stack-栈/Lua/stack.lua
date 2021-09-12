-- 栈、双端队列

local stack = {}

-- 基础类方法 new
function stack:new ()
    local obj = {
        data = {},
    }
    local mt = {
        __index = self
    }
    setmetatable(obj, mt)
    return obj
end

function stack:length()
    return #self.data
end

function stack:empty()
    if not next(self.data) then
        return true
    end

    return false
end

function stack:push(data)
    table.insert(self.data, data)
end

function stack:push_front(data)
    table.insert(self.data, 1, data)
end

function stack:pop()
    return table.remove(self.data)
end

function stack:pop_front()
    return table.remove(self.data, 1)
end

function stack:top()
    return self.data[stack:length()]
end

return stack