#!/bin/bash
make && make clean
insmod rw_ufile.ko
rmmod rw_ufile
dmesg
hd /root/hello
