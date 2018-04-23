KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := mmap.o

PWD := $(shell pwd)


all: default rmmod rm insmod mknod build clean

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

rmmod:
	sudo rmmod mmap

rm:
	sudo rm /dev/mmap

insmod:
	sudo insmod mmap.ko major=244

mknod:
	sudo mknod -m 666 /dev/mmap c 244 0

build:
	gcc -o mmap_app mmap_app.c

clean:
	rm *.o
