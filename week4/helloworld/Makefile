KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := hello.o
#object-module!

PWD := $(shell pwd)

default	:
		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
		#-C (Capital C)는 컴파일을 할 때 경로 지정 시 사용된다.

#결과인 ko는 Loadable Kernel Module
#다음 커맨드인 insmod means InstallModule
#다음 커맨드인 rmmod	means RemoveModule


#printk로 찍히는 내용은 dmseg를 통해 링 퍼버 조회로 확인 가능
