KERNELDIR = /lib/modules/$(shell uname -r)/build
obj-m := ledtest_dev.o
PWD := $(shell pwd)

lcdtest: lcdtest.c
	gcc -o lcdtest lcdtest.c -lwiringPi -lwiringPiDev

ledtest1: ledtest1.c
	gcc -o ledtest1 ledtest1.c -lwiringPi -lwiringPiDev

ledtest2: ledtest2.c
	gcc -o ledtest2 ledtest2.c -lwiringPi -lwiringPiDev

ledtest_dev:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

ledtest_app: ledtest_app.c
	gcc -o ledtest_app ledtest_app.c
