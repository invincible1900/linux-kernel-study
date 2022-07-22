obj-m += e1000.o
KDIR := /lib/modules/$(shell uname -r)/build

ccflags-y += -g

e1000-y := e1000_main.o e1000_hw.o e1000_ethtool.o e1000_param.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	@rm -rf *.o *.mod.c *.mod.o *.order *.symvers .*.cmd *.mod .tmp_versions

clean_all:
	@rm -rf *.o *.mod.c *.mod.o *.order *.symvers .*.cmd *.mod *.ko .tmp_versions
