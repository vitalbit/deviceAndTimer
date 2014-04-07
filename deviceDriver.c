#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <time.h>
#include <sys/times.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Device Driver with Time");
MODULE_AUTHOR("Vitali Ponteley");

static int waitTime = 0;

static int dev_open(struct inode *, struct file *);
static int dev_rls(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
    .read = dev_read,
    .open = dev_open,
    .write = dev_write,
    .release = dev_rls,
};

int init_module(void)
{
    int t = register_chrdev(89, "myDev", &fops);
    if (t < 0) printk(KERN_ALERT "Device Registration failed..\n");
    else printk(KERN_ALERT "Device Registered...\n");
    return t;
}

void cleanup_module(void)
{
    unregister_chrdev(89, "myDev");
}

static int dev_open(struct inode *inod, struct file *fil)
{
    return 0;
}

static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
    return count;
}

static ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    clock_t ct0, ct1;
    struct tms tms0, tms1;
    sscanf(buff, "%d", waitTime);
    if (ct0 = times (&tms0)) == -1)
	perror("times");
    while (1)
    {
	if (ct1 = times (&tms11)) == -1)
	    perror("times");
	if (ct1 - ct0 >= waitTime) {
	    ct0 = ct1;
            printk(KERN_ALERT "Hello!\n");
        }
    }
    return len;
}

static int dev_rls(struct inode *inod, struct file *fil)
{
    printk(KERN_ALERT "Device closed\n");
    return 0;
}
