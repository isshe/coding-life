# Proxy Cache 的使用与实现

> 基于 nginx-1.24.0
> 编译选项：./configure --prefix=/opt/nginx --with-debug --with-stream --with-http_stub_status_module --with-http_ssl_module

目的：

- 了解如何使用 Proxy Cache。
- 了解 Proxy Cache 是如何实现的。
  - 命中 cache 时是如何处理的。
  - 没命中 cache 时是如何处理的。
  - 如何保存 cache 的。
- keys zone 中存储了哪些信息？如何构建生成的？Nginx 进程启动时，是否会自动构建 key 信息存到 keys zone 中？
  - 当超过 keys zone 不够用了，会怎么处理？

## 使用

```nginx.conf
http {
    # ...
    set $proxy_cache_key    $uri;
    set $proxy_cache_zone   "proxycache";

    # 104857600: 100MB
    # 2592000: 30 天
    # 5368709120: 5GB
    proxy_cache_path /mnt/data/proxy_cache levels=1
        use_temp_path=off
        keys_zone=proxycache:104857600
        inactive=2592000
        max_size=5368709120;

    proxy_cache $proxy_cache_zone;
    proxy_cache_key $proxy_cache_key;
    # ...
}
```

以上是 cache 最基本的使用方式，proxy_cache_path 指令用于定义缓存文件的存储路径等信息，proxy_cache 指令用于指定使用哪个 cache zone。proxy_cache_key 指令则是用于指定 cache key 的生成方式。
可以看到，配置中使用 URI 作为了 cache key。在 OpenResty 中，可以通过 Lua 代码，动态修改 proxy_cache_zone 和 proxy_cache_key 成指定的内容，以达到定制  cache zone 和 cache key 的目的。

## 实现

### 关键调用栈

根据 [《阅读源码的方法》](../../../../M.方法论/阅读源码的方法.md) 中的方法。

通过命令

```bash
# 命令阻塞时，发送 HTTP 请求触发命中断点
bash trace.sh
```

获取关键函数的调用栈：

- 未命中 Cache 时

    ```lua
    ngx_http_file_cache_new at src/http/ngx_http_file_cache.c
    ngx_http_upstream_cache at src/http/ngx_http_upstream.c
    ngx_http_upstream_init_request at src/http/ngx_http_upstream.c
    ngx_http_upstream_init at src/http/ngx_http_upstream.c
    ngx_http_read_client_request_body at src/http/ngx_http_request_body.c
    ngx_http_proxy_handler at src/http/modules/ngx_http_proxy_module.c
    ngx_http_core_content_phase at src/http/ngx_http_core_module.c
    ngx_http_core_run_phases at src/http/ngx_http_core_module.c
    ngx_http_handler at src/http/ngx_http_core_module.c
    ngx_http_process_request at src/http/ngx_http_request.c
    ngx_http_process_request_headers at src/http/ngx_http_request.c
    ngx_http_process_request_line at src/http/ngx_http_request.c
    ngx_epoll_process_events at src/event/modules/ngx_epoll_module.c
    ...
    ```

    调用 `ngx_http_file_cache_new` 后，后续还会继续调用 `ngx_http_file_cache_open`、`ngx_http_file_cache_update`、`ngx_http_file_cache_free` 等，都在 `ngx_http_upstream_cache`。相关函数的作用下面继续探究。

    > 后续看了代码后，发现 `ngx_http_upstream_init` 是作为参数传递给 ngx_http_read_client_request_body 的。

- 命中 Cache 时：

    ```lua
    ngx_http_cache_send at src/http/ngx_http_file_cache.c
    ngx_http_upstream_cache_send at src/http/ngx_http_file_cache.c
    ngx_http_upstream_init_request at src/http/ngx_http_upstream.c
    ngx_http_read_client_request_body at src/http/ngx_http_request_body.c
    ngx_http_upstream_init at src/http/ngx_http_upstream.c
    ngx_http_proxy_handler at src/http/modules/ngx_http_proxy_module.c
    ngx_http_core_content_phase at src/http/ngx_http_core_module.c
    ngx_http_core_run_phases at src/http/ngx_http_core_module.c
    ngx_http_handler at src/http/ngx_http_core_module.c
    ngx_http_process_request at src/http/ngx_http_request.c
    ngx_http_process_request_headers at src/http/ngx_http_request.c
    ngx_http_process_request_line at src/http/ngx_http_request.c
    ngx_epoll_process_events at src/event/modules/ngx_epoll_module.c
    ...
    ```

