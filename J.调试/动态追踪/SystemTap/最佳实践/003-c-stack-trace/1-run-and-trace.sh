#!/bin/bash

/usr/bin/perl test.pl &

perl_pid=$!

stap -x $perl_pid \
    -v -DMAXSTRINGLEN=4096 \
    -DMAXSKIPPED=50000 \
    -DMAXBACKTRACE=256 \
    -DMAXACTION=500000 \
    -DMAXMAPENTRIES=20480 \
    trace.stp
