#!/bin/bash

OPENRESTY_SRC=$1
OPENRESTY_BIN=$2
MY_MODULE=$3

cd $OPENRESTY_SRC

orprefix=/usr/local/openresty
zlib_prefix=$orprefix/zlib
pcre_prefix=$orprefix/pcre
openssl_prefix=$orprefix/openssl111

./configure \
    --prefix="$OPENRESTY_BIN" \
    --with-cc='ccache gcc -fdiagnostics-color=always' \
    --with-cc-opt="-DNGX_LUA_ABORT_AT_PANIC -I$zlib_prefix/include -I$pcre_prefix/include -I$openssl_prefix/include" \
    --with-ld-opt="-L$zlib_prefix/lib -L$pcre_prefix/lib -L$openssl_prefix/lib -Wl,-rpath,$zlib_prefix/lib:$pcre_prefix/lib:$openssl_prefix/lib" \
    --with-pcre-jit \
    --without-http_rds_json_module \
    --without-http_rds_csv_module \
    --without-lua_rds_parser \
    --with-stream \
    --with-stream_ssl_module \
    --with-stream_ssl_preread_module \
    --with-http_v2_module \
    --without-mail_pop3_module \
    --without-mail_imap_module \
    --without-mail_smtp_module \
    --with-http_stub_status_module \
    --with-http_realip_module \
    --with-http_addition_module \
    --with-http_auth_request_module \
    --with-http_secure_link_module \
    --with-http_random_index_module \
    --with-http_gzip_static_module \
    --with-http_sub_module \
    --with-http_dav_module \
    --with-http_flv_module \
    --with-http_mp4_module \
    --with-http_gunzip_module \
    --with-threads \
    --with-compat \
    --with-luajit-xcflags='-DLUAJIT_NUMMODE=2 -DLUAJIT_ENABLE_LUA52COMPAT' \
    --with-debug --add-module=$MY_MODULE \
    -j4