从上面两个调用栈中可以得到以下信息

- 是在 `content` 阶段处理 Cache 的
- 共同的入口都是 `ngx_http_upstream_init_request` 函数，因此下一步我们主要跟踪这个函数的实现。

### ngx_http_upstream_init_request

> 看函数名称猜测它的功能应该是用于构造上游请求的。

```
- ngx_http_upstream_init_request
    \- rc = ngx_http_upstream_cache(): 进行缓存处理，设置缓存状态等
    \- if (rc == NGX_BUSY): 正忙，下次再进来，通过设置 r->write_event_handler = ngx_http_upstream_init_request 实现
    \- if (rc == NGX_ERROR): 出错了，直接结束请求
        \- ngx_http_finalize_request
    \- if (rc == NGX_OK): 缓存命中了
        \- ngx_http_upstream_cache_send: 直接将缓存作为响应
    \- if (rc != NGX_DECLINED): 非缓存未命中的情况，就直接结束请求
        \- ngx_http_finalize_request
    \- if (u->create_request(r) != NGX_OK): 构建请求
    \- if (ngx_http_upstream_set_local(r, u, u->conf->local) != NGX_OK): TODO?
    \- ...: 中间还有一大堆东西，设置发送、接收回调等
    \- ngx_http_upstream_connect: 发起连接到上游，其中与缓存流程相关的主要有以下几个 handler 的设置
        \- c->write->handler = ngx_http_upstream_handler
        \- c->read->handler = ngx_http_upstream_handler
        \- u->write_event_handler = ngx_http_upstream_send_request_handler
        \- u->read_event_handler = ngx_http_upstream_process_header
```

首先从最外层（ngx_http_upstream_init_request 直接调用）可以看到，与猜测一样，这里是进行上游请求的初始化（创建请求、连接到上游）。
不过更关键的是，会先检查是否有 cache，再进行请求创建，如果有 cache，则会直接使用。

ngx_http_upstream_cache_get 是获取 cache zone。可以是硬编码名称的形式 `proxy_cache proxy_cache_name;`，也可以是变量形式 `proxy_cache $arg_cache;`，当变量的值是 `off` 时不启用缓存。
什么是 predicate（谓词），`set $predicate1 "$arg_param1";` 像这样的配置中的 `$predicate1` 即是
缓存命中后，调用 `ngx_http_upstream_cache_send`。

接下来，我们一层一层解释 ngx_http_upstream_cache、ngx_http_upstream_cache_send 等。

ngx_http_upstream_connect 中设置的各个 handler，我们后续在“保存缓存”的章节进行说明。

## ngx_http_upstream_cache

