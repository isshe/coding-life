# Proxy Cache 的使用与实现

目的：

- 了解如何使用 Proxy Cache。
- 了解 Proxy Cache 是如何实现的。
  - 处理流程。
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
    ngx_http_upstream_cache_send at src/http/ngx_http_file_cache.c
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

```
- ngx_http_upstream_init_request
    \- rc = ngx_http_upstream_cache(): 进行缓存处理
        \- if (r->cache == NULL): 首先检查一下缓存对象是否存在，这个判断为真表示不存在
            \- if (!(r->method & u->conf->cache_methods)): 检查是否是能缓存的方法，不是直接返回 NGX_DECLINED。
            \- ngx_http_upstream_cache_get(r, u, &cache): TODO 这个是什么作用？没看懂，把 Nginx 跑起来看看
    \- if (rc == NGX_BUSY): 正忙，下次再进来，通过设置 r->write_event_handler = ngx_http_upstream_init_request 实现
    \- if (rc == NGX_ERROR): 出错了，直接结束请求
        \- ngx_http_finalize_request
    \- if (rc == NGX_OK): 缓存命中了
        \- ngx_http_upstream_cache_send: 直接将缓存作为响应
            \- TODO
    \- if (rc != NGX_DECLINED): 非缓存未命中的情况，就直接结束请求
        \- ngx_http_finalize_request
    \- ...: TODO
    \- ngx_http_upstream_connect:
        \- TODO
```
