
```
pc bazel build --sandbox_writable_path=/root/.ccache --compilation_mode=dbg -c opt envoy --verbose_failures --sandbox_writable_path="/run/user/$UID/ccache-tmp/" --sandbox_debug --copt=-fno-limit-debug-info --copt="-Wno-error"

bazel clean --expunge

yum install libstdc++-static

wget https://dl.rockylinux.org/pub/rocky/9/CRB/x86_64/os/Packages/l/libstdc++-static-11.3.1-4.3.el9.x86_64.rpm
yum install libstdc++-static-11.3.1-4.3.el9.x86_64.rpm
```


