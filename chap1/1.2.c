/* �޲���ϵͳ�Ĵ������� */
/************************
 *serival.h�ļ�
 ************************/

extern void SerivalInit(void);
extern void SerivalSend(const char buf*, int count);
extern void SerivalRecv(char buf*, int count);


/***********************
 *serival.c�ļ�
 ***********************/
/* ��ʼ������ */
void SerivalInit(void)
{
	...
}

/* ���ڷ��� */
void SerivalSend(const char buf*, int count)
{
	...
}

/* ���ڽ��� */
void SerivalRecv(char buf*, int count)
{
	...
}

/* �����жϴ����� */
void SerivalIsr(void)
{
	...
	serialInt = 1;
}

//����ģ����������豸��ʱ��ֻ��Ҫ�����豸������ͷ�ļ�serival.h����
//Ȼ��������е��ⲿ�ӿں������� 
