Nginx 模块数据处理
---

前面一篇文章中，我们解析了 Nginx 模块的初始化流程，知道了 Nginx 是如何对各个模块就行初始化的。

而接下来，这篇文章的目的是解答以下问题：

- **Nginx 是如何知道从端口接收到的数据，应该交给哪些模块进行处理的呢？**

# listen

我们都知道，通常 Nginx 的数据/请求是通过端口接收过来的，因此，我们来看看：

- **Nginx 是如何进行端口监听的？端口监听时做了些什么？**

通过搜索源码，我们得到了相关 command：

> src/http/ngx_http_core_module.c

```c
    { ngx_string("listen"),
      NGX_HTTP_SRV_CONF|NGX_CONF_1MORE,
      ngx_http_core_listen,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },
```

> src/mail/ngx_mail_core_module.c

```c
    { ngx_string("listen"),
      NGX_MAIL_SRV_CONF|NGX_CONF_1MORE,
      ngx_mail_core_listen,
      NGX_MAIL_SRV_CONF_OFFSET,
      0,
      NULL },
```

> src/stream/ngx_stream_core_module.c

```c
    { ngx_string("listen"),
      NGX_STREAM_SRV_CONF|NGX_CONF_1MORE,
      ngx_stream_core_listen,
      NGX_STREAM_SRV_CONF_OFFSET,
      0,
      NULL },
```

可以看到，各个类型，都有自己的 listen 指令，因此 nginx.conf 中，不同配置块（block）下的 listen 指令是不同。

我们以 HTTP 为例，看下调用流程。

## HTTP 模块中 listen 的调用流程

为便于理解，先插播一条 listen 指令的用法：

[listen](../Nginx指令/http-listen.md)


### 调用流程：端口监听

```
- ngx_init_cycle: core/ngx_cycle.c
    \ - ngx_conf_parse
        \ - ngx_http_block
            \ - ngx_conf_parse
                \ - ngx_http_core_listen: src/http/ngx_http_core_module.c
                    \ - ngx_parse_url: src/core/ngx_inet.c，解析 listen 指令后的第一部分（URL），先检查是否是 unix domain 类型，再检查是否是 IPv6 类型，都不是就调用 IPv4 的 URL 解析。
                    \ - ngx_strcmp: 解析指令的各个部分。
                    \ - ngx_inet_wildcard: src/core/ngx_inet.c，判断地址是否是通配地址（如 IPv4 的 INADDR_ANY）。
                    \ - ngx_http_add_listen:
                        \ - ngx_http_add_addresses:
                            \ - ngx_cmp_sockaddr: src/core/ngx_inet.c，对比当前列表中的地址与即将添加的地址。
                            \ - ngx_http_add_server: 将 server（虚拟主机） 添加到 address:port 对应的虚拟主机列表中（如果地址已经存在在当前列表中，则进行次操作，否则进行 ngx_http_add_address）
                            \ - ngx_http_add_address: 将 server 地址、server 名、server 对应类型的核心模块配置添加到端口列表。
                                \ - ngx_http_add_server: 将 server 对应类型的核心模块配置添加到 address:port
                        \ - ngx_http_add_address: 将 server 地址、server 名、server 对应类型的核心模块配置添加到端口列表。
                            \ - ngx_http_add_server: 将 server 对应类型的核心模块配置添加到 address:port
            \ - ngx_http_optimize_servers: 优化 ports、 addresses、 server names 列表。
                \ - ngx_http_server_names
                \ - ngx_http_init_listening
                    \ - ngx_http_add_listening
                        \ - ngx_create_listening: 把 cmcf->ports 中的 address 信息复制到 cycle->listening 中。
                            \ - ngx_array_push: 调用此函数把地址添加到列表
    \ - ngx_open_listening_sockets: 遍历 cycle->listening 进行实际的监听
        \ - setsockopt
        \ - ngx_socket
        \ - bind
        \ - listen

```

以上是监听的流程，相关操作是在 master 进程中，然后 fork 以后，worker 进程也得到了相关套接字信息。
worker 进程调用 ngx_worker_process_cycle 进入自身循环。
listen 之后，就能 accept 了，那么是如何处理 accept 的呢？

### 调用流程：event 初始化

```
- ngx_master_process_cycle: 起工作进程、cache 管理进程等，然后进入循环
    \ - sigemptyset: 清空信号集
    \ - sigaddset: 添加信号到信号集
    \ - sigprocmask: 屏蔽（SIG_BLOCK）信号集
    \ - sigemptyset: 清空信号集——只是清空变量不会实际影响到进程信号屏蔽字
    \ - ngx_setproctitle: 设置进程标题
    \ - ngx_get_conf: 获取核心模块配置
    \ - ngx_start_worker_processes: 启动工作进程
        \ - ngx_spawn_process: 生成进程
            \ - fork
            \ - proc = ngx_worker_process_cycle: 子进程调用此函数
                \ - ngx_worker_process_init

        \ - ngx_pass_open_channel
    \ - ngx_start_cache_manager_processes: 启动 cache 管理进程
    \ - sigsuspend: 进入循环，开放信号，挂起进程等待信号
```

### 调用流程：请求处理

```

```




# 参考

- [How nginx processes a request](http://nginx.org/en/docs/http/request_processing.html)
