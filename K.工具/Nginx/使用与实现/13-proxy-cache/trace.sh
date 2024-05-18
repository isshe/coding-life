#!/bin/bash

rm proxy_cache/* -rf

gdb -q -ex "set follow-fork-mode child" \
        -ex "set detach-on-fork off" \
        -ex "set args -p $(pwd) -c nginx.conf" \
        -ex "b ngx_http_file_cache_manager" \
        -ex "b ngx_http_cache_send" \
        -ex "b ngx_http_file_cache_new" \
        -ex "b ngx_http_file_cache_create" \
        -ex "b ngx_http_file_cache_open" \
        -ex "b ngx_http_file_cache_update" \
        -ex "b ngx_http_file_cache_free" \
        -ex "b ngx_http_file_cache_valid" \
        -ex "run" \
        -ex "bt" \
        -ex "quit" \
        /usr/local/openresty/nginx/sbin/nginx
