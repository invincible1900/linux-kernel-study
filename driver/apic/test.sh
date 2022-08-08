#!/bin/bash

make && make clean
insmod tc.ko && rmmod tc.ko && dmesg|tail -n 10

