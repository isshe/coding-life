local socket = require("socket")
local js = require("cjson.safe")


local my_ip = arg[1]
local other_server_ip = arg[2]
local my_port = tonumber(arg[3]) or 3478
local other_server_port = tonumber(arg[4]) or 3478
local my_port2 = 3479


local reveive_times = 100
local DEBUG = true

local udp1, udp2

local function sleep(n)
    os.execute("sleep " .. tonumber(n))
end

local function wait_for_data(udp)
    local msg, ip, port = udp:receivefrom()
    local udp_ip, udp_port = udp:getsockname()
    local _ = DEBUG and print(string.format("%s:%s receivefrom: %s:%s: %s", udp_ip or "", udp_port or "", ip, port, msg))
    local data = js.decode(msg)
    data.recv_ip = ip
    data.recv_port = port
    data.ext_map_ip = data.ext_map_ip or ip
    data.ext_map_port = data.ext_map_port or port
    return data
end

local function send_to(udp, data, peer_ip, peer_port)
    local res, err = udp:sendto(data, peer_ip, peer_port)
    local udp_ip, udp_port = udp:getsockname()
    local _ = DEBUG and print(string.format("%s:%s send to %s:%s: %s", udp_ip, udp_port, peer_ip, peer_port, data))
    if not res then
        print("send to failed：", err or "")
    end
end

local function reply_with_diffrent_port(udp, data)
    local _ = DEBUG and print("----reply_with_diffrent_port----")

    local recv_ip = data.recv_ip
    local recv_port = data.recv_port

    local send_msg = {
        ext_map_ip = data.ext_map_ip,
        ext_map_port = data.ext_map_port,
    }
    local reply_udp = udp == udp1 and udp2 or udp1
    send_to(reply_udp, js.encode(send_msg), recv_ip, recv_port)
end

local function forward_to_other_server(udp, data, other_port)
    local _ = DEBUG and print("----forward_to_other_server----")
    local send_msg = {
        ext_map_ip = data.ext_map_ip,
        ext_map_port = data.ext_map_port,
        reply_ip = data.ext_map_ip,
        reply_port = data.ext_map_port,
        other_port = other_port,
    }

    send_to(udp, js.encode(send_msg), other_server_ip, other_server_port)
end

local function reply_to_specify_ip_with_diff_port(udp, data, other_port)
    local _ = DEBUG and print("----reply_to_specify_ip_with_diff_port----")
    local send_msg = {
        ext_map_ip = data.ext_map_ip,
        ext_map_port = data.ext_map_port,
    }
    if other_port then
        local reply_udp = udp == udp1 and udp2 or udp1
        send_to(reply_udp, js.encode(send_msg), data.reply_ip, data.reply_port)
    else
        send_to(udp, js.encode(send_msg), data.reply_ip, data.reply_port)
    end
end

local function reply_to_peer(udp, data)
    local _ = DEBUG and print("----reply_to_peer----")
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
    if data.other_ip then
        forward_to_other_server(udp, data, data.other_port)
        return true
    end

    if data.reply_ip and data.reply_port then
        reply_to_specify_ip_with_diff_port(udp, data, data.other_port)
        return true
    end

    if data.other_port then
        reply_with_diffrent_port(udp, data)
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
    if not (my_ip and other_server_ip) then
        print("Usage: lua ./stun_server.lua <server_ip1/server_ip2> <server_ip2/server_ip1>")
        return
    end

    udp1 = socket.udp()
    local res, err = udp1:setsockname(my_ip, my_port)
    local _ = res or print(err or "")
    local _ = DEBUG and print("Listen in: ", udp1:getsockname())

    udp2 = socket.udp()
    local res, err = udp2:setsockname(my_ip, my_port2)
    local _ = res or print(err or "")
    local _ = DEBUG and print("Listen in: ", udp2:getsockname())
    for i = 1, reveive_times, 1 do
        socket_select(udp1, udp2)
    end
    udp1:close()
    udp2:close()
end

main()