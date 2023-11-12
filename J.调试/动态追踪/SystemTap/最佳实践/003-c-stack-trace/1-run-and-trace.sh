#!/bin/bash

/usr/bin/perl test.pl &

perl_pid=$!

sleep 2

export DEBUGINFOD_URLS=https://debuginfod.elfutils.org/
export DEBUGINFOD_PROGRESS=1

# -DDEBUG_UNWIND \
# -x $perl_pid
/opt/stap/bin/stap \
    -v \
    -DMAXSKIPPED=50000 \
    -DMAXACTION=500000 \
    -DMAXMAPENTRIES=20480 \
    -DSTP_LOG_BUF_LEN=8192 \
    -DMAXBACKTRACE=256 \
    -DMAXSTRINGLEN=4096 \
    --no-global-var-display \
    --save-uprobes \
    --all-modules \
    -d /usr/bin/perl \
    -d /usr/lib64/ld-linux-x86-64.so.2 \
    -d /usr/lib64/libc.so.6 \
    -d /usr/lib64/libcrypt.so.2.0.0 \
    -d /usr/lib64/libm.so.6 \
    -d /usr/lib64/libperl.so.5.32.1 \
    --download-debuginfo \
    -B CC=/usr/bin/gcc \
    -p4 -m trace \
    -k \
    trace.stp

# 采用自行编译的 stap
/opt/stap/bin/staprun "trace.ko" \
    "-b 4" \
    "stp_probe_must_match_targets=1" \
    "stp_min_uprobe_interval_cycles=3000000" \
    "stp_min_tracepoint_interval_cycles=3000000" \
    "stp_min_kprobe_interval_cycles=3000000" \
    "stp_perf_profile_sample_period=200000" \
    -x $perl_pid -R > stap.out

cat /proc/$perl_pid/maps > maps.out

# gdb -p $perl_pid

kill -9 $perl_pid
