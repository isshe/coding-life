#!/bin/bash

file=/usr/lib64/ld-linux-x86-64.so.2
if [ -n "$1" ]; then
    file=$1
fi

pkg=$(rpm -qf $file 2>/dev/null)
if [ $? -ne 0 ] || [ -z "$pkg" ]; then
    echo "[!] $file does not belong to any package."
    exit 1
fi

echo "[+] $file belong to package $pkg."

pkg_name=$(rpm -qi $pkg 2>/dev/null | grep Name | awk -F ' ' '{print $3}')
debug_pkg_name=${pkg_name}-debuginfo
if rpm -q $debug_pkg_name &> /dev/null; then
    echo "[!] debug package $debug_pkg_name already installed."
else
    # install debug package
    sudo dnf install -y $debug_pkg_name
    if [ $? -eq 0 ]; then
        echo "[+] debug package $debug_pkg_name installed successfully."
    else
        echo "[!] failed to install debug package $debug_pkg_name."
        exit 1
    fi
fi

filename=$(basename $file)
debug_file_path=$(rpm -ql $debug_pkg_name 2>/dev/null | grep $filename)
echo "[+] Debug file path: $debug_file_path"
