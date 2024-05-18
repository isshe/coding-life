# Proxy Cache 的使用与实现

目的：

- 了解如何使用 Proxy Cache。
- 了解 Proxy Cache 是如何实现的。
  - 处理流程。

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
        keys_zone=proxycache
        inactive=2592000
        max_size=5368709120;

    proxy_cache $proxy_cache_zone;
    proxy_cache_key $proxy_cache_key;
    # ...
}
```

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

    调用 `ngx_http_file_cache_new` 后，后续还会继续调用 `ngx_http_file_cache_open`、`ngx_http_file_cache_update`、`ngx_http_file_cache_free` 等，都在 `ngx_http_upstream_cache`。相关函数的作用后续继续探究（TODO）。

- 命中 Cache 时：

    ```lua
    ngx_http_cache_send at src/http/ngx_http_file_cache.c
    ngx_http_upstream_init_request at src/http/ngx_http_upstream.c
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

从上面两个调用栈中可以得到以下信息

- 是在 `content` 阶段处理 Cache 的
- 共同的入口都是 `ngx_http_upstream_init_request` 函数，因此下一步我们主要跟踪这个函数的实现。

### ngx_http_upstream_init_request



```gpt
Nginx 缓存上游响应的过程主要涉及以下几个关键步骤:

1. **确定是否需要缓存**

Nginx 首先会根据请求的 URI、HTTP 方法、请求头等信息判断该请求是否可以被缓存。这些判断逻辑主要位于源文件 `src/http/modules/ngx_http_cache_util.c` 中的 `ngx_http_file_cache_manager` 函数。

2. **构建缓存 Key**

如果请求可以被缓存，Nginx 会根据相关信息构建一个缓存 Key。主要逻辑位于 `src/http/modules/ngx_http_file_cache.c` 中的 `ngx_http_cache_manager_utils` 函数。

3. **查询缓存**

使用构建好的缓存 Key 在缓存空间中查询是否有对应的缓存项。这一步的主要逻辑在 `src/http/modules/ngx_http_file_cache.c` 中的 `ngx_http_cache_manager_utils` 函数中实现。

4. **命中缓存**

如果命中缓存，Nginx 会从缓存空间读取响应数据并返回给客户端。相关逻辑在 `src/http/modules/ngx_http_file_cache.c` 中的 `ngx_http_cache_send` 函数中实现。

5. **未命中缓存**

如果未命中缓存，Nginx 会继续向上游服务器发送请求，获取响应数据。相关逻辑在 `src/http/ngx_http_upstream.c` 中的 `ngx_http_upstream_process_headers` 函数中实现。

6. **缓存响应数据**

当从上游服务器获取到响应数据后，Nginx 会根据响应头中的缓存相关字段判断是否需要缓存该响应。如果需要缓存，Nginx 会将响应数据写入缓存空间。相关逻辑在 `src/http/modules/ngx_http_file_cache.c` 中的 `ngx_http_file_cache_set` 函数中实现。

在整个过程中，Nginx 还会根据缓存空间的大小、缓存过期策略等配置进行相应的操作。例如，当缓存空间不足时，Nginx 会根据配置的策略淘汰部分缓存项，以腾出空间存储新的缓存项。

总的来说，Nginx 缓存上游响应的核心逻辑主要集中在 `src/http/modules/ngx_http_file_cache.c` 和 `src/http/modules/ngx_http_cache_util.c` 这两个源文件中。通过对请求信息的分析、缓存 Key 的构建、缓存空间的查询和写入等步骤，Nginx 实现了对上游响应数据的高效缓存。
```
