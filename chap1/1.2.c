/* 无操作系统的串口驱动 */
/************************
 *serival.h文件
 ************************/

extern void SerivalInit(void);
extern void SerivalSend(const char buf*, int count);
extern void SerivalRecv(char buf*, int count);


/***********************
 *serival.c文件
 ***********************/
/* 初始化串口 */
void SerivalInit(void)
{
	...
}

/* 串口发送 */
void SerivalSend(const char buf*, int count)
{
	...
}

/* 串口接收 */
void SerivalRecv(char buf*, int count)
{
	...
}

/* 串口中断处理函数 */
void SerivalIsr(void)
{
	...
	serialInt = 1;
}

//其他模块想用这个设备的时候只需要包含设备驱动的头文件serival.h即可
//然后调用其中的外部接口函数即可 
