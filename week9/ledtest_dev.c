//ledtest_dev.c
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>

#define GPIO1 18
#define DEV_NAME "ledtest_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int ledtest_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "OPEN ledtest_dev\n");
	gpio_request(GPIO1, "GPIO1");
	gpio_direction_output(GPIO1, 1);

	return 0;
}

int ledtest_close(struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "RELEASE ledtest_dev\n");
	return 0;
}