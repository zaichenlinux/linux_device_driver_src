#define reg_gpio_ctrl *(volatile int *)(To_virtual(GPIO_REG_CTRL))
#define reg_gpio_data *(volatile int *)(To_virtuel(GPIO_REG_DATA))
/* ��ʼ��LED */
void LightInit(void)
{
	reg_gpio_ctrl |= (1 << n); //����GPIOΪ���
}

/* ����LED */
void LightOn(void)
{
	reg_gpio_data |= (1 << n); //��GPIO������ߵ�ƽ
}

/* Ϩ��LED */
void LightOff(void)
{
	reg_gpio_data &= ~(1 << n); //��GPIO������͵�ƽ
}

