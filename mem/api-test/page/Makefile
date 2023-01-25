obj-m += page-tc.o
#KDIR := /lib/modules/$(shell uname -r)/build
#KDIR := /laptop/qemu-vm/x64/linux-5.4-lkad
KDIR := /laptop/git-home/linux-5.10.90

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	@rm -rf *.o *.mod.c *.mod.o *.order *.symvers .*.cmd *.mod .tmp_versions

clean_all:
	@rm -rf *.o *.mod.c *.mod.o *.order *.symvers .*.cmd *.mod *.ko .tmp_versions
