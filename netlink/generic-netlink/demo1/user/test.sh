#!/bin/bash

make clean & make
scp genl root@192.168.53.76:~
ssh root@192.168.53.76 "./genl"
