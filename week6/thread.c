#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *thread1;

int thread_fn(void *p)
{
	while(!kthread_should_stop())
	{
		//should_stop 뜰 때까지 계속 돌면서 10초마다 running msg 찍어줌
		printk("thread running\n");
		ssleep(10);
	}
	printk("thread done\n");

	return 0;
}


int __init thread_init (void)
{
	printk("init thread testing module\n");

	thread1 = kthread_create(thread_fn, NULL, "thread1");
	if((thread1))
	{
		wake_up_process(thread1);
		printk("success to create thread\n");
	}

	return 0;
}


void __exit thread_exit(void)
{
	if(kthread_stop(thread1) != EINTR)
		printk("thread stop");
}

module_init(thread_init);
module_exit(thread_exit);
