#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/device.h>


MODULE_LICENSE("GPL");

dev_t dev_no;

struct my_char_dev
{
	struct cdev cdev;
	
};
struct my_char_dev *myDev;

int dev_open(struct inode *inode, struct file *filp)	
{
	printk(KERN_NOTICE "device open\n");
	return 0;
}

int dev_release(struct inode *inode, struct file *filp)	
{
	printk(KERN_NOTICE "device release\n");	
	return 0;
}
static ssize_t dev_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE "device read\n");	
	return 0;
}


static const struct file_operations my_char_dev_fops =
{
	.owner = THIS_MODULE,
	.open = dev_open,
	.release = dev_release,
	.read = dev_read,
};

static void setupMyCharDev(struct my_char_dev *myDev, int index)
{
	int ret = 0;
	
	cdev_init(&myDev->cdev, &my_char_dev_fops);
	myDev->cdev.owner = THIS_MODULE;
	myDev->cdev.ops = &my_char_dev_fops;
	
	ret = cdev_add(&myDev->cdev, dev_no, 1);
	if (ret < 0)
	{
		printk(KERN_EMERG "ADD DEV FAILD\n");
	}
}

static int __init my_init(void)
{
	int ret;
	int dev_major = 0;
	
	ret =  alloc_chrdev_region(&dev_no, 0, 1, "myCharTest");
	dev_major=MAJOR(dev_no);
	
	if(ret < 0)
	{
		printk(KERN_EMERG "CHAR DEV INIT FAIL\n");
		return ret;
	}
	myDev = kmalloc(sizeof(struct my_char_dev), GFP_KERNEL);
	if(!myDev)
	{
		ret = -ENOMEM;
		printk(KERN_EMERG "ALLOC MEM FAIL\n");
		goto fail_malloc;
	}
	memset(myDev, 0, sizeof(struct my_char_dev));
	setupMyCharDev(myDev, 0);

	//myDev_class = class_create(THIS_MODULE, "myCharDev");
	//device_create(myDev_class, NULL, MKDEV(dev_major, 0), NULL, "myDevice1");
	
	printk(KERN_EMERG "hello, I'm init\r\n");
	return 0;
    fail_malloc:
	    unregister_chrdev_region(dev_no, 1);
	return 0;

}

static void __exit my_exit(void)
{
	printk(KERN_EMERG "bye, I'm exit\r\n");
	if(myDev)
	{
		cdev_del(&myDev->cdev);
		kfree(myDev);
	}
	unregister_chrdev_region(dev_no, 1);
}

module_init(my_init);
module_exit(my_exit);
