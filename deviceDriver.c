#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/hrtimer.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Device Driver with Time");
MODULE_AUTHOR("Vitali Ponteley");

static int waitTime = 0;
static int times = 0;
static char msg[10] = "hello\n";
static struct hrtimer htimer;
static ktime_t kt_periode;

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

static enum hrtimer_restart timer_function (struct hrtimer * unused)
{

    if (waitTime != 0) {
        printk( "%s\n", msg);
        kt_periode = ktime_set(waitTime, 0); 
    }
    hrtimer_forward_now(& htimer, kt_periode);
    return HRTIMER_RESTART;
}

int init_module(void)
{
    kt_periode = ktime_set(1, 0); 
    hrtimer_init (& htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    htimer.function = timer_function;
    hrtimer_start(& htimer, kt_periode, HRTIMER_MODE_REL);
    int t = register_chrdev(89, "deviceDriver", &fops);
    if (t < 0) printk(KERN_ALERT "Device Registration failed..\n");
    else printk(KERN_ALERT "Device Registered...\n");
    return t;
}

void cleanup_module(void)
{
    unregister_chrdev(89, "deviceDriver");
    hrtimer_cancel(& htimer);
}

static int dev_open(struct inode *inod, struct file *fil)
{
    times++;
    return 0;
}

static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
    short readPos = 0;
    while (len && (msg[readPos]!=0))
    {
	put_user(msg[readPos], buff++);
	readPos++;
	len--;
    }
    return readPos;
}

static ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    sscanf(buff, "%d", &waitTime);
    return len;
}

static int dev_rls(struct inode *inod, struct file *fil)
{
    times--;
    printk(KERN_ALERT "Device closed\n");
    return 0;
}
