/*Dynamic character device driver program*/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>/*copy to user and copy from user*/
#include<linux/gpio.h> /*for gpio */
#include<linux/err.h>
#include<linux/delay.h>

//Led is connected to this GPIO

#define GPIO_60 (60)
dev_t dev=0;

static struct class *dev_class;
static struct cdev my_cdev;

static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);


static int my_open(struct inode *inode, struct file *filp);
static int my_release(struct inode *inode,struct file *filp);
static ssize_t my_write(struct file *filp,const char *buf,size_t len,loff_t *off);


static struct file_operations fops =
{
	.owner   = THIS_MODULE,
	.write   = my_write,
	.open    = my_open,
	.release = my_release,
};

/*function will be called when we open the device file */

static int my_open(struct inode *inode, struct file *filp)
{

	printk(KERN_INFO "Device file opened..\n");
	return 0;
}
/*function will be called when we close the device file*/

static int my_release(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Device File is closed..\n");
	return 0;
}

/*function will be called when we write the device file*/
static ssize_t my_write(struct file *filp,const char __user *buf,size_t len,loff_t * off)
{
	int i;
	for(i = 0;i<10;i++)
	{
		//set the GPIO value to HIGH
		gpio_set_value(GPIO_60, 1);
 		msleep(2);
		//set the GPIO value to LOW
		gpio_set_value(GPIO_60, 0);
		msleep(2);
	}
	 
	return len;
}

static int __init chr_driver_init(void)
{
	/*Allocating a Major number */

	if((alloc_chrdev_region(&dev,0,1 ,"arr_dev"))<0)
	{
		printk(KERN_INFO "cannot allocate the major number..\n");
		return -1;		
	}
	printk(KERN_INFO"major =%d minor =%d..\n",MAJOR(dev),MINOR(dev));

	/*Creating cdev structure */

	cdev_init(&my_cdev, &fops);

	/*adding character device to the system */

	if((cdev_add(&my_cdev,dev,1))<0)
	{
		printk(KERN_INFO "cannot add the device to the syatem..\n");
	}

	/*creating struct class*/
	if((dev_class = class_create(THIS_MODULE,"my_class"))==NULL)
	{
		printk(KERN_INFO "cannot create the struct class..\n");
	
	}
	/*creating device */

	if((device_create(dev_class,NULL,dev,NULL,"my_device")) ==NULL)
	{
		printk(KERN_INFO "cannot create the device..\n");
	}

	/*Requesting the gpio*/

	if(gpio_request(GPIO_60,"GPIO_60")<0)
	{
		printk(KERN_INFO"ERROR:gpio%d request\n",GPIO_60);
	}

	/*configure the gpio as output*/

	gpio_direction_output(GPIO_60,0);

	gpio_export(GPIO_60,false);


	printk(KERN_INFO"device driver insert...done properly..\n");
	return 0;

}

/*module exit function*/


static void __exit chr_driver_exit(void)
{
	gpio_unexport(GPIO_60);
	gpio_free(GPIO_60);
	device_destroy(dev_class,dev);
	cdev_del(&my_cdev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev,1);
	printk(KERN_INFO" device driver is removed sucessfully..\n");

}
module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chamska");
MODULE_DESCRIPTION("character device driver dynamically ");



