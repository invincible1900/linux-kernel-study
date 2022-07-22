obj-m := list_rcu_example.o 

# Please set your kernel directory before build
KDIR:=/lib/modules/$(shell uname -r)/build

ccflags-y += -Wall
ccflags-y += -Werror

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o  *.mod *.symvers *.order *.mod.c .*.cmd *.mod .tmp_versions

