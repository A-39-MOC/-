#include <reg52.h>
#include "key.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "uart.h"

sbit beep = P2^3;  // ���������Ŷ���

char code dig[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};  // �����ַ�����
char code mainPage1[] = "Temperature";  // ��ҳ���һ����ʾ����
char code mainPage2[] = "C";  // ��ҳ��ڶ�����ʾ����
char code LoSet[] = "Lo Set:";  // ����ֵ������ʾ����
char code HiSet[] = "Hi Set:";  // ����ֵ������ʾ����

char strBuf[6];  // 1602 ��ʾ������
uchar uartArry[7];  // ���ڷ��ͻ�����

int LoLV = 200;  // ����ֵ
int HiLV = 800;  // ����ֵ
int temp = 0;  // ʵʱ�¶�

char selectIdx;  // ��ǰѡ�еĲ˵�������
char digIdx;  // ��ǰѡ�е�����λ
uint cycleCount;  // ѭ��������
char taskIdx;  // ��������

void task_handle(void);  // ��������
void key_handle(uchar value);  // ����������
void temp_ctrl(void);  // �¶ȿ��ƺ���
void value_deal(int num);  // ��ֵ������

void main()
{
    lcd1602_init();  // ��ʼ�� LCD1602
    uart_init();  // ��ʼ�� UART

    selectIdx = 0;  // ��ʼ��ѡ�в˵���Ϊ 0
    lcd1602_write_str(mainPage1, 0, 2);  // ��ʾ��ҳ���һ��
    lcd1602_write_str(mainPage2, 1, 10);  // ��ʾ��ҳ��ڶ���
    taskIdx = -2;  // ��ʼ����������

    while (1)
    {
        key_check();  // ��鰴��
        if (inputKey != 0)  // ����а�������
        {
            key_handle(inputKey);  // ������ֵ
            inputKey = 0;  // ���ð���ֵ
        }
        else
        {
            if (++cycleCount >= 15000)  // ѭ���������ﵽ 15000
            {
                cycleCount = 0;  // ����ѭ��������
                task_handle();  // ִ��������
            }
        }
    }
}

void task_handle(void)
{
    if (selectIdx == 0)  // �����ǰѡ�е�����ҳ��
    {
        if (++taskIdx >= 3)  // ������������������ 3 ʱ����Ϊ 1
        {
            taskIdx = 1;
        }
        switch (taskIdx)
        {
            case -1:
            case 1:  // �¶�ת��
                ds18b20_temp_conver();
                break;
            case 2:  // ��ȡ�¶Ȳ�����
                temp = ds18b20_read_temp();
                temp_ctrl();
                value_deal(temp);
                lcd1602_write_str(strBuf, 1, 5);  // ���� LCD ��ʾ
                uart_send_bytes(uartArry, 7);  // �������ݵ�����
                break;
        }
    }
}

