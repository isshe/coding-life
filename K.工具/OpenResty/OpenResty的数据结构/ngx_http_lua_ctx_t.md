ngx_http_lua_ctx_t
---

```c
typedef struct ngx_http_lua_ctx_s {
    /* for lua_code_cache off: */
    ngx_http_lua_vm_state_t  *vm_state;

    ngx_http_request_t      *request;
    ngx_http_handler_pt      resume_handler;

    ngx_http_lua_co_ctx_t   *cur_co_ctx; /* co ctx for the current coroutine */

    /* FIXME: we should use rbtree here to prevent O(n) lookup overhead */
    ngx_list_t              *user_co_ctx; /* coroutine contexts for user
                                             coroutines */

    ngx_http_lua_co_ctx_t    entry_co_ctx; /* coroutine context for the
                                              entry coroutine */

    ngx_http_lua_co_ctx_t   *on_abort_co_ctx; /* coroutine context for the
                                                 on_abort thread */

    int                      ctx_ref;  /*  reference to anchor
                                           request ctx data in lua
                                           registry */

    unsigned                 flushing_coros; /* number of coroutines waiting on
                                                ngx.flush(true) */

    ngx_chain_t             *out;  /* buffered output chain for HTTP 1.0 */
    ngx_chain_t             *free_bufs;
    ngx_chain_t             *busy_bufs;
    ngx_chain_t             *free_recv_bufs;

    ngx_chain_t             *filter_in_bufs;  /* for the body filter */
    ngx_chain_t             *filter_busy_bufs;  /* for the body filter */

    ngx_http_cleanup_pt     *cleanup;

    ngx_http_cleanup_t      *free_cleanup; /* free list of cleanup records */

    ngx_chain_t             *body; /* buffered subrequest response body
                                      chains */

    ngx_chain_t            **last_body; /* for the "body" field */

    ngx_str_t                exec_uri;
    ngx_str_t                exec_args;

    ngx_int_t                exit_code;

    void                    *downstream;  /* can be either
                                             ngx_http_lua_socket_tcp_upstream_t
                                             or ngx_http_lua_co_ctx_t */

    ngx_uint_t               index;              /* index of the current
                                                    subrequest in its parent
                                                    request */

    ngx_http_lua_posted_thread_t   *posted_threads;

    int                      uthreads; /* number of active user threads */

    uint16_t                 context;   /* the current running directive context
                                           (or running phase) for the current
                                           Lua chunk */

    unsigned                 run_post_subrequest:1; /* whether it has run
                                                       post_subrequest
                                                       (for subrequests only) */

    unsigned                 waiting_more_body:1;   /* 1: waiting for more
                                                       request body data;
                                                       0: no need to wait */

    unsigned         co_op:2; /*  coroutine API operation */

    unsigned         exited:1;

    unsigned         eof:1;             /*  1: last_buf has been sent;
                                            0: last_buf not sent yet */

    unsigned         capture:1;  /*  1: response body of current request
                                        is to be captured by the lua
                                        capture filter,
                                     0: not to be captured */


    unsigned         read_body_done:1;      /* 1: request body has been all
                                               read; 0: body has not been
                                               all read */

    unsigned         headers_set:1; /* whether the user has set custom
                                       response headers */
    unsigned         mime_set:1;    /* whether the user has set Content-Type
                                       response header */

    unsigned         entered_rewrite_phase:1;
    unsigned         entered_access_phase:1;
    unsigned         entered_content_phase:1;

    unsigned         buffering:1; /* HTTP 1.0 response body buffering flag */

    unsigned         no_abort:1; /* prohibit "world abortion" via ngx.exit()
                                    and etc */

    unsigned         header_sent:1; /* r->header_sent is not sufficient for
                                     * this because special header filters
                                     * like ngx_image_filter may intercept
                                     * the header. so we should always test
                                     * both flags. see the test case in
                                     * t/020-subrequest.t */

    unsigned         seen_last_in_filter:1;  /* used by body_filter_by_lua* */
    unsigned         seen_last_for_subreq:1; /* used by body capture filter */
    unsigned         writing_raw_req_socket:1; /* used by raw downstream
                                                  socket */
    unsigned         acquired_raw_req_socket:1;  /* whether a raw req socket
                                                    is acquired */
    unsigned         seen_body_data:1;
} ngx_http_lua_ctx_t;
```
