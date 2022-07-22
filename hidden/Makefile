obj-m += hidden.o
KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	@rm -rf *.o *.mod.c *.mod.o *.order *.symvers .*.cmd *.mod .tmp_versions

clean_all:
	@rm -rf *.o *.mod.c *.mod.o *.order *.symvers .*.cmd *.mod *.ko .tmp_versions
