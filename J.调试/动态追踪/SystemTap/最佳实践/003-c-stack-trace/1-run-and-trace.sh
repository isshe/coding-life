#!/bin/bash

/usr/bin/perl test.pl &

perl_pid=$!

sleep 2

path="/root/orinc/A-Personal/job-examples/stap-c-on-cpu/build-box/job-6741500"

# -DDEBUG_UNWIND \
# -x $perl_pid
stap -v \
    -DMAXSKIPPED=50000 \
    -DMAXACTION=500000 \
    -DMAXMAPENTRIES=20480 \
    -DSTP_CTL_BUFFER_SIZE=8320 \
    -DSTP_LOG_BUF_LEN=8192 \
    -DMAXBACKTRACE=256 \
    -DMAXSTRINGLEN=4096 --nonfatal-overload \
    --no-global-var-display \
    --no-w 'Potential type mismatch in reassignment' \
    --no-w 'side-effect-free' \
    --no-w 'Eliding unused' \
    --no-w 'statement will never be reached' \
    --no-w 'never-assigned local variable' \
    --no-w 'Eliding assignment to ' \
    --no-w 'never-assigned global variable' \
    --no-global-var-display \
    --save-uprobes \
    --nonfatal-maxaction \
    -r /root/orinc/A-Personal/job-examples/stap-c-on-cpu/build-box/5.14.0-284.18.1.el9_2.x86_64 \
    -d /usr/bin/perl:$path/ld/perl.1.melf \
    -d /usr/lib64/ld-linux-x86-64.so.2:$path/ld/ld-linux-x86-64.so.2.2.melf \
    -d /usr/lib64/libc.so.6:$path/ld/libc.so.6.3.melf \
    -d /usr/lib64/libcrypt.so.2.0.0:$path/ld/libcrypt.so.2.0.0.4.melf \
    -d /usr/lib64/libm.so.6:$path/ld/libm.so.6.5.melf \
    -d /usr/lib64/libperl.so.5.32.1:$path/ld/libperl.so.5.32.1.6.melf \
    -d /usr/lib64/perl5/vendor_perl/auto/Time/HiRes/HiRes.so:$path/ld/HiRes.so.7.melf \
    --print-modidx \
    --include /usr/local/openresty-stap/share/systemtap/tapset/private30.stpm \
    --include /usr/local/openresty-stap/share/systemtap/tapset/uconversions.stp \
    --include /usr/local/openresty-stap/share/systemtap/tapset/linux/task.stpm \
    --include /usr/local/openresty-stap/share/systemtap/tapset/choose_defined.stpm \
    --include /usr/local/openresty-stap/share/systemtap/tapset/linux/task.stp \
    --include /usr/local/openresty-stap/share/systemtap/tapset/linux/context.stp \
    --include /usr/local/openresty-stap/share/systemtap/tapset/linux/rlimit.stp \
    --include /usr/local/openresty-stap/share/systemtap/tapset/logging.stp \
    --include /usr/local/openresty-stap/share/systemtap/tapset/linux/ucontext-unwind.stp \
    --include /usr/local/openresty-stap/share/systemtap/tapset/string.stp \
    --include /usr/local/openresty-stap/share/systemtap/tapset/macros.stpm \
    --include /usr/local/openresty-stap/share/systemtap/tapset/linux/conversions.stp \
    --use-stapconf $path/khdr/stapconf.h \
    -B CC=/usr/bin/gcc \
    -p4 -m trace \
    trace.stp

staprun "trace.ko" \
    -a "max_samples=1000" \
    -a "sample_time=15" "-b 4" \
    "stp_probe_must_match_targets=1" \
    "stp_min_uprobe_interval_cycles=3000000" \
    "stp_min_tracepoint_interval_cycles=3000000" \
    "stp_min_kprobe_interval_cycles=3000000" \
    "stp_perf_profile_sample_period=200000" -x $perl_pid -R > stap.out

kill -9 $perl_pid

    # -d /usr/lib64/perl5/vendor_perl/auto/Time/HiRes/HiRes.so \
    # -d /usr/lib64/libz.so.1.2.11 \
    # -d /usr/lib64/libcrypt.so.2.0.0 \
    # -d /usr/lib64/ld-linux-x86-64.so.2 \
    # -d /usr/lib64/libc.so.6 \
    # -d /usr/lib64/libperl.so.5.32.1:/usr/lib/debug/.build-id/8b/9e38a8d738070b5cffea4a2fd9fde7c873d6a8.debug:/usr/lib/debug/.dwz/perl-5.32.1-480.el9.x86_64 \