```
- ngx_http_upstream_cache(): 进行缓存处理
    \- if (r->cache == NULL): 首先检查一下缓存是否存在，这个判断为真表示不存在
        \- if (!(r->method & u->conf->cache_methods)): 检查是否是能缓存的方法，不是直接返回 NGX_DECLINED
        \- ngx_http_upstream_cache_get(r, u, &cache): 选择 cache zone，决定要把 cache 存到哪里去
        \- ngx_http_file_cache_new(r): 创建缓存对象
        \- ngx_http_file_cache_create_key(r): 创建缓存 key
        \- ngx_http_test_predicates(r, u->conf->cache_bypass): u->conf->cache_bypass 即是 predicates，对应 proxy_cache_bypass 指令的配置。
            \- 返回 NGX_ERROR 表示出错；NGX_DECLINED 表示测试通过，需要 bypass（回源上游）；NGX_OK 表示测试没通过，继续进行缓存相关动作。
        \- u->cache_status = NGX_HTTP_CACHE_MISS: 状态设置为 MISS，没有命中缓存但需要缓存（继续后续流程）。
    \- 缓存存在；或是缓存不存在，但是需要进行缓存，也就是前面 if 执行完了
    \- rc = ngx_http_file_cache_open(r): 打开缓存
    \- switch (rc): 根据 cache open 的返回值进行处理。这个 switch 会更新 rc。后续再进行一次 switch。这个 switch 会设置 NGX_HTTP_CACHE_STALE、NGX_HTTP_CACHE_UPDATING、NGX_HTTP_CACHE_HIT 这些状态。
        \- NGX_HTTP_CACHE_STALE: 缓存已经过期。如果需要后台更新缓存则调用 ngx_http_upstream_cache_background_update 进行后台更新，Cache 状态也会被设置成 NGX_HTTP_CACHE_STALE。
        \- NGX_HTTP_CACHE_UPDATING: 缓存文件正在被**其他请求**更新。如果允许使用过期缓存，则把 Cache 状态设置成 NGX_HTTP_CACHE_UPDATING，把 rc 更新成 NGX_OK。否则把 rc 更新成 NGX_HTTP_CACHE_STALE，表示请求过期。
        \- NGX_OK: 缓存有效，直接把状态设置成 NGX_HTTP_CACHE_HIT。
    \- switch (rc): 再进行一次 switch。这个 switch 会设置 NGX_HTTP_CACHE_EXPIRED、NGX_HTTP_CACHE_HIT 状态。
        \- NGX_OK: 直接返回 NGX_OK
        \- NGX_HTTP_CACHE_STALE: 设置缓存状态为 NGX_HTTP_CACHE_EXPIRED，表示缓存过期了。
        \- NGX_DECLINED: 缓存不存在之类的情况，总之是需要访问上游的。
        \- NGX_HTTP_CACHE_SCARCE: 表示缓存空间不够了，cacheable 标记会被设置成 0，表示不可缓存。
        \- NGX_AGAIN: 缓存正忙，直接返回 NGX_BUSY。
        \- NGX_ERROR: 出错了，直接返回 NGX_ERROR
        \- default: 设置成 NGX_HTTP_CACHE_HIT 状态
    \- 能跑到这里表示都需要访问后端了，该返回的前面已经返回了。
    \- ngx_http_upstream_cache_check_range: 检查 Range 请求头，看缓存是否能满足请求的范围。返回 NGX_DECLINED 表示不能满足。会把 cacheable 标记设置成 0，表示不可缓存。
    \- r->cached = 0: 将请求对象 `r` 的 `cached` 字段设置为 0，表示该请求没有使用缓存，而是从后端服务器获取了响应
```

ngx_http_upstream_cache 函数主要是检查缓存是否存在，如何存在就打开缓存，后续读取返回；不存在就继续后续的回源处理。以及会设置响应体中的缓存状态。

## ngx_http_upstream_cache_send

```
- ngx_http_upstream_cache_send: 直接将缓存作为响应发送给客户端
    \- r->cached = 1: 上来先把缓存状态设置成 1，表示请求使用了缓存。
    \- if (c->header_start == c->body_start): 表示缓存中没有头部信息，是 HTTP 0.9。
        \- ngx_http_cache_send：直接发送缓存响应
            \- ngx_http_send_header: 发送响应头
            \- ngx_http_output_filter: 调用输出过滤链，最后发送响应
    \- 接下来主要是设置上游对象（u）的信息
    \- ngx_list_init(&u->headers_in.headers, ...): 初始化上游的响应头部列表，就是普通的请求头如 Content-Type。
    \- ngx_list_init(&u->headers_in.trailers, ...): 初始化上游的响应尾部列表。尾部列表通常用于传输一些元数据或附加信息，主要用于分块传输编码。
    \- rc = u->process_header(r): 调用上游的 `process_header` 回调函数处理响应头部
        \- rc == NGX_OK: 调用 `ngx_http_upstream_process_headers` 函数处理响应头部，如果处理失败，返回 `NGX_DONE`。否则，调用 `ngx_http_cache_send` 函数发送缓存响应。
            \- ngx_http_upstream_process_headers: 处理响应头部
            \- ngx_http_cache_send: 发送响应
        \- rc == NGX_ERROR: 出错了，直接返回 NGX_ERROR
        \- rc == NGX_AGAIN: 返回值设置为 `NGX_HTTP_UPSTREAM_INVALID_HEADER`，表示无效的响应头部
```

