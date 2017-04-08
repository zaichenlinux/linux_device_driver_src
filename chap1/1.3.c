#define reg_gpio_ctrl *(volatile int *)(To_virtual(GPIO_REG_CTRL))
#define reg_gpio_data *(volatile int *)(To_virtuel(GPIO_REG_DATA))
/* 初始化LED */
void LightInit(void)
{
	reg_gpio_ctrl |= (1 << n); //设置GPIO为输出
}

/* 点亮LED */
void LightOn(void)
{
	reg_gpio_data |= (1 << n); //在GPIO上输出高电平
}

/* 熄灭LED */
void LightOff(void)
{
	reg_gpio_data &= ~(1 << n); //在GPIO上输出低电平
}

