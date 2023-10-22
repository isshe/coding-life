#!/bin/bash

/usr/bin/perl test.pl &

perl_pid=$!

sleep 2

# -DDEBUG_UNWIND \
stap -x $perl_pid \
    -v -DMAXSTRINGLEN=4096 \
    -DMAXSKIPPED=50000 \
    -DMAXBACKTRACE=256 \
    -DMAXACTION=500000 \
    -DMAXMAPENTRIES=20480 \
    --no-global-var-display \
    --save-uprobes \
    --nonfatal-maxaction \
    -d /usr/lib64/libc.so.6 \
    -d /usr/lib64/libperl.so.5.32.1:/usr/lib/debug/.build-id/8b/9e38a8d738070b5cffea4a2fd9fde7c873d6a8.debug:/usr/lib/debug/.dwz/perl-5.32.1-480.el9.x86_64 \
    --print-modidx \
    trace.stp > stap.out

kill -9 $perl_pid
