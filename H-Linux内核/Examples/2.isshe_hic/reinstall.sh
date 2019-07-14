#!/bin/sh

sudo rmmod isshe_hic
make
sudo insmod isshe_hic.ko
sudo dmesg -c