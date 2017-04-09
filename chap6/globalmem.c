#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MEM_SIZE	0x1000
#define MEM_CLEAR	0x1
#define MEM_MAJOR	230

static int mem_major = MEM_MAJOR;
module_param(mem_major, int, S_IRUGO);

struct mem_dev {
	struct cdev cdev;
	unsigned char mem[MEM_SIZE];
};

struct mem_dev *mem_devp;

static int mem_open(struct inode *inode, struct file *filp)
{
	filp->private_data = mem_devp;
	return 0;
}

static int mem_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t mem_read(struct file *filp, char __user *buf, size_t size,
		loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct mem_dev *dev = filp->private_data;

	if (p > MEM_SIZE)
		return 0;
	if (count > MEM_SIZE - p)
		count = MEM_SIZE - p;

	if (copy_to_user(buf, dev->mem + p, count)) {
		return -EFAULT;
	}
	else {
		*ppos += count;
		ret = count;
	}
	return ret;
}

static ssize_t mem_write(struct file *filp, const char __user *buf, size_t size,
		loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct mm_dev *dev = filp->private_data;

	if (p > MEM_SIZE)
		return 0;
	if (count > MEM_SIZE - p)
		count = MEM_SIZE - p;
#if 1
	if (copy_from_user(dev->mem + p, buf, count))
		ret = -EFAULT;
	else {
		*ppos += count;
		ret = count;
	}
#endif
	return ret;
}

static loff_t mem_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	switch (orig) {
	case 0:
		if (offset < 0) {
			ret = -EINVAL;
			break;
		}
		if ((unsigned int)offset > MEM_SIZE) {
			ret = -EINVAL;
			break;
		}

		filp->f_pos = (unsigned int)offset;
		ret = filp->f_pos;
		break;
	case 1:
		if ((filp->f_pos + offset) > MEM_SIZE) {
			ret = -EINVAL;
			break;
		}
		if ((filp->f_pos + offset) < 0) {
			ret = -EINVAL;
			break;
		}

		filp->f_pos += offset;
		ret = filp->f_pos;
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static long mem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct mem_dev *dev = filp->private_data;

	switch (cmd) {
	case MEM_CLEAR:
		memset(dev->mem, 0, MEM_SIZE);
		break;
	default:
		return -EINVAL;
		break;
	}
	return 0;
}

static const struct file_operations mem_fops = {
	.owner = THIS_MODULE,
	.open = mem_open,
	.release = mem_release,
	.read = mem_read,
	.write = mem_write,
	.llseek = mem_llseek,
	.unlocked_ioctl = mem_ioctl,
};



static void mem_setup_dev(struct mem_dev *dev, int index)
{
	int err;
	int devno = MKDEV(mem_major, 0);

	cdev_init(&dev->cdev, &mem_fops);
	dev->cdev.owner = THIS_MODULE;

	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
		printk("cdev_add failed\n");
}

static int __init mem_init(void)
{
	int ret;
	dev_t devno = MKDEV(mem_major, 0);

	if (mem_major)
		ret = register_chrdev_region(devno, 1, "mem_dev");
	else {
		ret = alloc_chrdev_region(&devno, 0, 1, "mem_dev");
		mem_major = MAJOR(devno);
	}
	if (ret < 0)
		return ret;

	mem_devp = kzalloc(sizeof(struct mem_dev), GFP_KERNEL);
	if (!mem_devp) {
		ret = -ENOMEM;
		goto fail_malloc;
	}

	mem_setup_dev(mem_devp, 0);
	return 0;

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return ret;
}

static void __exit mem_exit(void)
{
	cdev_del(&mem_devp->cdev);
	kfree(mem_devp);
	unregister_chrdev_region(MKDEV(mem_major, 0), 1);
}

module_init(mem_init);
module_exit(mem_exit);

MODULE_LICENSE("GPL");
