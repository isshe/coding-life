#!/bin/bash

/usr/bin/perl test.pl &

perl_pid=$!

sleep 2

# -DDEBUG_UNWIND \
# -x $perl_pid
/usr/bin/stap -v \
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
    -d /usr/lib64/perl5/vendor_perl/auto/Time/HiRes/HiRes.so \
    -d /usr/lib/debug/usr/bin/perl-5.32.1-480.el9.x86_64.debug \
    -d /usr/lib/debug/lib64/ld-linux-x86-64.so.2-2.34-60.el9.x86_64.debug \
    -d /usr/lib/debug/lib64/libc.so.6-2.34-60.el9.x86_64.debug \
    -d /usr/lib/debug/usr/lib64/libcrypt.so.2.0.0-4.4.18-3.el9.x86_64.debug \
    -d /usr/lib/debug/lib64/libm.so.6-2.34-60.el9.x86_64.debug \
    -d /usr/lib/debug/usr/lib64/libperl.so.5.32.1-5.32.1-480.el9.x86_64.debug \
    -d /usr/lib/debug/usr/lib64/perl5/vendor_perl/auto/Time/HiRes/HiRes.so-1.9764-462.el9.x86_64.debug \
    -B CC=/usr/bin/gcc \
    -p4 -m trace \
    -k \
    trace.stp

/usr/bin/staprun "trace.ko" \
    "-b 4" \
    "stp_probe_must_match_targets=1" \
    "stp_min_uprobe_interval_cycles=3000000" \
    "stp_min_tracepoint_interval_cycles=3000000" \
    "stp_min_kprobe_interval_cycles=3000000" \
    "stp_perf_profile_sample_period=200000" \
    -x $perl_pid -R > stap.out

cat /proc/$perl_pid/maps > maps.out

gdb -p $perl_pid

kill -9 $perl_pid

    # -d /usr/lib64/perl5/vendor_perl/auto/Time/HiRes/HiRes.so \
    # -d /usr/lib64/libz.so.1.2.11 \
    # -d /usr/lib64/libcrypt.so.2.0.0 \
    # -d /usr/lib64/ld-linux-x86-64.so.2 \
    # -d /usr/lib64/libc.so.6 \
    # -d /usr/lib64/libperl.so.5.32.1:/usr/lib/debug/.build-id/8b/9e38a8d738070b5cffea4a2fd9fde7c873d6a8.debug:/usr/lib/debug/.dwz/perl-5.32.1-480.el9.x86_64 \
