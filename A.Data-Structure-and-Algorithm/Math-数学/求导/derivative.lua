-- 求导。详见《Lua程序设计 第4版》94页

function derivative(f, delta)
    delta = delta or 1e-4
    return function (x)
        return (f(x + delta) - f(x)) / delta
    end
end

function test1()
    c = derivative(math.sin)
    print(math.cos(5.2), c(5.2))
    print(math.cos(10), c(10))
end

test1()