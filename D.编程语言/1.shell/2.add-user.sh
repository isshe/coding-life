#!/bin/bash

set -x

if [ "$UID" != 0 ]; then
    echo "Please run this script with sudo:"
    echo "    sudo bash $0 $*"
    exit 1
fi

username=$1
publickey=$2

useradd $username

mkdir -p /home/$username/.ssh
echo "$publickey" >> /home/$username/.ssh/authorized_keys
chmod 0600 /home/$username/.ssh/authorized_keys
chown -R $username:$username /home/$username/.ssh
echo "$username ALL=(ALL)       NOPASSWD: ALL" > /etc/sudoers.d/$username
