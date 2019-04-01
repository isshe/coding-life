
local socket = require("socket")
local js = require("cjson.safe")


local client_ip = "172.16.4.186"
--local client_ip = "192.168.2.33"
local client_port1 = 0
local server_ip = "192.168.2.192" -- "47.244.5.179" --
local server_ip2 = "192.168.2.139" -- "45.32.82.86" --
local server_port = 3478
local server_port2 = 3479


local DEBUG = true
local receive_timeout = 3
local receive_retry_times = 3

local function send_and_recv(udp, peer_ip, peer_port, datagram)
    if DEBUG then
        print(string.format("->: send to %s:%s: %s", peer_ip, peer_port, datagram))
    end

    --udp:setpeername(peer_ip, peer_port)
    --print(udp:getpeername())
    --local res, err = udp:send(datagram)
    local res, err = udp:sendto(datagram, peer_ip, peer_port)
    if not res then
        local err_str = string.format("send to %s:%s failed", peer_ip, peer_port)
        return false, err or err_str
    end
    --udp:setpeername("*")

    udp:settimeout(receive_timeout)
    for i = 1, receive_retry_times, 1 do
        local msg, ip, port = udp:receivefrom()
        --local msg, ip, port = udp:receive()
        if msg and ip and port then
            if DEBUG then
                print(string.format("<-: receive from %s:%s: %s", ip, port, msg))
            end
            return msg
        end
    end
    --udp:setpeername("*")

    return false, "receivefrom timeout or failed!"
end

local function print_ext_info(ip1, port1, ip2, port2)
    local ext_info = string.format("your ext info: \n\tip:port = %s:%s\n", ip1, port1)
    if ip2 and port2 then
        ext_info = string.format("%s\tip:port = %s:%s\n", ext_info, ip2, port2)
    end
    print(ext_info)
end


local function has_nat(ext_map_ip)
    if ext_map_ip == client_ip then
        return false
    end

    return true
end

-- Request echo from same address, same port
local function test1(udp, peer_ip, peer_port)
    print("do test1...")
    local send_info = {}
    local recv_msg, err = send_and_recv(udp, peer_ip, peer_port, js.encode(send_info))
    local recv_msg_tab = js.decode(recv_msg)
    if not (recv_msg and recv_msg_tab) then
        return false, err or "send and recv failed, exiting.."
    end
    return recv_msg_tab
end

-- Request echo from different address, different port
local function test2(udp, peer_ip, peer_port)
    print("do test2...")
    local send_info = {
        other_ip = true,
        other_port = true,
    }
    local recv_msg, err = send_and_recv(udp, peer_ip, peer_port, js.encode(send_info))
    local recv_msg_tab = js.decode(recv_msg)
    if not (recv_msg and recv_msg_tab) then
        return false, err or "send and recv failed, exiting.."
    end
    return recv_msg_tab
end

-- Request echo from same address, different port
local function test3(udp, peer_ip, peer_port)
    print("do test3...")
    local send_info = {
        other_port = true,
    }
    local recv_msg, err = send_and_recv(udp, peer_ip, peer_port, js.encode(send_info))
    local recv_msg_tab = js.decode(recv_msg)
    if not (recv_msg and recv_msg_tab) then
        return false, err or "send and recv failed, exiting.."
    end
    return recv_msg_tab
end

local function nat_type_get(udp, server_ip, server_port)
    local t1_received, err = test1(udp, server_ip, server_port)
    if not t1_received then
        return "UDP blocked"
    end

    print_ext_info(t1_received.ext_map_ip, t1_received.ext_map_port)

    if not has_nat(t1_received) then
        local t2_received, err = test2(udp, server_ip, server_port)
        if t2_received then
            return "Open Internat"
        end
        return "Symetric Firewall"
    end

    local t2_received, err = test2(udp, server_ip, server_port)
    if t2_received then
        return "Full cone NAT"
    end

    local t1_received2, err = test1(udp, server_ip2, server_port2)
    if not t1_received2 then
        return err or "error: test1 failed!"
    end

    if t1_received.ext_map_ip == t1_received2.ext_map_ip
            and t1_received.ext_map_port == t1_received2.ext_map_port then
        local t3_received, err = test3(udp, server_ip, server_port)
        if t3_received then
            return "Addr Restricted cone NAT"
        end

        return "Port Restricted cone NAT"
    end

    return "Symmetric NAT"

end


local function main()
    local udp = socket.udp()
    udp:setsockname(client_ip, client_port1)
    print("your int1 info: ", udp:getsockname())
    local type = nat_type_get(udp, server_ip, server_port)
    print(string.format("TYPE: %s", type))
    udp:close()
end

main()