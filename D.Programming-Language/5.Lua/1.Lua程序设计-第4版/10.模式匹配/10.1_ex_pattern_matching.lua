
s = "hello world"
print("find: ", string.find(s, "hello"))
print("find2: ")

local status, err = pcall(string.find,"a [word]", "[")
if status then
    print("find2 successed")
else
    print("find2 error: ", err)
end
print("find3: ", string.find("a [word]", "[", 1, true))


local i, j = string.find(s, "hello")
print("sub:", string.sub(s, i, j))

print("match: ", string.match(s, "hello"))

print("gsub: ", string.gsub("Lua is cute", "cute", "great"))
print("gsub2: ", string.gsub("all lii", "l", "x", 2))

print("gmatch: ")
s = "some string"
for w in string.gmatch(s, "%a+") do
    print(w)
end