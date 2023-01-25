obj-m	:= dram.o 

KDIR	:= /lib/modules/$(shell uname -r)/build

PWD	:= $(shell pwd)
default:	
	$(MAKE) -C $(KDIR) M=$(PWD) modules 
	rm -r -f .tmp_versions *.mod.c .*.cmd *.o *.symvers *.mod *.order

fileview:
	g++ fileview.cpp -o fileview

clean:
	rm -f fileview dram.ko
