ngx_http_lua_srv_conf_t
---

```c
struct ngx_http_lua_srv_conf_s {
#if (NGX_HTTP_SSL)
    struct {
        ngx_http_lua_srv_conf_handler_pt     ssl_cert_handler;
        ngx_str_t                            ssl_cert_src;
        u_char                              *ssl_cert_src_key;
        u_char                              *ssl_cert_chunkname;
        int                                  ssl_cert_src_ref;

        ngx_http_lua_srv_conf_handler_pt     ssl_sess_store_handler;
        ngx_str_t                            ssl_sess_store_src;
        u_char                              *ssl_sess_store_src_key;
        u_char                              *ssl_sess_store_chunkname;
        int                                  ssl_sess_store_src_ref;

        ngx_http_lua_srv_conf_handler_pt     ssl_sess_fetch_handler;
        ngx_str_t                            ssl_sess_fetch_src;
        u_char                              *ssl_sess_fetch_src_key;
        u_char                              *ssl_sess_fetch_chunkname;
        int                                  ssl_sess_fetch_src_ref;

        ngx_http_lua_srv_conf_handler_pt     ssl_alpn_select_handler;
        ngx_str_t                            ssl_alpn_select_src;
        u_char                              *ssl_alpn_select_src_key;
        int                                  ssl_alpn_select_src_ref;

        ngx_http_lua_srv_conf_handler_pt     ssl_client_hello_handler;
        ngx_str_t                            ssl_client_hello_src;
        u_char                              *ssl_client_hello_src_key;
        u_char                              *ssl_client_hello_chunkname;
        int                                  ssl_client_hello_src_ref;
    } srv;
#endif

    struct {
        ngx_uint_t                           max_cached;
        ngx_queue_t                          cache;
        ngx_queue_t                          free;
        ngx_queue_t                         *buckets;
        ngx_uint_t                           bucket_cnt;
        ngx_http_upstream_init_pt            original_init_upstream;
        ngx_http_upstream_init_peer_pt       original_init_peer;

        ngx_http_lua_srv_conf_handler_pt     handler;
        ngx_str_t                            src;
        u_char                              *src_key;
        u_char                              *chunkname;
        int                                  src_ref;
    } balancer;
};
```
