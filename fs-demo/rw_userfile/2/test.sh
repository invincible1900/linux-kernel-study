#!/bin/bash
#echo "hello" > /root/hello
make && make clean
insmod rw_ufile.ko
rmmod rw_ufile
dmesg
hd /root/hello
rm /root/hello
