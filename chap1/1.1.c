/* 单任务的典型架构 */

int main(int argc, char *argv[])
{
	while(1)
	{
		if (serialInit == 1)
		/* 有串口中断 */
		{
			ProcessSerialInit(); /* 处理串口中断 */
			serialInit = 0; /* 中断标志变量清零0 */
		}

		if (keyInit == 1)
		/* 有按键中断 */
		{
			ProcessKeyInit(); /* 处理按键中断 */
			keyInit = 0; /* 中断标志变量清零0 */
		}

		status = CheckXXX();
		switch (status)
		{
			...
		}
		...
	}
}
//在这样的系统中，虽然不存在操作系统，但是设备驱动依然存在，一般情况
//每一种设备驱动都会定义一个软件模块，包含.h .c文件。
//.h文件定义该设备驱动的数据结构并声明外部函数，
//.c文件进行驱动的具体实现。
