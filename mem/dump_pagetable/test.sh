#!/bin/bash

make
insmod pg-dump.ko
rmmod pg-dump
dmesg 
make clean_all
