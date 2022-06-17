#!/bin/bash

make && make clean
scp genl.ko root@192.168.53.76:~
ssh root@192.168.53.76 "rmmod genl"
ssh root@192.168.53.76 "insmod genl.ko"

