-- 迷宫游戏，goto实现
-- 8.4中是函数实现

goto room1

::room1:: do
    print("in room1, you can go south/east: ")
    local move = io.read()
    if move == "south" then goto room3
    elseif move == "east" then goto room2
    else
        print("Invalid move")
        goto room1
    end
end

::room2:: do
    print("in room2, you can go south/west: ")
    local move = io.read()
    if move == "south" then goto room4
    elseif move == "west" then goto room1
    else
        print("Invalid move")
        goto room2
    end
end

::room3:: do
    print("in room3, you can go north/east: ")
    local move = io.read()
    if move == "north" then goto room1
    elseif move == "east" then goto room4
    else
        print("Invalid move")
        goto room3
    end
end

::room4:: do
    print("Congratulations, you won!")
end