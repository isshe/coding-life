-- 迷宫游戏，函数实现
-- 8.2中是goto实现

function room1()
    print("in room1, you can go south/east: ")
    local move = io.read()
    if move == "south" then room3()
    elseif move == "east" then room2()
    else
        print("Invalid move")
        room1()
    end
end

function room2()
    print("in room2, you can go south/west: ")
    local move = io.read()
    if move == "south" then room4()
    elseif move == "west" then room1()
    else
        print("Invalid move")
        room2()
    end
end

function room3()
    print("in room3, you can go north/east: ")
    local move = io.read()
    if move == "north" then room1()
    elseif move == "east" then room4()
    else
        print("Invalid move")
        room3()
    end
end

function room4()
    print("Congratulations, you won!")
end

room1()