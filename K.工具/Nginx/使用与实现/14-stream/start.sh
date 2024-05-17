#!/bin/bash

basepath=$(cd `dirname $0`; pwd)

if [ ! -d $basepath/logs ]; then
    mkdir $basepath/logs
fi

echo "stopping nginx..."
/usr/local/openresty/nginx/sbin/nginx -p $(pwd) -c nginx.conf -s stop

echo "starting nginx..."
/usr/local/openresty/nginx/sbin/nginx -p $(pwd) -c nginx.conf

echo "done"
