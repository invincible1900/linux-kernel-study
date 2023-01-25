#!/bin/bash

make
insmod slab-tc.ko
rmmod slab-tc
dmesg 
make clean_all
