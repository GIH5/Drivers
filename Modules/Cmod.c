#include <linux/init.h>											/*For init and cleanup functions*/
#include <linux/module.h>										/*For providing the functions and symbols*/
#include <linux/fs.h>											/*File system usage*/
#include <linux/cdev.h>											/*Implements the char device and functionality*/
#include <linux/kdev_t.h>  										/*kdev_t and device is to automatically create the device file*/
#include <linux/device.h>										/*DO NOT TOUCH THEM PLEASE*/
#include <linux/err.h>
#define dev	501
#define min	0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabe");
MODULE_DESCRIPTION("Char device");

static struct cdev c_cdev;
static struct class *c_dev_class;
/*static struct device *c_dev_crt;*/
int ident, resp;

/*********function-prototypes***********/
static ssize_t  c_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t  c_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
static int 	c_open(struct inode *inode, struct file *file);
static int 	c_release(struct inode *inode, struct file *file);
/*-------------------------------------*/

static int c_open(struct inode *inode, struct file *file) {

	 pr_info("Call to open");
	 return 0;
};

static ssize_t c_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {

         pr_info("Call to read");
         return 0;
};

static ssize_t c_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {

	pr_info("Call to write");
	return 0;
};

static int c_release(struct inode *inode, struct file *file) {

	 pr_info("Call to release");
	 return 0;
};

static struct file_operations fops = {
	.owner	 =	THIS_MODULE,
	.read	 =	c_read,
	.write	 =	c_write,
	.open	 =	c_open,
	.release =	c_release
};

static int __init Load(void) {									       /*Check if we can staticlly assign a device number.*/
	if (dev) {
	   ident = MKDEV(dev, min);
	   resp = register_chrdev_region(ident, 1, "Cmod");

	   if (resp < min) {
              pr_info("Can't register using: register_chrdev_region() - MEM_INFO: 0x%p", resp);
	      return resp;
	   }

	   pr_info("Module loaded! Device identifier - MAJOR(%d):MINOR(%d)", dev, min);
	   goto cdev_init;
        }
	else {
	   resp = alloc_chrdev_region(&ident, min, 1, "Cmod");
	   pr_info("Module Loaded! Device identifier - MAJOR(%d):MINOR(%d)", dev, min);
														/*If number assigned is not valid*/
	   if (resp < min) {
	      pr_err("Failure to allocate a device number\n Device identifier: %d", ident);
	      return resp;
	   }

	   goto cdev_init;
	};

	cdev_init:
		pr_info("Implementing charecter device...\n");
		cdev_init(&c_cdev, &fops);
		pr_info("Charecter device initiated!");

		cdev_add(&c_cdev, ident, 1);
		pr_info("Charecter device added");

		/*c_dev_class = class_create(THIS_MODULE, "Cclass");*/
		/*c_dev_crt = device_create(c_dev_class, NULL, dev, NULL, "C-device");*/

		if (IS_ERR(c_dev_class = class_create(THIS_MODULE, "Cclass"))) {
		   pr_err("class_create() function error caught here!");
		   unregister_chrdev_region(dev, 1);
		   return -1;
                }

               if (IS_ERR(device_create(c_dev_class, NULL, dev, NULL, "C-device_1"))) {
		  pr_err("device_create() function error caught here!");
		  class_destroy(c_dev_class);
               }

		/*
		if (c_dev_class == NULL || c_dev_crt == NULL) {
		   pr_err("An class_create() or device_create() exception was caught here");
		   class_destroy(c_dev_class);
		   return -1;
		}
		*/

		 return 0;

};

static void __exit Unload(void) {
	cdev_del(&c_cdev);
	unregister_chrdev_region(ident, 1);
	device_destroy(c_dev_class, dev);									/*Must have device_destroy() before class_destroy() to avoid a NULL pointer dereference*/
	class_destroy(c_dev_class);
	pr_info("Unloaded module");
};

module_init(Load);
module_exit(Unload);
