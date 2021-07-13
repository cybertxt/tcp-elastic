obj-m += tcp_elastic.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules
install:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules_install
clean:
	rm -f \
		*.ko \
		.*.o.cmd \
		*.o \
		*.mod.c \
		Module.symvers \
		.*.mod.cmd \
		modules.order \
		.*.ko.cmd \
		*.mod \
		.modules.order.cmd \
		.Module.symvers.cmd
