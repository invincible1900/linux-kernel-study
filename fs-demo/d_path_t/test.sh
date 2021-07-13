#!/bin/bash
mkdir /tmp/123
cp /bin/ls /tmp/123/ls

make && make clean
insmod d_path_t.ko
rmmod d_path_t
