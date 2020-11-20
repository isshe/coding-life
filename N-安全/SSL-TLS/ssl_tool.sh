#!/bin/bash

basepath=$(cd `dirname $0`; pwd)


domain=pkg.openresty.com
if [ $1 ]; then
    domain=$1
fi

day=3650
if [ $2 ]; then
    day=$2
fi

dirname=${basepath}/ssl-${domain}-`date +%s`
mkdir ${dirname}
cd ${dirname}


ca=${domain}-ca
server=${domain}-server
client=${domain}-client

echo "[+] gen ca.key"
openssl genrsa -out ${ca}.key 2048

echo "[+] gen ca.csr"
openssl req -new -key ${ca}.key -out ${ca}.csr -subj "/C=CN/ST=Guangdong/L=Shenzhen/O=Global Security/OU=IT Department/CN=${domain}"

echo "[+] gen ca.crt"
openssl x509 -req -days 365 -in ${ca}.csr -signkey ${ca}.key -out ${ca}.crt

echo "[+] gen client.key"
openssl genrsa -out ${client}.key 1024

echo "[+] gen client.csr"
openssl req -new -out ${client}.csr -key ${client}.key -subj "/C=CN/ST=Guangdong/L=Shenzhen/O=Global Security/OU=IT Department/CN=${domain}"

echo "[+] gen client.crt"
openssl x509 -req -in ${client}.csr -out ${client}.crt -signkey ${client}.key -CA ${ca}.crt -CAkey ${ca}.key -CAcreateserial -days 3650

echo "[+] gen client.p12"
openssl pkcs12 -export -clcerts -in ${client}.crt -inkey ${client}.key -out ${client}.p12

openssl genrsa -out ${server}.key 1024

openssl req -new -x509 -days ${day} -key ${server}.key -out ${server}.crt -subj "/C=CN/ST=Guangdong/L=Shenzhen/O=Global Security/OU=IT Department/CN=${domain}"
