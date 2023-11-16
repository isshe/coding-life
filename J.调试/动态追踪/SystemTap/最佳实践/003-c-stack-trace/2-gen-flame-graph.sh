#!/bin/bash

basepath=$(cd `dirname $0`; pwd)

if [ ! -f $basepath/stap.out ]; then
    echo "[!] missing stap.out"
    exit 1
fi

cp $basepath/stap.out /tmp/stap.out

if [ ! -d /tmp/FlameGraph ]; then
    echo "[+] git clone FlameGraph"
    git clone git@github.com:brendangregg/FlameGraph.git /tmp/FlameGraph
fi

echo "[+] generating cbt..."
/tmp/FlameGraph/stackcollapse-stap.pl /tmp/stap.out > /tmp/stap.out.cbt

echo "[+] generating flamegraph..."
/tmp/FlameGraph/flamegraph.pl /tmp/stap.out.cbt > $basepath/out.svg

echo "[!] flame graph generated: $basepath/out.svg"

rm /tmp/stap.out
rm /tmp/stap.out.cbt
