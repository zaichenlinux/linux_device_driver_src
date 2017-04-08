#include <linux/module.h>
#include <linux/init.h>
#include <liinux/kernel.h>

#define LED_MAJOR 230

/* �豸�ṹ�� */
struct led_dev {
	struct cdev cdev;    //�ַ��豸�ṹ��cdev
	unsigned char value; //LED ��ʱΪ1, ��ʱΪ0
};

struct led_dev *led_devp;
int led_major = LED_MAJOR;

/* �򿪺͹رպ��� */
int led_open(struct inode *inode, struct file *filp)
{
	struct led_dev *led_devp;
	//����豸�ṹ��
	led_devp = container_of(inode->i_cdev, struct led_dev, cdev);
	//�豸�ṹ����Ϊ�豸��˽����Ϣ
	filp->private_data = led_devp;

	return 0;
}

int led_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/* ��д�豸 */
ssize_t led_read(struct file *filp, char __user *buf, size_t count,
		loff_t *f_pos)
{
	struct led_dev *led_devp = filp->private_data; //����豸�ṹ��
	if (copy_to_user(buf, &(led_devp->value), 1))
		return -EFAULT;
	return 1;

}


ssize_t led_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *f_pos)
{
	struct led_dev *led_devp = filp->private_data; //����豸�ṹ��
	if (copy_from_user(&(led_devp->value), buf, 1))
		return -EFAULT;

	/* ����д���ֵ����LED ��Ϩ�� */
	if (led_devp->value == 1)
		light_on();
	else
		light_off();
	return 1;
}

/* ioctl ���� */
int led_ioctl(struct inode *inode, struct file *filp, unsigned int cmd,
		unsigned log arg)
{
	struct led_dev *led_devp = filp->private_data;

	switch (cmd) {
	case LIGHT_ON:
		led_devp->value = 1;
		light_on();
		break;
	case LIGHT_OFF:
		led_devp->value = 0;
		light_off();
		break;
	default:
		/* ����֧�ֵ����� */
		return -ENOTTY;
	}

	return 0;
}

struct file_operations led_ops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.read = led_read,
	.write = led_write,
	.release = led_release,
	.ioctl = led_ioctl,
};

/* �����ַ��豸�ṹ�� */
static void led_setup_cdev(struct led_dev *led_devp, int index)
{
	int err;
	int devno;
	devno = MKDEV(led_major, index);

	cdev_init(&(led_devp->cdev), &led_ops);///

	led_devp->cdev.owner = THIS_MODULE;///
	led_devp->cdev.ops = &led_ops;///
	err = cdev_add(&led_devp->cdev, devno, 1);///
	if (err)
		printk(KERN_NOTICE "error %d adding led%d", err, index);
}

/* ģ����غ��� */
int led_init(void)
{
	int ret;
	dev_t dev = MKDEV(led_major, 0);
	/* �����ַ��豸�� */
	if (led_major)
		ret = register_chrdev_region(dev, 1, "LED");
	else {
		ret = alloc_chrdev_region(&dev, 0, 1, "LED");
		led_major = MAJOR(dev);
	}

	if (ret < 0)
		return ret;

	/* �����豸�ṹ����ڴ� */
	led_devp = kmalloc(sizeof(struct led_dev), GFP_KERNEL);
	if (!led_devp) {
		ret = -ENOMEM;
		goto fail_malloc;
	}

	memset(led_devp, 0, sizeof(struct led_dev));
	led_setup_cdev(led_devp, 0);///
	led_gpio_init();
	return 0;

fail_malloc:
	unregister_chrdev_region(dev, led_devp);
	return ret;
}

/* ģ��ж�غ��� */
void led_cleanup(void)
{
	cdev_del(&led_devp->cdev);//ɾ���ַ��豸�ṹ��
	kfree(led_devp);//�ͷŷ�����ڴ�ռ�
	unregister_chrdev_region(MKDEV(led_major, 0), 1);//ɾ���ַ��豸
}

module_init(led_init);
module_exit(led_cleanup);

MODULE_LICENSE("GPL");
