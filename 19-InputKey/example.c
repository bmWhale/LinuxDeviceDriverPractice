#include <linux/init.h>
#include <linux/module.h>
#include <linux/input.h>

#define KEY_SCAN_MS 500
static struct input_dev *button_dev;
struct timer_list k_timer;

static int adc_key_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
	pr_info("<SIMPLE-INPUT>: Key Type=%d, code=%d, value=%d\n",type,code,value);
	return 0;
}

static int adc_key_open(struct input_dev *dev)
{
	pr_info("<SIMPLE-INPUT>: %s\n", __FUNCTION__);
	return 0;
}

static void adc_key_close(struct input_dev *dev)
{
	pr_info("<SIMPLE-INPUT>: %s\n", __FUNCTION__);
}

static int adc_key_flush(struct input_dev *dev, struct file *file)
{
	pr_info("<SIMPLE-INPUT>: %s\n", __FUNCTION__);
	return 0;
}

static void adc_key_scan(struct timer_list *kk_timer)
{
	static unsigned int count=0;
	switch(count) {
	case 0:
		count=2;
		pr_info("<SIMPLE-INPUT>: %s send KEY_DOWN, key = %x\n", __FUNCTION__, BTN_0);
		input_report_key(button_dev, BTN_0, 1);
		//mod_timer(kk_timer,jiffies + msecs_to_jiffies(KEY_SCAN_MS));
		mod_timer(kk_timer,jiffies + HZ);
		break;
	case 1:
		count=2;
		pr_info("<SIMPLE-INPUT>: %s send KEY_DOWN, key = %x\n", __FUNCTION__, BTN_0);
		input_report_key(button_dev, BTN_0, 1);
		mod_timer(kk_timer,jiffies + msecs_to_jiffies(KEY_SCAN_MS));
		break;
	case 2:
	default:
		count=0;
		pr_info("<SIMPLE-INPUT>: %s send KEY_UP, key = %x\n", __FUNCTION__, BTN_0);
		input_report_key(button_dev, BTN_0, 0);
		mod_timer(kk_timer,jiffies + HZ);
		break;
	}
	input_sync(button_dev);
}

static int __init button_init(void)
{
	int error;
	pr_info("<SIMPLE-INPUT>: %s\n", __FUNCTION__);
	button_dev = input_allocate_device();
	if (!button_dev) {
		printk(KERN_ERR "button.c: Not enough memory\n");
		error = -ENOMEM;
		goto err_free_dev;
	}
	button_dev->name = "Simple Keyboard";
	button_dev->phys = "Simple/input0";
	button_dev->id.bustype = BUS_HOST;
	button_dev->id.vendor  = 0x0001;
	button_dev->id.product = 0x0001;
	button_dev->id.version = 0x0001;
	button_dev->keycode = NULL;
	button_dev->keycodesize = sizeof(unsigned char);
	button_dev->keycodemax = 0;
	button_dev->event = adc_key_event;
	button_dev->open = adc_key_open;
	button_dev->close = adc_key_close;
	button_dev->flush = adc_key_flush;
	button_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP)|BIT_MASK(EV_PWR);
	button_dev->keybit[BIT_WORD(BTN_0)] = BIT_MASK(BTN_0);
	/* poll key */
	timer_setup(&k_timer, adc_key_scan, 0);
	k_timer.expires = jiffies+msecs_to_jiffies(KEY_SCAN_MS);
	add_timer(&k_timer);

	error = input_register_device(button_dev);
	if (error) {
		printk(KERN_ERR "button.c: Failed to register device\n");
		goto err_free_dev;
	}

	return 0;

err_free_dev:
	input_free_device(button_dev);
	return error;
}

static void __exit button_exit(void)
{
	pr_info("<SIMPLE-INPUT>: %s\n", __FUNCTION__);
	del_timer(&k_timer);
	input_unregister_device(button_dev);
}

module_init(button_init);
module_exit(button_exit);

MODULE_AUTHOR("bmWhale, <bryanmonsterw@gmail.com>");
MODULE_DESCRIPTION("Simple Input driver");
MODULE_LICENSE("Dual BSD/GPL");
