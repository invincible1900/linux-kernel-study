#!/bin/bash

make && make clean
scp tc.ko root@192.168.53.76:~
ssh root@192.168.53.76 "insmod tc.ko && rmmod tc.ko && dmesg|tail"

