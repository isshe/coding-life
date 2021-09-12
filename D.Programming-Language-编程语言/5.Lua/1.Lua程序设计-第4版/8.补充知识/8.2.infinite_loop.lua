-- 第一种
while true do

end

-- 第二种
repeat
    print("abc")
until false

-- 第三种
for i = 1, math.huge do

end

-- 第四种
local function abc()
    return function()
        return true
    end
end

for k, v in abc() do

end

