#!/bin/bash


level3_max_pid=262144

while true; do
    pid=$(ps aux | grep nginx | grep grep | awk -F ' ' '{print $2}')
    echo "current max pid: $pid"
    if [ "$pid" -gt "$level3_max_pid" ]; then
        exit 0
    fi
done
