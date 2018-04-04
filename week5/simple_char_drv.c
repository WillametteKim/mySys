/*
User App
	<Low level file op.>
Device File
	<system call, file op.>
Device Driver
	<hardware control>
H/W
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define BUFSIZE 32

struct cdev my_cdev;
dev_t devno;
static char *msg = NULL;
int major, retval;
char buf [BUFSIZE];
char msgCorrect[] 	= "TEST";			//RETURN MSG
char msgIncorrect[] = "NOT TEST";	//RETURN MSG

//module_param(name, type, permission), 모듈이 시작될 때 파라미터 받아온다~
module_param(major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major, "major");


int simple_open (struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "Open simple char drv\n");
	memset(msg, 0, BUFSIZE);
	return 0;
}


//read한다는게 user가 디바이스로부터 읽을 때를 의미한다?
ssize_t simple_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_ALERT "Read simple char drv\n");

	if(strcasecmp(buf, "test") == 0)	//Ignore Case
		//copy_to_user(to사용자메모리블록주소, from커널메모리블록주소, n기록할바이트수)
		retval = copy_to_user(buffer, msgCorrect, sizeof(msgCorrect));	//Device file에 기록

	else
		retval = copy_to_user(buffer, msgIncorrect, sizeof(msgIncorrect));

	if(retval < 0)
		printk(KERN_ALERT "copy tu user fail!\n");

	return 0;
}


ssize_t simple_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_ALERT "write simple char drv\n");

	//copy_from_user(to커널메모리블록주소, from사용자메모리블록주소, n기록할바이트수)
	if(copy_from_user(buf, buffer, BUFSIZE) < 0)		//Device file에 입력이 있으면 그 입력값을 읽음
		printk(KERN_ALERT "WRITE ERROR %s\n", buf);

	return length;
}


int simple_close (struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "Release simple drv\n");

	return 0;
}


//모듈은 등록될 때 file_operations라는 구조체를 이용해 커널에 정보를 넘겨주는데,
//내가 정의한 simple_read()함수를 이 구조체와 연결하여, 디바이스로부터 읽기를 하고 싶을 때 simple_read()를 수행한다~
struct file_operations fop = {
	.owner = THIS_MODULE,
	.open  = simple_open,
	.read  = simple_read,
	.write = simple_write,
	.release = simple_close,
};


//insmod 혹은 부트 시 module_init() 실행
int __init simple2_init(void)
{
	int err;

	if(major)
	{
		devno = MKDEV(major, 0);	//디바이스를 시스템에 추가 = MKDEV(major, minor#)
		register_chrdev_region(devno, 1, "Simple char drv");	//first, count, name: 디바이스# 시작, 디바이스# 총 수(여기선 한개만 등록하겠다~), 디바이스명
	}
	else
	{
		alloc_chrdev_region(&devno, 0, 1, "Simple char drv");	//(dev, BaseMinor, Count, Nmae): BaseMinor를 기준으로 Count만큼 minor 넘버를 할당해줌. Major는 동적으로 남는 곳에 할당됨.
																													//alloc은 알아서 남는 major#에 할당해줌.
																													//이것 덕분에 따로 # 지정 안해줘도 알아서 처리
		major = MAJOR(devno);
	}

	cdev_init(&my_cdev, &fop);	//cdev 구조체 초기화
	my_cdev.owner = THIS_MODULE;//THIS_MODULE은 모듈이 올라갈 때 모듈의 시작 주소를 가리킴.
	err = cdev_add(&my_cdev, devno, 1);	//chrdev에 대한 객체의 관리는 cdev_map이 주관. 이 cdev_map에devno을 넘겨주어 디바이스 번호 등록. 카운트는 주로 1

	if(err < 0)
	{
		printk("device add err\n");
		return -1;
	}

	printk("Hello world. this is my first char dev.\n");
	printk("sudo mknod -m 666 /dev/simple_chr_dev c %d 0\n", MAJOR(devno));
	//이 메시지를 보면 어떤 devno 할당받았는지 확인가능. MAJOR()로 해당 디바이스#의 메이저 넘버 조회.
	//mknod 경로(simple~) 모드(c 문자파일) 디바이스(devno), 우린 여기서 -m을 이용해 초기값 지정을 무시했다.

	msg = (char *)kmalloc(32, GFP_KERNEL);
	if (msg != NULL)
		printk("malloc allocator addr 0x%p\n", msg);

	return 0;
}


//rmmod 시 module_exit() 실행
void __exit simple2_exit(void)
{
	printk("Goodbye\n");

	if(msg)
	{
		unregister_chrdev_region(devno, 1);
		cdev_del(&my_cdev);
	}
}

module_init(simple2_init);
module_exit(simple2_exit);


// lsmod 로 설치된 모듈 목록 볼 수 있다.
// dmesg로 커널 메시지 확인할 수 있다.
