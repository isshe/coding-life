#!/bin/bash

basepath=$(cd `dirname $0`; pwd)

# cat stap.out | awk -F" " '{for(i=2;i<=NF;i++) printf $i" ";printf "\n"}' > /tmp/stap.out.processed
cp stap.out /tmp/stap.out.processed

/root/orinc/FlameGraph/flamegraph.pl /tmp/stap.out.processed > $basepath/flamegraph.svg

rm /tmp/stap.out.processed
