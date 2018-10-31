#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/delay.h>

#define BUFLEN 256

#define MY_IOCTL_NUMBER 100
#define MY_IOCTL_READ		_IOR( MY_IOCTL_NUMBER, 0, int )
#define MY_IOCTL_WRITE		_IOW( MY_IOCTL_NUMBER, 1, int )
#define MY_IOCTL_STATUS		_IO(  MY_IOCTL_NUMBER, 2 )
#define MY_IOCTL_READ_WRITE	_IOWR(MY_IOCTL_NUMBER, 3, int)
#define MY_IOCTL_NR		4

/*버젼 두개다! 주석 안지운것과 지운것
1.
sema_init(&sem, val) val==1이면 뮤텍스
down(&sem)
down_interruptible(&sem), acquire sem
up(&sem), release sem

2.
init_rwsem()
down_read(), down_write()
up_read(), up_write()
*/

struct cdev my_cdev;
dev_t devno;
static char *msg = NULL;
int major;
//struct semaphore sem;
struct rw_semaphore rwsem;	//rw_sem은 1 Writer 무한 Reader를 허용한다. 

module_param(major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(major, "major");

int ioctl_dev_open (struct inode *pinode, struct file *pfile)
{
  printk("open simple char drv\n");
/*
  printk("open sema\n");
  if(down_interruptible(&sem))
  {
    printk("could not hold sema\n"); return -1;
  }
*/
  return 0;
}


int ioctl_dev_close (struct inode *pinode, struct file *pfile)
{
  printk("release simple char drv\n");
/*
  printk("release sema\n");
  up(&sem);
*/
  return 0;
}


long char_dev_ioctl (struct file *pfile, unsigned int command, unsigned long arg)
{
  switch(command)
  {
    case MY_IOCTL_WRITE:
      down_read(&rwsem);
      printk("write simple char drv\n");
      if(copy_from_user((void*)msg, (const void*)arg, (ssize_t)5))
      {
        printk("write err\n"); return -1;
      }
      printk("%s\n", msg);

      ssleep(10); up_read(&rwsem);

      break;

    case MY_IOCTL_READ:
      down_write(&rwsem);
      printk("read simple char drv\n");
      if(strcmp(msg, "test") == 0)
	      copy_to_user((void*)arg, (void*)"GET TEST", (ssize_t)8);
      else
      	copy_to_user((void*)arg, (void*)"NOT TEST", (ssize_t)8);
      up_write(&rwsem);
      break;

    default:
      printk("Not found command\n");
      up_write(&rwsem);
      break;
  }
  return 0;
}


struct file_operations fop =
{
  .open = ioctl_dev_open,
  .release = ioctl_dev_close,
  .unlocked_ioctl = char_dev_ioctl,
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

//  sema_init(&sem, 1);
  init_rwsem(&rwsem);
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
