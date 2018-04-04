#커널 빌드 디렉토리
KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := simple_char_drv.o

#현재 디렉토리
PWD := $(shell pwd)

#-C 옵션으로 커널 빌드 디렉토리로 알아서 변경한다.
default:
		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
