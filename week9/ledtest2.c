//ledtest_2.c
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LED 18

int main()
{
	if (wiringPiSetupGpio() == -1)
		return -1;
	
	pinMode(LED, PWM_OUTPUT);
	
	while(1)
	{
		pwmWrite(LED, 0);
		delay(500);
		pwmWrite(LED, 1024);
		delay(500);
	}

	return 0;

}

/*
struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = ledtest_open,
	.release = ledtest_close,
};

int __init ledtest_init(void){
	printk(KERN_ALERT "INIT ledtest\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	return 0;
}

void __exit ledtest_exit(void){
	printk(KERN_ALERT "EXIT ledtest\n");
	unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(ledtest_init);
module_exit(ledtest_exit);
*/