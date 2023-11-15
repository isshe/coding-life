#!/bin/bash

/usr/bin/perl test.pl &

perl_pid=$!

sleep 2

/opt/stap/bin/stap \
    -v \
    -d /usr/bin/perl \
    -d /usr/lib64/ld-linux-x86-64.so.2 \
    -d /usr/lib64/libc.so.6 \
    -d /usr/lib64/libcrypt.so.2.0.0 \
    -d /usr/lib64/libm.so.6 \
    -d /usr/lib64/libperl.so.5.32.1 \
    -x $perl_pid \
    trace.stp > stap.out

kill -9 $perl_pid
