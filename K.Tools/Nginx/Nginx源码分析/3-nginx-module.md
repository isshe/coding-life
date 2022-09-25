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

[listen]()



调用流程：

```
- ngx_http_core_listen: src/http/ngx_http_core_module.c
    \ - ngx_parse_url: src/core/ngx_inet.c, 解析 listen 指令后的第一部分（URL），先检查是否是 unix domain 类型，再检查是否是 IPv6 类型，都不是就调用 IPv4 的 URL 解析。




```


# event


另见：

- [How nginx processes a request](http://nginx.org/en/docs/http/request_processing.html)
