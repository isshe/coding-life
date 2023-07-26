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
ngx_http_lua_highcpu_test(ngx_http_request_t *r, const char *filename)
{
    ngx_file_t              file;
    ngx_str_t               file_path;
    u_char                  buffer[4096];
    ngx_regex_compile_t     rc;
    u_char                  errstr[NGX_MAX_CONF_ERRSTR];
    ngx_str_t               pattern =
        ngx_string("<body.*(https?://[-_%./\w\d#\?]*?).*></body>");
    ngx_regex_t            *re;
    ngx_int_t               n;
    ngx_int_t               len;
    ngx_str_t               input;

    ngx_log_error(NGX_LOG_INFO, r->connection->log, 0, "running high cpu test");

    file_path.data = filename;
    file_path.len = ngx_strlen(filename);

    ngx_memzero(&file, sizeof(ngx_file_t));
    file.name = file_path;
    file.log = r->connection->log;

    file.fd = ngx_open_file(file.name.data, NGX_FILE_RDONLY,
                            NGX_FILE_OPEN, 0);
    if (file.fd == NGX_INVALID_FILE) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                            ngx_open_file_n " \"%V\" failed", &file.name);
        return NGX_ERROR;
    }

    // read file
    len = ngx_read_file(&file, buffer, sizeof(buffer), 0);
    if (len == NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
            ngx_read_file_n " \"%V\" failed", &file.name);
        ngx_close_file(file.fd);
    }

    ngx_memzero(&rc, sizeof(ngx_regex_compile_t));

    rc.pattern = pattern;
    rc.pool = r->pool;
    rc.err.len = NGX_MAX_CONF_ERRSTR;
    rc.err.data = errstr;

    // regex
    if (ngx_regex_compile(&rc) != NGX_OK) {
        ngx_close_file(file.fd);
        return NGX_ERROR;
    }

    re = rc.regex;

    input.data = buffer;
    input.len = len;

    int captures[(1 + rc.captures) * 3];
    n = ngx_regex_exec(re, &input, captures, (1 + rc.captures) * 3);
    if (n >= 0) {
        /* string matches expression */
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0,
            ngx_regex_exec_n " matched: %i", n);

    } else if (n == NGX_REGEX_NO_MATCHED) {
        /* no match was found */
        ngx_log_error(NGX_LOG_INFO, r->connection->log, 0,
            ngx_regex_exec_n " no match was found: %i", n);

    } else {
        /* some error */
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
            ngx_regex_exec_n " failed: %i", n);
        // ngx_close_file(file.fd);
    }

    ngx_close_file(file.fd);

    return NGX_OK;
}
