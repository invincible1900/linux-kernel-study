#!/bin/bash
make
make fileview
insmod dram.ko
mknod /dev/dram c 85 0
./fileview /dev/dram
