ngx_http_lua_loc_conf_t
---

```c
typedef struct {
#if (NGX_HTTP_SSL)
    ngx_ssl_t              *ssl;  /* shared by SSL cosockets */
    ngx_uint_t              ssl_protocols;
    ngx_str_t               ssl_ciphers;
    ngx_uint_t              ssl_verify_depth;
    ngx_str_t               ssl_trusted_certificate;
    ngx_str_t               ssl_crl;
#if (nginx_version >= 1019004)
    ngx_array_t            *ssl_conf_commands;
#endif
#endif

    ngx_flag_t              force_read_body; /* whether force request body to
                                                be read */

    ngx_flag_t              enable_code_cache; /* whether to enable
                                                  code cache */

    ngx_flag_t              http10_buffering;

    ngx_http_handler_pt     rewrite_handler;
    ngx_http_handler_pt     access_handler;
    ngx_http_handler_pt     content_handler;
    ngx_http_handler_pt     log_handler;
    ngx_http_handler_pt     header_filter_handler;

    ngx_http_output_body_filter_pt         body_filter_handler;

    u_char                  *rewrite_chunkname;
    ngx_http_complex_value_t rewrite_src;    /*  rewrite_by_lua
                                                inline script/script
                                                file path */

    u_char                  *rewrite_src_key; /* cached key for rewrite_src */
    int                      rewrite_src_ref;

    u_char                  *access_chunkname;
    ngx_http_complex_value_t access_src;     /*  access_by_lua
                                                inline script/script
                                                file path */

    u_char                  *access_src_key; /* cached key for access_src */
    int                      access_src_ref;

    u_char                  *content_chunkname;
    ngx_http_complex_value_t content_src;    /*  content_by_lua
                                                inline script/script
                                                file path */

    u_char                 *content_src_key; /* cached key for content_src */
    int                     content_src_ref;


    u_char                      *log_chunkname;
    ngx_http_complex_value_t     log_src;     /* log_by_lua inline script/script
                                                 file path */

    u_char                      *log_src_key; /* cached key for log_src */
    int                          log_src_ref;

    ngx_http_complex_value_t header_filter_src;  /*  header_filter_by_lua
                                                     inline script/script
                                                     file path */

    u_char                 *header_filter_chunkname;
    u_char                 *header_filter_src_key;
                                    /* cached key for header_filter_src */
    int                     header_filter_src_ref;


    ngx_http_complex_value_t         body_filter_src;
    u_char                          *body_filter_src_key;
    u_char                          *body_filter_chunkname;
    int                              body_filter_src_ref;

    ngx_msec_t                       keepalive_timeout;
    ngx_msec_t                       connect_timeout;
    ngx_msec_t                       send_timeout;
    ngx_msec_t                       read_timeout;

    size_t                           send_lowat;
    size_t                           buffer_size;

    ngx_uint_t                       pool_size;

    ngx_flag_t                       transform_underscores_in_resp_headers;
    ngx_flag_t                       log_socket_errors;
    ngx_flag_t                       check_client_abort;
    ngx_flag_t                       use_default_type;
} ngx_http_lua_loc_conf_t;
```
