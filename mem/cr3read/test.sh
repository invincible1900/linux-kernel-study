#!/bin/bash

make
insmod cr3read.ko
rmmod cr3read
dmesg 
make clean_all
