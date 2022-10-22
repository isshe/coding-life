ngx_http_lua_main_conf_s
---

```c
struct ngx_http_lua_main_conf_s {
    lua_State           *lua;
    ngx_pool_cleanup_t  *vm_cleanup;

    ngx_str_t            lua_path;
    ngx_str_t            lua_cpath;

    ngx_cycle_t         *cycle;
    ngx_pool_t          *pool;

    ngx_int_t            max_pending_timers;
    ngx_int_t            pending_timers;

    ngx_int_t            max_running_timers;
    ngx_int_t            running_timers;

    ngx_connection_t    *watcher;  /* for watching the process exit event */

    ngx_int_t            lua_thread_cache_max_entries;

#if (NGX_PCRE)
    ngx_int_t            regex_cache_entries;
    ngx_int_t            regex_cache_max_entries;
    ngx_int_t            regex_match_limit;
#   if (LUA_HAVE_PCRE_JIT)
    pcre_jit_stack      *jit_stack;
#   endif
#endif

    ngx_array_t         *shm_zones;  /* of ngx_shm_zone_t* */

    ngx_array_t         *shdict_zones; /* shm zones of "shdict" */

    ngx_array_t         *preload_hooks; /* of ngx_http_lua_preload_hook_t */

    ngx_flag_t           postponed_to_rewrite_phase_end;
    ngx_flag_t           postponed_to_access_phase_end;

    ngx_http_lua_main_conf_handler_pt    init_handler;
    ngx_str_t                            init_src;
    u_char                              *init_chunkname;

    ngx_http_lua_main_conf_handler_pt    init_worker_handler;
    ngx_str_t                            init_worker_src;
    u_char                              *init_worker_chunkname;

    ngx_http_lua_main_conf_handler_pt    exit_worker_handler;
    ngx_str_t                            exit_worker_src;
    u_char                              *exit_worker_chunkname;

    ngx_chain_t                            *body_filter_chain;
                    /* neither yielding nor recursion is possible in
                     * body_filter_by_lua*, so there cannot be any races among
                     * concurrent requests when storing the chain
                     * data pointer in the main conf.
                     */

    ngx_http_variable_value_t              *setby_args;
                    /* neither yielding nor recursion is possible in
                     * set_by_lua*, so there cannot be any races among
                     * concurrent requests when storing the args pointer
                     * in the main conf.
                     */

    size_t                                  setby_nargs;
                    /* neither yielding nor recursion is possible in
                     * set_by_lua*, so there cannot be any races among
                     * concurrent requests when storing the nargs in the
                     * main conf.
                     */

    ngx_uint_t                      shm_zones_inited;

    ngx_http_lua_sema_mm_t         *sema_mm;

    ngx_uint_t           malloc_trim_cycle;  /* a cycle is defined as the number
                                                of requests */
    ngx_uint_t           malloc_trim_req_count;

    ngx_uint_t           directive_line;

#if (nginx_version >= 1011011)
    /* the following 2 fields are only used by ngx.req.raw_headers() for now */
    ngx_buf_t          **busy_buf_ptrs;
    ngx_int_t            busy_buf_ptr_count;
#endif

    ngx_int_t            host_var_index;

    ngx_flag_t           set_sa_restart;

    ngx_queue_t          free_lua_threads;  /* of ngx_http_lua_thread_ref_t */
    ngx_queue_t          cached_lua_threads;  /* of ngx_http_lua_thread_ref_t */

    ngx_uint_t           worker_thread_vm_pool_size;


#ifdef NGX_HTTP_LUA_CHECK_LICENSE_OPTIONAL
    ngx_flag_t           enable_plus_license_check;
#endif

    unsigned             requires_header_filter:1;
    unsigned             requires_body_filter:1;
    unsigned             requires_capture_filter:1;
    unsigned             requires_rewrite:1;
    unsigned             requires_access:1;
    unsigned             requires_log:1;
    unsigned             requires_shm:1;
    unsigned             requires_capture_log:1;
};
```
