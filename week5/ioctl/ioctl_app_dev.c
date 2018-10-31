#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define BUFLEN 256

/*
-read(), write()와 달리 디바이스의 복잡한 기능을 수행하거나 상태를 얻기 위해 사용
-왜?  file_operations 구조체에 존재하는 함수의 갯수에는 제한이 있으므로, 여기에 적혀있는것 외에 다른 함수를
 사용할 경우엔 ioctl 사용하는 것이 적절하다.
-Unique function for each Device!
-ioctl(fd, REQUEST(요청코드 매크로), arg)
  return 0 if success, -1 if fail
*/


#define MY_IOCTL_NUMBER 100	//magic number
/*
SELF DEFINED CMD
드라이버 입장에서 어떤 어플리케이션에서 요청이 왔는지 구분할 수 있게 해주는 요소
0~7 	구분번호: 각 커맨드를 구분하는 명령어 순차번호
8~15 	매직넘버: 다른 드라이버의 ioctl과 구분용, A to Z or a to z(0~255)
16~29	Size: arg를 통해 받는 데이터 크기
29~31: Direction: R인지 W인지 구분용
*/

/*
CMD 명령을 만드는 매크로 함수
_IO(매직넘버, 구분번호)	: 부가적 데이터가 없는 명령을 만드는 매크로
_IOR(매직넘버, 구분번호, 변수형)	: 드라이버에서 데이터를 읽어오기 위한 명령을 만드는 매크로
*/
#define MY_IOCTL_READ			_IOR( MY_IOCTL_NUMBER, 0, int )
#define MY_IOCTL_WRITE		_IOW( MY_IOCTL_NUMBER, 1, int )
#define MY_IOCTL_STATUS		_IO(  MY_IOCTL_NUMBER, 2 )
#define MY_IOCTL_READ_WRITE	_IOWR(MY_IOCTL_NUMBER, 3, int)
#define MY_IOCTL_NR		4

/*
CMD 명령을 해석하는 함수
_IOC_NR : 구분번호를 읽는 함수
_IOC_TYPE: 매직넘버를 읽는 함수
_IOC_SIZE, _IOC_DIR
*/

struct cdev my_cdev;
dev_t devno;
static char *msg = NULL;
int major;

module_param(major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major, "major");

int ioctl_dev_open (struct inode *pinode, struct file *pfile)
{
  printk("open simple char drv\n");
  return 0;
}


int ioctl_dev_close (struct inode *pinode, struct file *pfile)
{
  printk("release simple char drv\n");
  return 0;
}


long char_dev_ioctl (struct file *pfile, unsigned int command, unsigned long arg)
{
  switch(command)
  {
    case MY_IOCTL_WRITE:
      printk("write simple char drv\n");
      if(copy_from_user((void*)msg, (const void*)arg, (ssize_t)5))
      {
        printk("write err\n"); return -1;
      }
      printk("%s\n", msg);
      break;

    case MY_IOCTL_READ:
      printk("read simple char drv\n");
      if(strcmp(msg, "test") == 0)
      	copy_to_user((void*)arg, (void*)"GET TEST", (ssize_t)8);
      else
      	copy_to_user((void*)arg, (void*)"NOT TEST", (ssize_t)8);
      break;

    default:
      printk("Not found command\n");
      break;
  }
  return 0;
}


struct file_operations fop =
{
  .open = ioctl_dev_open,
  .release = ioctl_dev_close,
  .unlocked_ioctl = char_dev_ioctl,
  //unlocked_ioctl은 file_operations 구조체에 있다.
  //BKL(Big Kernel Lock, 모든 CPU에 대한 락)을 사용하지 않고 자기 자신의 락을 사용.
};


int __init ioctl_init_module(void)
{
  int err;
  if(major)
  {
    devno = MKDEV(major, 0);
    register_chrdev_region(devno, 1, "ioctl_app_dev");
  }
  else
  {
    alloc_chrdev_region(&devno, 0, 1, "ioctl_app_dev");
    major= MAJOR(devno);
  }

  cdev_init(&my_cdev, &fop);
  my_cdev.owner = THIS_MODULE;
  err = cdev_add(&my_cdev, devno, 1);

  if(err < 0)
  {
    printk("device add err\n");
    return -1;
  }

  printk("hello world!!.\n");
  printk("sudo mknod -m 666 /dev/ioctl_app_dev c %d 0\n", MAJOR(devno));

  msg = (char *)kmalloc(32, GFP_KERNEL);
  if(msg != NULL)
    printk("malloc alloc addr: 0x%p\n", msg);

  return 0;
}


void __exit ioctl_exit_module(void)
{
  printk("goodbye\n");
  if(msg)
    kfree(msg);

  unregister_chrdev_region(devno, 1);
  cdev_del(&my_cdev);
}

module_init(ioctl_init_module);
module_exit(ioctl_exit_module);
MODULE_LICENSE("GPL");
