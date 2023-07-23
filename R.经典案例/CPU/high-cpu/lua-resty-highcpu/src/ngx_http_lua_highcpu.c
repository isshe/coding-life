/*
 * Copyright 2023 (C) isshe.
 * @author: isshe (i.sshe@outlook.com)
 */

#include "ngx_http_lua_highcpu.h"

static ngx_http_module_t ngx_http_lua_highcpu_module_ctx = {
    NULL,                               /* preconfiguration */
    NULL,                               /* postconfiguration */

    NULL,                               /* create main configuration */
    NULL,                               /* init main configuration */

    NULL,                               /* create server configuration */
    NULL,                               /* merge server configuration */

    NULL,                               /* create location configuration */
    NULL                                /* merge location configuration */
};


ngx_module_t  ngx_http_lua_highcpu_module = {
    NGX_MODULE_V1,
    &ngx_http_lua_highcpu_module_ctx,           /* module context */
    NULL,                                       /* module directives */
    NGX_HTTP_MODULE,                            /* module type */
    NULL,                                       /* init master */
    NULL,                                       /* init module */
    NULL,                                       /* init process */
    NULL,                                       /* init thread */
    NULL,                                       /* exit thread */
    NULL,                                       /* exit process */
    NULL,                                       /* exit master */
    NGX_MODULE_V1_PADDING
};


ngx_int_t
ngx_http_lua_highcpu_test(ngx_http_request_t *r)
{
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "running high cpu test");

    return NGX_OK;
}
