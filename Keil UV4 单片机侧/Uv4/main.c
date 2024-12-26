#include <reg52.h>
#include "key.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "uart.h"

sbit beep = P2^3;  // 蜂鸣器引脚定义

char code dig[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};  // 数字字符数组
char code mainPage1[] = "Temperature";  // 主页面第一行显示内容
char code mainPage2[] = "C";  // 主页面第二行显示内容
char code LoSet[] = "Lo Set:";  // 低阈值设置显示内容
char code HiSet[] = "Hi Set:";  // 高阈值设置显示内容

char strBuf[6];  // 1602 显示缓冲区
uchar uartArry[7];  // 串口发送缓冲区

int LoLV = 200;  // 低阈值
int HiLV = 800;  // 高阈值
int temp = 0;  // 实时温度

char selectIdx;  // 当前选中的菜单项索引
char digIdx;  // 当前选中的数字位
uint cycleCount;  // 循环计数器
char taskIdx;  // 任务索引

void task_handle(void);  // 任务处理函数
void key_handle(uchar value);  // 按键处理函数
void temp_ctrl(void);  // 温度控制函数
void value_deal(int num);  // 数值处理函数

void main()
{
    lcd1602_init();  // 初始化 LCD1602
    uart_init();  // 初始化 UART

    selectIdx = 0;  // 初始化选中菜单项为 0
    lcd1602_write_str(mainPage1, 0, 2);  // 显示主页面第一行
    lcd1602_write_str(mainPage2, 1, 10);  // 显示主页面第二行
    taskIdx = -2;  // 初始化任务索引

    while (1)
    {
        key_check();  // 检查按键
        if (inputKey != 0)  // 如果有按键按下
        {
            key_handle(inputKey);  // 处理按键值
            inputKey = 0;  // 重置按键值
        }
        else
        {
            if (++cycleCount >= 15000)  // 循环计数器达到 15000
            {
                cycleCount = 0;  // 重置循环计数器
                task_handle();  // 执行任务处理
            }
        }
    }
}

void task_handle(void)
{
    if (selectIdx == 0)  // 如果当前选中的是主页面
    {
        if (++taskIdx >= 3)  // 任务索引递增，超过 3 时重置为 1
        {
            taskIdx = 1;
        }
        switch (taskIdx)
        {
            case -1:
            case 1:  // 温度转换
                ds18b20_temp_conver();
                break;
            case 2:  // 读取温度并处理
                temp = ds18b20_read_temp();
                temp_ctrl();
                value_deal(temp);
                lcd1602_write_str(strBuf, 1, 5);  // 更新 LCD 显示
                uart_send_bytes(uartArry, 7);  // 发送数据到串口
                break;
        }
    }
}

