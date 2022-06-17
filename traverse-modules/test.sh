#!/bin/bash
make && make clean
insmod tm.ko
rmmod tm
dmesg  

