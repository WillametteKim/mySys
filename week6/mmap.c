#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#include <linux/mm.h>  /* mmap related stuff */

struct cdev my_cdev;
static char *msg = NULL;
dev_t devno;

struct mmap_info {
	char *data; /* the data */
	int reference; /* how many times it is mmapped */
};

/* keep track of how many times it is mmapped */
void mmap_open(struct vm_area_struct *vma)
{
	struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
	info->reference++;
}

/* decrement reference cout */
void mmap_close(struct vm_area_struct *vma)
{
	struct mmap_info *info = (struct mmap_info *)vma->vm_private_data;
	info->reference--;
}

int mmap_fault(struct vm_fault *vmf)
{
	struct page *page;
	struct mmap_info *info;
	unsigned long address = (unsigned long)vmf->address;;
	struct vm_area_struct *vma;
	vma = vmf->vma;

	// is the address valid?
	if (address > vma->vm_end) {
		printk("invalid address");
		return VM_FAULT_SIGBUS;
	}
	// the data is in vma->vm_private_data 
	info = (struct mmap_info *)vma->vm_private_data;
	if (!info->data) {
		printk("no data");
		return VM_FAULT_SIGBUS;
	}

	// get the page 
	page = virt_to_page(info->data);

	// increment the reference count of this page
	get_page(page);
	// type is the page fault type 
	vmf->page = page;
	return 0;
}

struct vm_operations_struct mmap_vm_ops = {
	.open =     mmap_open,
	.close =    mmap_close,
	.fault =    mmap_fault,
};


//!!!HERE
int simple_mmap(struct file *filp, struct vm_area_struct *vma)
{
	printk("simple mem mapping\n");
	vma->vm_ops = &mmap_vm_ops;
	vma->vm_flags |= VM_IO;

	//assign file private data to the vm private data
	vma->vm_private_data = filp->private_data;

	return 0;
}


//!!!!!!HERE
int simple_mmap_open (struct inode *pinode, struct file *pfile)
{
	/*
	mmap은 파일 거치는 read write와 달리 페이지 잡아놓고 바로 작업하므로 굉장히 빠르다
	*/

	printk("open mmap modulee\n");
	struct mmap_info *info;

	info = kmalloc(sizeof(struct mmap_info), GFP_KERNEL);

	//get new mem
	info->data = (char *)get_zeroed_page(GFP_KERNEL);

	//write
	memcpy(info->data, "hello from kernel", 17);

	//assign this info to the file
	pfile->private_data = info;

	return 0;
}


//!!!!!!!!!!!!!HERE
int simple_mmap_close (struct inode *pinode, struct file *pfile)
{
	printk("releas mmap module\n");

	struct mmap_info *info = pfile->private_data;
	free_page((unsigned long)info->data);
	kfree(info);
	pfile->private_data = NULL;

	return 0;
}

struct file_operations fop = {
	.owner = THIS_MODULE,
	.open = simple_mmap_open,
	.release = simple_mmap_close,
	.mmap = simple_mmap,
};

int __init simple_mmap_init(void)
{
	int err;

	alloc_chrdev_region(&devno, 0, 1, "mmap drv");

	cdev_init(&my_cdev, &fop);
	my_cdev.owner = THIS_MODULE;
	err = cdev_add(&my_cdev, devno, 1);

	if(err < 0) {
		printk("Device add error\n");
		return -1;
	}

	printk("'sudo mknod -m 666 /dev/mmap c %d 0'.\n", MAJOR(devno));

	msg = (char *)kmalloc(32, GFP_KERNEL);
	if (msg != NULL)
		printk("malloc allocator address: 0x%p\n", msg);

	return 0;
}

void __exit simple_mmap_exit(void)
{
	printk("Goodbye\n");

	if(msg) {
		kfree(msg);
	}
	unregister_chrdev_region(devno, 1);
	cdev_del(&my_cdev);
}	

module_init(simple_mmap_init);
module_exit(simple_mmap_exit);
