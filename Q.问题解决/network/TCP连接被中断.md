# TCP 连接被中断

客户在使用我们的网关时，发现 wss 握手失败。
解决方式感觉可以拓展到其他类似的网络问题，因此记录一下。

## 排查步骤

**最终目标是定位到问题，因此需要始终记得目标，逐步缩小范围。**
**容易确认的内容优先确认。**

1. 检查客户的配置，确认客户配置正确。
2. 本地尝试复现，无法复现。
3. 客户环境抓包，发现连接确实被网关中断了，同时发送了 FIN 给上下游。

    ```bash
    tcpdump -i eth0 port 443 and port 80 -v -w tcp.pcap
    ```

4. 通过 FIN 知道 socket 被 close，接着排查是哪个代码导致的 close。
   1. 先看 socket 怎么被 close 的，把探针打在 close 上，用 `_sym_ubt(_ubt())`(OpenResty ylang) 打印堆栈，没发现异常
   2. 看看 request 或 upstream 被关闭时的状态，打印各个标志位 —— 把探针打在 ngx_http_finalize_request 等关键/可疑函数上，然后打印请求的标记位。**发现 r->connection->read->eof 标记被设置了，也就是读下游数据结束。**（如果我们不使用 OpenResty Xray 之类的动态追踪工具，可以使用传统打 Log 的方法 —— 源码中加入打 Log 代码，重新执行程序，不过就只能在测试环境使用了。）

        ```log
        -------ngx_http_finalize_request-------
        0x5568e41ddcb0 342 /LANext.Dispatch.API/DispatchHub
        dc 0x7fd9a3ed0310 timeoutd:0 error 0  r eof: 1, wtimeout 0, rtimeout 0
        uc 0x7fd9a3ed0410 timedout：0, error:0 r eof: 0, wtimeout 0, rtimeout 0
        --------------
        ```

   3. 接着排查这个标志位什么时候被设置上去的。用 openresty xray 就在各个关键节点的函数增加打印。（如果不使用 OpenResty Xray 可以使用 gdb 的 watch，同样要求有调试信息并是非正式环境），定位到 `ngx_http_ssl_handshake(ngx_event_t *rev)`：

        ```
        ~~~ 24
        eof:1
        5ecb2: ngx_unix_recv[2c]
        8a148: ngx_http_ssl_handshake[2c]
        68bae: ngx_epoll_process_events.lto_priv.0[2c]
        63c3e: ngx_process_events_and_timers[2c]
        6a4a6: ngx_worker_process_cycle[2c]
        60a12: ngx_spawn_process[2c]
        61478: ngx_start_worker_processes[2c]
        691c5: ngx_master_process_cycle[2c]
        3c1d3: main[2c]
        29d90: __libc_start_call_main[8]
        29e40: __libc_start_main@GLIBC_2.2.5[8]
        3c215: _start[2c]
        ```

   4. 仔细排查后，发现是 c->recv（这个在 nginx 中最终调用 ngx_unix_recv 等平台相关函数）传入 0 时，导致 C 库的 recv 返回 0，进而设置了 eof = 1：

        ```c
        // ngx_http_ssl_handshake
        if (c->recv(c, buf, size) != (ssize_t) size) {
            ngx_http_close_connection(c);
            return;
        }
        ```

        ```c
        // ngx_unix_recv
        n = recv(c->fd, buf, size, 0);
        if (n == 0) {
            rev->ready = 0;
            rev->eof = 1;
            // ...
        }
        ```
