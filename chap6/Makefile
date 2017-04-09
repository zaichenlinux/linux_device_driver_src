obj-m := ldm.o
$(obj-m:.o=)-objs := globalmem.o

KERNEL_PATH := /media/chenzai/linuxfile/linux3.5src/linux-3.5
NFS_PATH := /home/chenzai/work/nfsroot

ccflags-y += -Wno-declaration-after-statement

all: build mv clean

build:
	make M=`pwd` -C $(KERNEL_PATH) modules

mv:
	mv $(obj-m:.o=.ko) $(NFS_PATH)

clean:
	make M=`pwd` -C $(KERNEL_PATH) clean

.PHONY: all clean build mv
