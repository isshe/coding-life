安装
---

```
./configure--with-cc-opt="-O0 -fno-omit-frame-pointer" \
    --with-debug \
    --prefix=/root/persional/openresty-plus-bin \
    --with-http_ssl_module \
    --with-http_v2_module \
    --with-luajit-xcflags="-DLUAJIT_USE_VALGRIND"
```
