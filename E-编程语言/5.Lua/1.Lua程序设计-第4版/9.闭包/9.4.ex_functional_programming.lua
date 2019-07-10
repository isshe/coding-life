-- 创建一个(x, y)为中心，r为半径的圆盘
function disk(cx, cy, r)
    return function(x, y)
        return (x - cx)^2 + (y - cy)^2 <= r^2
    end
end

-- 创建一个轴对称矩形
function rect(left_x1, right_x2, bottom_y1, up_y2)
    return function(x, y)
        return left_x1 <= x and x <= right_x2 and bottom_y1 <= y and y <= up_y2
    end
end

-- 区域的补集
function complement(r)
    return function(x, y)
        return not r(x, y)
    end
end

-- 区域的并集
function union(r)
    return function(x, y)
        return not r(x, y)
    end
end

-- 区域的交集
function complement(r)
    return function(x, y)
        return not r(x, y)
    end
end

-- 区域的差集
function complement(r)
    return function(x, y)
        return not r(x, y)
    end
end

local disk1 = disk(1.0, 3.0, 5)
print(disk1(1, 2))  -- 点(1,2)是否在disk1中

local rect1 = rect(1, 1, 4, 4)
print(disk1(2, 3))
