#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

static struct task_struct *thread1;

int __init vma_init(void)
{
	printk("vma module init\n");
	struct task_struct *task;
	struct mm_struct *mm;
	struct vm_area_struct *mmap;
	/*
	vm_area_struct는 Process Memory 정보를 유지하는데 사용된다.
	전체는 링크트 리스트처럼 연결되어 있으며, 따라서 vm_next를 따라가면
	태스크가 사용하는 전체 가상메모리 공간을 확인할 수 있다.
	*/

	task = current;
	mm = task->mm;
	mmap = mm->mmap;

	printk("task_id = %d\n", task->pid);
	do
	{
		printk("vm_start 	= %8x ", mmap->vm_start);
		printk("vm_end 		= %8x ", mmap->vm_end);
		printk("vm_area 	= %dkb\n ", (mmap->vm_end - mmap->vm_start) / 1024);
	}while(mmap = mmap->vm_next);

	return 0;
}


void __exit vma_exit(void)
{
	printk("vma exit");
}

module_init(vma_init);
module_exit(vma_exit);
