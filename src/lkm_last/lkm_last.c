#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pierre-Yves Aillet");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

const char* msg[] = {"\n",
"   KERNEL PANIC ! The Talk is over.\n",
"   Thank you !\n",
"\n",
"   ▄▄▄▄▄▄▄  ▄  ▄▄  ▄▄▄▄  ▄▄▄▄▄▄▄  \n",
"   █ ▄▄▄ █ ▄▄█▀█▄█▀█▀ ▀█ █ ▄▄▄ █  \n",
"   █ ███ █  ▀█▀ ▀█▀ ▄██  █ ███ █  \n",
"   █▄▄▄▄▄█ ▄▀▄ ▄▀▄ ▄▀█▀█ █▄▄▄▄▄█  \n",
"   ▄▄▄ ▄▄▄▄█▀█▄▀▀█▄██ █ ▄▄   ▄    \n",
"   █▄    ▄ ▄▄▀▀ ▄▀▀▄   █▄█ ▄▀▄▄█  \n",
"   ██▄  █▄▄▀  ▀▄  ███ ▀ ▄▄▀▀▄ █▄  \n",
"   ▄▄▀▄▀ ▄▀ ▄ ██▀▄ ▀▀  ▀▀█ ▄█ ▄█  \n",
"   ▀▄ ▀█▀▄▀▀█▄▄▀▀█▀██▀▄█▀█▄ █ █▄  \n",
"   ▄▀▄ ▀▀▄▀▀█▀▀ ▄▀ █▀▀  █▀▄▄▀▀▄█  \n",
"   ▄▀  ██▄▄██▄▀▄  ▄██▄██████▀ ▀   \n",
"   ▄▄▄▄▄▄▄ █ ▄██▀▄ █▄ ██ ▄ █▄▀██  \n",
"   █ ▄▄▄ █ ██ ▄▀▀█  █▀ █▄▄▄█▀ ▀█  \n",
"   █ ███ █ ▄ █▀ ▄▀ ▀▄▄▄▀  ██▄▀▀█  \n",
"   █▄▄▄▄▄█ █▀▀▀▄  ▀██▀███ ▄█  █▄  \n",
"\n",
"   https://github.com/pyaillet/kernel\n"};

static int __init lkm_example_init(void) {
 int i;
 for(i=0; i<21; ++i) {
  printk( KERN_ERR "%s", msg[i]);
 }
 msleep(500);
 panic("STOP");
 return 0;
}

static void __exit lkm_example_exit(void) {
 printk(KERN_INFO "Goodbye, World! 👋 \n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);

