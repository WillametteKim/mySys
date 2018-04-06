KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := ioctl_app_dev.o

PWD := $(shell pwd)


all: default rmmod rm insmod mknod clean

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

rmmod:
	sudo rmmod ioctl_app_dev

rm:
	sudo rm /dev/ioctl_app_dev

insmod:
	sudo insmod ioctl_app_dev.ko major=280

mknod:
	sudo mknod -m 666 /dev/ioctl_app_dev c 280 0

ioctl_app:
	gcc -o ioctl_app ioctl_app.c

clean:
	rm *.o