void key_handle(uchar value)
{
    switch (value)
    {
        case 0x01:  // �л���ť
            if (++selectIdx >= 3)  // ѡ�в˵������������ 3 ʱ����Ϊ 0
            {
                selectIdx = 0;
            }
            lcd1602_clear();  // ��� LCD ��ʾ
            hide_cursor();  // ���ع��
            digIdx = 1;  // ����ѡ�е�����λΪ 1
            switch (selectIdx)
            {
                case 0:  // ʵʱ�¶�״̬
                    lcd1602_write_str(mainPage1, 0, 2);
                    lcd1602_write_str(mainPage2, 1, 10);
                    break;
                case 1:  // ����ֵ����
                    lcd1602_write_str(LoSet, 0, 5);
                    value_deal(LoLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    show_cursor(1, 7);  // ��ʾ���
                    break;
                case 2:  // ����ֵ����
                    lcd1602_write_str(HiSet, 0, 5);
                    value_deal(HiLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    show_cursor(1, 7);  // ��ʾ���
                    break;
            }
            break;

        case 0x02:  // ���Ӱ�ť
            switch (selectIdx)
            {
                case 1:  // ����ֵ����
                    if (digIdx == 1)
                    {
                        LoLV += 10;  // ���� 10
                    }
                    else
                    {
                        LoLV += 100;  // ���� 100
                    }
                    if (LoLV >= 1000)  // �������ֵΪ 1000
                    {
                        LoLV = 1000;
                    }
                    value_deal(LoLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    break;
                case 2:  // ����ֵ����
                    if (digIdx == 1)
                    {
                        HiLV += 10;  // ���� 10
                    }
                    else
                    {
                        HiLV += 100;  // ���� 100
                    }
                    if (HiLV >= 1200)  // �������ֵΪ 1200
                    {
                        HiLV = 1200;
                    }
                    value_deal(HiLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    break;
            }
            if (selectIdx != 0)  // ���������ҳ��
            {
                show_cursor(1, 6 + digIdx);  // ��ʾ���
            }
            break;
        case 0x04:  // ���ٰ�ť
            switch (selectIdx)
            {
                case 1:  // ����ֵ����
                    if (digIdx == 1)
                    {
                        LoLV -= 10;  // ���� 10
                    }
                    else
                    {
                        LoLV -= 100;  // ���� 100
                    }
                    if (LoLV <= 0)  // ������СֵΪ 0
                    {
                        LoLV = 0;
                    }
                    value_deal(LoLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    break;
                case 2:  // ����ֵ����
                    if (digIdx == 1)
                    {
                        HiLV -= 10;  // ���� 10
                    }
                    else
                    {
                        HiLV -= 100;  // ���� 100
                    }
                    if (HiLV <= 200)  // ������СֵΪ 200
                    {
                        HiLV = 200;
                    }
                    value_deal(HiLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    break;
            }
            if (selectIdx != 0)  // ���������ҳ��
            {
                show_cursor(1, 6 + digIdx);  // ��ʾ���
            }
            break;
        case 0x08:  // λ�л���ť
            if (selectIdx != 0)  // ���������ҳ��
            {
                if (digIdx == 1)
                {
                    digIdx = 0;  // �л�����λ
                }
                else
                {
                    digIdx = 1;  // �л�����λ
                }
                show_cursor(1, 6 + digIdx);  // ��ʾ���
            }
            break;
        default:
            break;
    }
}

void temp_ctrl(void)
{
    if ((temp <= LoLV) || (temp >= HiLV))  // ����¶ȵ��ڵ���ֵ����ڸ���ֵ
    {
        if (beep)
        {
            beep = 0;  // �رշ�����
        }
    }
    else  // ����¶�����ֵ��Χ��
    {
        if (!beep)
        {
            beep = 1;  // �򿪷�����
        }
    }
}

// �����ʾ������
void clear_strBuf(void)
{
    strBuf[0] = '\0';
    strBuf[1] = '\0';
    strBuf[2] = '\0';
    strBuf[3] = '\0';
    strBuf[4] = '\0';
    strBuf[5] = '\0';
}

// ������ֵ����ʽ��Ϊ�ַ���
void value_deal(int num)
{
    int t = num;
    clear_strBuf();  // �����ʾ������
    strBuf[0] = t >= 1000 ? '1' : ' ';  // ����ǧλ
    if (t < 0)
    {
        ds18b20_temp_change(&t);  // ������
        strBuf[0] = '-';
    }
    strBuf[1] = dig[t / 100 % 10];  // ��λ
    strBuf[2] = dig[t / 10 % 10];  // ʮλ
    strBuf[3] = '.';  // С����
    strBuf[4] = dig[t % 10];  // ��λ

    uartArry[0] = strBuf[0];
    uartArry[1] = strBuf[1];
    uartArry[2] = strBuf[2];
    uartArry[3] = strBuf[3];
    uartArry[4] = strBuf[4];
    uartArry[5] = '\r';  // �س���
    uartArry[6] = '\n';  // ���з�
}#include <reg52.h>
#include "key.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "uart.h"

sbit beep = P2^3;

char code dig[]={'0','1','2','3','4','5','6','7','8','9'};//�����ַ�����
char code mainPage1[] = "Temperature";		
char code mainPage2[] = "C";	
char code LoSet[] = "Lo Set:";
char code HiSet[] = "Hi Set:";

char strBuf[6];		//1602��ʾ���ݻ���
uchar uartArry[7];

int LoLV = 200;		//����
int HiLV = 800;		//����
int temp = 0;		//ʵʱ�¶�

char selectIdx;		//�л�ѡ��
char digIdx;		//ѡ���λ
uint cycleCount;	//ѭ������
char taskIdx;		//�������

void task_handle(void);
void key_handle(uchar value);//����������
void temp_ctrl(void);
void value_deal(int num);

void main()
{
	lcd1602_init();		//��ʼ��Һ��
	uart_init();		//���ڳ�ʼ��

	selectIdx = 0;
	lcd1602_write_str(mainPage1, 0, 2);
	lcd1602_write_str(mainPage2, 1, 10);
	taskIdx = -2;

	while(1)
	{
		key_check();//�������
		if(inputKey != 0)//�а�������
		{
			key_handle(inputKey);//������ֵ����
			inputKey = 0;		 //��ֵ��0
		}
		else
		{
			if(++cycleCount >= 15000)
			{
				cycleCount = 0;
				task_handle();
			}
		}
	}
}

void task_handle(void)
{
	if(selectIdx == 0)
	{
		if(++taskIdx >= 3)
		{
			taskIdx = 1;
		}
		switch(taskIdx)
		{
			case -1:
			case 1://�¶�ת��
				ds18b20_temp_conver();
				break;
			case 2://�¶ȶ�ȡ������
				temp = ds18b20_read_temp();
				temp_ctrl();
				value_deal(temp);
				lcd1602_write_str(strBuf, 1, 5);
				uart_send_bytes(uartArry, 7);
				break;
		}
	}
}


void key_handle(uchar value)
{
	switch(value)
	{
		case 0x01:	  //�л���ť
			if(++selectIdx >= 3)
			{
				selectIdx = 0;
			}
			lcd1602_clear();
			hide_cursor();//���ع��
			digIdx = 1;//��λ
			switch(selectIdx)
			{
				case 0:	 //ʵʱ���״̬
					lcd1602_write_str(mainPage1, 0, 2);
					lcd1602_write_str(mainPage2, 1, 10);
					break;
				case 1:	//Lo����
					lcd1602_write_str(LoSet, 0, 5);
					value_deal(LoLV);
					lcd1602_write_str(strBuf, 1, 5);
					show_cursor(1, 7);
					break;
				case 2:	//Hi����
					lcd1602_write_str(HiSet, 0, 5);
					value_deal(HiLV);
					lcd1602_write_str(strBuf, 1, 5);
					show_cursor(1, 7);
					break;
			}
			break;

		case 0x02://+
			switch(selectIdx)
			{
				case 1:	//Lo����
					if(digIdx == 1)
					{
						LoLV += 10;
					}
					else
					{
						LoLV += 100;
					}
					if(LoLV >= 1000)
					{
						LoLV = 1000;
					}
					value_deal(LoLV);
					lcd1602_write_str(strBuf, 1, 5);
					break;
				case 2:	//Hi����
					if(digIdx == 1)
					{
						HiLV += 10;
					}
					else
					{
						HiLV += 100;
					}
					if(HiLV >= 1200)
					{
						HiLV = 1200;
					}
					value_deal(HiLV);
					lcd1602_write_str(strBuf, 1, 5);
					break;
			}
			if(selectIdx != 0)
			{
				show_cursor(1, 6 + digIdx);
			}
			break;
		case 0x04: //-
			switch(selectIdx)
			{
				case 1:	//Lo����
					if(digIdx == 1)
					{
						LoLV -= 10;
					}
					else
					{
						LoLV -= 100;
					}
					if(LoLV <= 0)
					{
						LoLV = 0;
					}
					value_deal(LoLV);
					lcd1602_write_str(strBuf, 1, 5);
					break;
				case 2:	//Hi����
					if(digIdx == 1)
					{
						HiLV -= 10;
					}
					else
					{
						HiLV -= 100;
					}
					if(HiLV <= 200)
					{
						HiLV = 200;
					}
					value_deal(HiLV);
					lcd1602_write_str(strBuf, 1, 5);
					break;
			}
			if(selectIdx != 0)
			{
				show_cursor(1, 6 + digIdx);
			}
			break;
		case 0x08://λ�л�
			if(selectIdx != 0)
			{
				if(digIdx == 1)
				{
					digIdx = 0;
				}
				else
				{
					digIdx = 1;
				}
				show_cursor(1, 6 + digIdx);
			}
			break;
		default:
			break;
	}
}

void temp_ctrl(void)
{
	if((temp <= LoLV) || (temp >= HiLV))//�¶ȴﵽ���޻�����
	{
		if(beep)
		{
			beep = 0;
		}
	}
	else//�¶�����
	{
		if(!beep)
		{
			beep = 1;
		}
	}
}

//�������
void clear_strBuf(void)
{
	strBuf[0]='\0';
	strBuf[1]='\0';
	strBuf[2]='\0';
	strBuf[3]='\0';
	strBuf[4]='\0';
	strBuf[5]='\0';
}


//�¶����ݴ���
void value_deal(int num)
{
	int t = num;
	clear_strBuf();
	strBuf[0] = t >= 1000 ? '1' : ' ';
	if(t < 0)
	{
		ds18b20_temp_change(&t);
		strBuf[0] = '-';
	}
	strBuf[1] = dig[t/100%10];
	strBuf[2] = dig[t/10%10];
	strBuf[3] = '.';
	strBuf[4] = dig[t%10];

	uartArry[0] = strBuf[0];
	uartArry[1] = strBuf[1];
	uartArry[2] = strBuf[2];
	uartArry[3] = strBuf[3];
	uartArry[4] = strBuf[4];
	uartArry[5] = '\r';
	uartArry[6] = '\n';
}