ngx_http_upstream_cache_send 用于把缓存作为响应发送回给客户端。

到这里为止可以回答，命中 cache 和没命中 cache 时，进行了哪些操作：

- 命中 cache 时，读取 cache 并发送给客户端。
- 没命中 cache 时，返回对应的状态码，后续进行回源。

接下来继续看下回源后，是如何保存响应体到 cache 文件中的。

## 保存 cache 的调用流程

- 获取缓存保持时间（不是验证缓存！）：

    ```lua
    ngx_http_file_cache_valid at src/http/ngx_http_file_cache.c:2299
    ngx_http_upstream_send_response at src/http/ngx_http_upstream.c:3156
    ngx_http_upstream_process_header at src/http/ngx_http_upstream.c:2503
    ngx_http_upstream_handler src/http/ngx_http_upstream.c:1292
    ngx_epoll_process_events at src/event/modules/ngx_epoll_module.c:901
    ```

- 缓存更新/保存：

    ```lua
    ngx_http_file_cache_update at src/http/ngx_http_file_cache.c:1360
    ngx_http_upstream_process_request at src/http/ngx_http_upstream.c:4154
    ngx_http_upstream_process_upstream at src/http/ngx_http_upstream.c:4097
    ngx_http_upstream_handler src/http/ngx_http_upstream.c:1292
    ngx_epoll_process_events at src/event/modules/ngx_epoll_module.c:901
    ```

从这两个调用栈，我们可以想到大致会进行以下两个工作：

1. 发送响应头给客户端前，需要知道缓存是否有效，是否使用了缓存，响应头也可能有和缓存相同的需要进行响应。
2. 处理上游发送过来的响应时，如果需要缓存，则进行缓存。

此外，获取这两个调用栈过程中，我们也可以再次确认，当命中缓存时，是**不会**再进入到这两个逻辑，也就是和我们前面看的的那样，已经提前返回了。

这两个调用栈共同的入口是 `ngx_http_upstream_handler`，因此接下来看下这个函数。以及 ngx_http_file_cache_valid 和 ngx_http_file_cache_update 这两个关键的操作缓存的函数，我们后续也来跟一下。

## ngx_http_upstream_handler

在代码中搜索了 ngx_http_upstream_handler 后，发现有必要介绍上游 handler 的设置节点以及串联起所有流程。
不过我们先继续看具体的函数，都介绍完后，再进行串联。

```
- ngx_http_upstream_handler
    \- ngx_http_set_log_request: 这是一个宏，用于将当前请求关联到连接的日志对象上。
    \- write_event_handler: 如果是写事件，调用写处理函数。
    \- read_event_handler: 如果是读事件，调用读处理函数。
    \- ngx_http_run_posted_requests: 处理 posted 的请求。（之前中断执行，接下来还需要执行的请求）
```

