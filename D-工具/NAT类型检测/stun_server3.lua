local socket = require("socket")
local js = require("cjson.safe")


local my_ip = arg[1] or "192.168.2.192"
local my_port = tonumber(arg[2]) or 3478
local my_port2 = 3479
local other_server_ip = arg[3] or "192.168.2.139"
local other_server_port = tonumber(arg[4]) or 3478

local reveive_times = 100
local DEBUG = true

local function sleep(n)
    os.execute("sleep " .. tonumber(n))
end

local function wait_for_data(udp)
    local msg, ip, port = udp:receivefrom()
    print(string.format("receivefrom: %s:%s: %s", ip, port, msg))
    local data = js.decode(msg)
    data.recv_ip = ip
    data.recv_port = port
    data.ext_map_ip = data.ext_map_ip or ip
    data.ext_map_port = data.ext_map_port or port
    return data
end

local function send_to(udp, data, peer_ip, peer_port)
    local res, err = udp:sendto(data, peer_ip, peer_port)
    print(string.format("send to %s:%s: %s", peer_ip, peer_port, data))
    if not res then
        print("send to failed：", err or "")
    end
end

local function reply_with_diffrent_port(udp, data)
    local recv_ip = data.recv_ip
    local recv_port = data.recv_port

    local send_msg = {
        ext_map_ip = data.ext_map_ip,
        ext_map_port = data.ext_map_port,
    }
    local new_udp = socket.udp()
    new_udp:setsockname(my_ip, my_port2)
    send_to(new_udp, js.encode(send_msg), recv_ip, recv_port)
    new_udp:close()
end

local function forward_to_other_server(udp, data)
    local send_msg = {
        ext_map_ip = data.ext_map_ip,
        ext_map_port = data.ext_map_port,
        reply_ip = data.ext_map_ip,
        reply_port = data.ext_map_port,
    }

    send_to(udp, js.encode(send_msg), other_server_ip, other_server_port)
end

local function reply_to_specify_ip_with_diff_port(udp, data)
    local send_msg = {
        ext_map_ip = data.ext_map_ip,
        ext_map_port = data.ext_map_port,
    }
    local new_udp = socket.udp()
    new_udp:setsockname(my_ip, my_port2)
    send_to(new_udp, js.encode(send_msg), data.reply_ip, data.reply_port)
    new_udp:close()
end

local function reply_to_peer(udp, data)
    local recv_ip = data.recv_ip
    local recv_port = data.recv_port

    local send_msg = {
        ext_map_ip = data.ext_map_ip,
        ext_map_port = data.ext_map_port,
    }
    send_to(udp, js.encode(send_msg), recv_ip, recv_port)
end


local function recv_and_parse(udp)
    local data = wait_for_data(udp)
    --sleep(1)
    if data.other_ip and data.other_port then
        forward_to_other_server(udp, data)
        return true
    end

    if data.other_port then
        reply_with_diffrent_port(udp, data)
        return true
    end

    if data.reply_ip and data.reply_port then
        reply_to_specify_ip_with_diff_port(udp, data)
        return true
    end

    reply_to_peer(udp, data)
    return true
end

local function socket_select(udp, udp2)
    local watch_recv_tab = {udp, udp2}
    local watch_send_tab = {}
    local recved_tab, send_tab, status = socket.select(watch_recv_tab, watch_send_tab)
    for _, v in ipairs(recved_tab or {}) do
        recv_and_parse(v)
    end
end

local function main()
    local udp = socket.udp()
    udp:setsockname(my_ip, my_port)
    print("Listen in: ", udp:getsockname())

    local udp2 = socket.udp()
    udp2:setsockname(my_ip, my_port2)
    print("Listen in: ", udp2:getsockname())
    for i = 1, reveive_times, 1 do
        socket_select(udp, udp2)
        --recv_and_parse(udp)
    end
    udp:close()
    udp2:close()
end

main()