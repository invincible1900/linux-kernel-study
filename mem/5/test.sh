#!/bin/bash

make
insmod p2v.ko
rmmod p2v
dmesg 
make clean_all
