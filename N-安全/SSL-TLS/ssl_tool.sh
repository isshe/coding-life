#!/bin/bash

basepath=$(cd `dirname $0`; pwd)


domain=www.edgetest.com
if [ $1 ]; then
    domain=$1
fi

day=1
if [ $2 ]; then
    day=$2
fi

dirname=${basepath}/ssl-${domain}-`date +%s`
mkdir ${dirname}
cd ${dirname}


ca=${domain}-ca
server=${domain}-server
client=${domain}-client

openssl genrsa -out ${server}.key 1024

openssl req -new -x509 -days ${day} -key ${server}.key -out ${server}.crt -subj "/C=CN/ST=Guangdong/L=Shenzhen/O=Global Security/OU=IT Department/CN=${domain}"
