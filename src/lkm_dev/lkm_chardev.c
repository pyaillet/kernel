#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#define  DEVICE_NAME "tzchar"
#define  CLASS_NAME  "tz"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pierre-Yves Aillet");
MODULE_DESCRIPTION("A simple Linux char driver for the TZ 🦆");
MODULE_VERSION("0.1");

static int    majorNumber;
static char   message[256] = {0};
static short  size_of_message;
static int    numberOpens = 0;
static struct class*  charClass  = NULL;
static struct device* charDevice = NULL;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static int __init char_init(void){
   printk(KERN_INFO "TzChar: Initializing the Char LKM\n");

   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "TzChar failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "TzChar: registered correctly with major number %d\n", majorNumber);

   charClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(charClass)){
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(charClass);
   }
   printk(KERN_INFO "TzChar: device class registered correctly\n");

   charDevice = device_create(charClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(charDevice)){
      class_destroy(charClass);
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(charDevice);
   }
   printk(KERN_INFO "TzChar: device class created correctly\n");
   return 0;
}

static void __exit char_exit(void){
   device_destroy(charClass, MKDEV(majorNumber, 0));
   class_unregister(charClass);
   class_destroy(charClass);
   unregister_chrdev(majorNumber, DEVICE_NAME);
   printk(KERN_INFO "TzChar: Goodbye from the LKM!\n");
}

static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "TzChar: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   error_count = copy_to_user(buffer, message, size_of_message);

   if (error_count==0){
      printk(KERN_INFO "TzChar: Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);
   }
   else {
      printk(KERN_INFO "TzChar: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;
   }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%zu letters)", buffer, len);
   size_of_message = strlen(message);
   printk(KERN_INFO "TzChar: Received %zu characters from the user\n", len);
   return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "TzChar: Device successfully closed\n");
   return 0;
}

module_init(char_init);
module_exit(char_exit);
