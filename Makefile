obj-m := dratio.o
dratio-objs := probes.o main.o
DEVNAME := dratio

.PHONY: all clean

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@echo "Module compiled!"

mod-install:
	insmod dratio.ko
	@echo "Module installed!"

mod-uninstall:
	- rmmod dratio

#	@if stat -t obj/* >/dev/null 2>&1; then mv -f obj/* . ; else mkdir -p obj; fi
#@mv -f *.o modules.order Module.symvers spcd.mod.c obj

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	

#	@rm -rf obj/