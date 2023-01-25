obj-m	:= dram.o 

#KDIR	:= /lib/modules/$(shell uname -r)/build
KDIR := /root/githome/linux-5.10.90

PWD	:= $(shell pwd)
all:	
	$(MAKE) -C $(KDIR) M=$(PWD) modules 
	rm -r -f .tmp_versions *.mod.c .*.cmd *.o *.symvers *.mod *.order


clean:
	rm -f dram.ko