void key_handle(uchar value)
{
    switch (value)
    {
        case 0x01:  // 切换按钮
            if (++selectIdx >= 3)  // 选中菜单项递增，超过 3 时重置为 0
            {
                selectIdx = 0;
            }
            lcd1602_clear();  // 清除 LCD 显示
            hide_cursor();  // 隐藏光标
            digIdx = 1;  // 设置选中的数字位为 1
            switch (selectIdx)
            {
                case 0:  // 实时温度状态
                    lcd1602_write_str(mainPage1, 0, 2);
                    lcd1602_write_str(mainPage2, 1, 10);
                    break;
                case 1:  // 低阈值设置
                    lcd1602_write_str(LoSet, 0, 5);
                    value_deal(LoLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    show_cursor(1, 7);  // 显示光标
                    break;
                case 2:  // 高阈值设置
                    lcd1602_write_str(HiSet, 0, 5);
                    value_deal(HiLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    show_cursor(1, 7);  // 显示光标
                    break;
            }
            break;

        case 0x02:  // 增加按钮
            switch (selectIdx)
            {
                case 1:  // 低阈值设置
                    if (digIdx == 1)
                    {
                        LoLV += 10;  // 增加 10
                    }
                    else
                    {
                        LoLV += 100;  // 增加 100
                    }
                    if (LoLV >= 1000)  // 限制最大值为 1000
                    {
                        LoLV = 1000;
                    }
                    value_deal(LoLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    break;
                case 2:  // 高阈值设置
                    if (digIdx == 1)
                    {
                        HiLV += 10;  // 增加 10
                    }
                    else
                    {
                        HiLV += 100;  // 增加 100
                    }
                    if (HiLV >= 1200)  // 限制最大值为 1200
                    {
                        HiLV = 1200;
                    }
                    value_deal(HiLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    break;
            }
            if (selectIdx != 0)  // 如果不是主页面
            {
                show_cursor(1, 6 + digIdx);  // 显示光标
            }
            break;
        case 0x04:  // 减少按钮
            switch (selectIdx)
            {
                case 1:  // 低阈值设置
                    if (digIdx == 1)
                    {
                        LoLV -= 10;  // 减少 10
                    }
                    else
                    {
                        LoLV -= 100;  // 减少 100
                    }
                    if (LoLV <= 0)  // 限制最小值为 0
                    {
                        LoLV = 0;
                    }
                    value_deal(LoLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    break;
                case 2:  // 高阈值设置
                    if (digIdx == 1)
                    {
                        HiLV -= 10;  // 减少 10
                    }
                    else
                    {
                        HiLV -= 100;  // 减少 100
                    }
                    if (HiLV <= 200)  // 限制最小值为 200
                    {
                        HiLV = 200;
                    }
                    value_deal(HiLV);
                    lcd1602_write_str(strBuf, 1, 5);
                    break;
            }
            if (selectIdx != 0)  // 如果不是主页面
            {
                show_cursor(1, 6 + digIdx);  // 显示光标
            }
            break;
        case 0x08:  // 位切换按钮
            if (selectIdx != 0)  // 如果不是主页面
            {
                if (digIdx == 1)
                {
                    digIdx = 0;  // 切换数字位
                }
                else
                {
                    digIdx = 1;  // 切换数字位
                }
                show_cursor(1, 6 + digIdx);  // 显示光标
            }
            break;
        default:
            break;
    }
}

void temp_ctrl(void)
{
    if ((temp <= LoLV) || (temp >= HiLV))  // 如果温度低于低阈值或高于高阈值
    {
        if (beep)
        {
            beep = 0;  // 关闭蜂鸣器
        }
    }
    else  // 如果温度在阈值范围内
    {
        if (!beep)
        {
            beep = 1;  // 打开蜂鸣器
        }
    }
}

// 清除显示缓冲区
void clear_strBuf(void)
{
    strBuf[0] = '\0';
    strBuf[1] = '\0';
    strBuf[2] = '\0';
    strBuf[3] = '\0';
    strBuf[4] = '\0';
    strBuf[5] = '\0';
}

// 处理数值并格式化为字符串
void value_deal(int num)
{
    int t = num;
    clear_strBuf();  // 清除显示缓冲区
    strBuf[0] = t >= 1000 ? '1' : ' ';  // 处理千位
    if (t < 0)
    {
        ds18b20_temp_change(&t);  // 处理负数
        strBuf[0] = '-';
    }
    strBuf[1] = dig[t / 100 % 10];  // 百位
    strBuf[2] = dig[t / 10 % 10];  // 十位
    strBuf[3] = '.';  // 小数点
    strBuf[4] = dig[t % 10];  // 个位

    uartArry[0] = strBuf[0];
    uartArry[1] = strBuf[1];
    uartArry[2] = strBuf[2];
    uartArry[3] = strBuf[3];
    uartArry[4] = strBuf[4];
    uartArry[5] = '\r';  // 回车符
    uartArry[6] = '\n';  // 换行符
}#include <reg52.h>
#include "key.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "uart.h"

sbit beep = P2^3;

char code dig[]={'0','1','2','3','4','5','6','7','8','9'};//数字字符数组
char code mainPage1[] = "Temperature";		
char code mainPage2[] = "C";	
char code LoSet[] = "Lo Set:";
char code HiSet[] = "Hi Set:";

char strBuf[6];		//1602显示数据缓存
uchar uartArry[7];

int LoLV = 200;		//低限
int HiLV = 800;		//高限
int temp = 0;		//实时温度

char selectIdx;		//切换选择
char digIdx;		//选择的位
uint cycleCount;	//循环周期
char taskIdx;		//任务序号

void task_handle(void);
void key_handle(uchar value);//按键处理函数
void temp_ctrl(void);
void value_deal(int num);

void main()
{
	lcd1602_init();		//初始化液晶
	uart_init();		//串口初始化

	selectIdx = 0;
	lcd1602_write_str(mainPage1, 0, 2);
	lcd1602_write_str(mainPage2, 1, 10);
	taskIdx = -2;

	while(1)
	{
		key_check();//按键检测
		if(inputKey != 0)//有按键按下
		{
			key_handle(inputKey);//按键数值处理
			inputKey = 0;		 //键值清0
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
			case 1://温度转换
				ds18b20_temp_conver();
				break;
			case 2://温度读取并处理
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
		case 0x01:	  //切换按钮
			if(++selectIdx >= 3)
			{
				selectIdx = 0;
			}
			lcd1602_clear();
			hide_cursor();//隐藏光标
			digIdx = 1;//个位
			switch(selectIdx)
			{
				case 0:	 //实时监控状态
					lcd1602_write_str(mainPage1, 0, 2);
					lcd1602_write_str(mainPage2, 1, 10);
					break;
				case 1:	//Lo设置
					lcd1602_write_str(LoSet, 0, 5);
					value_deal(LoLV);
					lcd1602_write_str(strBuf, 1, 5);
					show_cursor(1, 7);
					break;
				case 2:	//Hi设置
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
				case 1:	//Lo设置
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
				case 2:	//Hi设置
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
				case 1:	//Lo设置
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
				case 2:	//Hi设置
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
		case 0x08://位切换
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
	if((temp <= LoLV) || (temp >= HiLV))//温度达到下限或上限
	{
		if(beep)
		{
			beep = 0;
		}
	}
	else//温度适中
	{
		if(!beep)
		{
			beep = 1;
		}
	}
}

//清除缓存
void clear_strBuf(void)
{
	strBuf[0]='\0';
	strBuf[1]='\0';
	strBuf[2]='\0';
	strBuf[3]='\0';
	strBuf[4]='\0';
	strBuf[5]='\0';
}


//温度数据处理
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