write_event_handler、read_event_handler 是 [ngx_http_upstream_init_request](#ngx_http_upstream_init_request) 中提到的 ngx_http_upstream_send_request_handler 和 ngx_http_upstream_process_header。

## ngx_http_file_cache_valid

```
- ngx_http_file_cache_valid
    \- for (i = 0; i < cache_valid->nelts; i++): 遍历 cache_valid 数组
        \- if (valid[i].status == 0): 如果状态码是 0，返回验证时间（表示会进行缓存）
        \- if (valid[i].status == status): 如果上游响应状态码和 proxy_cache_valid 指令设置的相同，则返回对应的验证时间。
```

与之前猜测的不同，这个函数并不是验证缓存，而是获取缓存的保持的时间。配置示例：

```nginx.conf
proxy_cache_valid 200 302 10m;
```

对 200 和 302 状态码的上游响应，设置缓存时间为 10 分钟。

## ngx_http_file_cache_update

```
- ngx_http_file_cache_update
    \- 检查是否需要更新缓存
    \- 如果需要更新:
        \- 创建临时文件
        \- 写入缓存元数据(如headers)
        \- 写入响应体数据
        \- 重命名临时文件为正式缓存文件
    \- 更新缓存元数据(如访问时间、过期时间等)
    \- 更新缓存键在内存中的信息
```

TODO

## 如何回源？如何保存 Cache 的？

- ngx_http_upstream_init_request 的时候
  - 判断是否命中缓存，如何命中并且缓存可用，直接使用缓存。
  - 如果需要回源，则创建回源请求，并连接到上游；连接到上游后，会设置好客户端与代理服务器的读写回调以及代理服务器与上游的读写回调。分别是：
    - c->write->handler = ngx_http_upstream_handler
    - c->read->handler = ngx_http_upstream_handler
    - u->write_event_handler = ngx_http_upstream_send_request_handler
    - u->read_event_handler = ngx_http_upstream_process_header
      - header 处理完后，会将 handler 改为 ngx_http_upstream_process_upstream。
- 当读写事件到达时，调用对应的回调。
- 当 ngx_http_upstream_process_header 回调中处理完上游的响应头时，会把 u->read_event_handler 更改为 ngx_http_upstream_process_upstream 以处理响应体。
- 在 ngx_http_upstream_process_header 的处理中，会调用 ngx_http_file_cache_valid 来获取缓存验证时间，以标记缓存什么时候过期。
- 在 ngx_http_upstream_process_upstream 的处理中，会调用 ngx_http_file_cache_update 更新/保存缓存。

至此，我们串联起来了 cache 处理的核心流程，同时也是回源上游的核心流程。

## Key zone

TODO

Key zone 是 Nginx 用于存储缓存键和元数据的共享内存区域。以下是关于 Key zone 的一些重要信息：

存储的信息：

缓存键 (通常是请求的 URL)
缓存文件名
缓存使用次数
上次访问时间
缓存创建时间
缓存过期时间
构建生成：

当配置了 proxy_cache_path 指令时，Nginx 会在启动时创建指定大小的共享内存区域。
缓存键信息是在请求处理过程中动态生成和存储的。
Nginx 进程启动时：

Nginx 不会自动构建所有可能的缓存键信息。
它只会分配指定大小的共享内存区域。
缓存键信息是在处理请求时动态添加的。
当 Key zone 不够用时：

Nginx 会使用 LRU (Least Recently Used) 算法。
最近最少使用的缓存项会被移除，为新的缓存项腾出空间。
被移除的缓存项对应的磁盘文件不会立即删除，而是在 inactive 参数指定的时间后才会被清理。
优化建议：

合理设置 Key zone 大小，以平衡内存使用和缓存效率。
监控 Key zone 使用情况，适时调整大小。
考虑使用多个缓存区域来分散负载。

## 函数与指令之间的关联整理

- proxy_cache_bypass: ngx_http_test_predicates
  - 示例：`proxy_cache_bypass $cookie_nocache $arg_nocache$arg_comment;`

- ngx_http_file_cache_valid: proxy_cache_valid
  - 示例：
    - proxy_cache_valid 200 302 10m;
    - proxy_cache_valid 404      1m;

## 更多说明

- u->headers_in.trailers：响应的尾部列表。尾部字段并不常用，主要用于分块传输编码（Chunked Transfer Encoding）的场景。以下是一个请求示例。

    ```
    POST /upload HTTP/1.1
    Host: www.example.com
    Content-Type: text/plain
    Transfer-Encoding: chunked
    Trailer: Signature

    7\r\n
    Mozilla\r\n
    9\r\n
    Developer\r\n
    7\r\n
    Network\r\n
    0\r\n
    Signature: s0m3s1gn4tur3\r\n
    \r\n
    ```
