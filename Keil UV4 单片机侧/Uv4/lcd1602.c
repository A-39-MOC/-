#include <reg52.h>
#include "delay.h"
#include "lcd1602.h"

/*******************************************************************************
* 函 数 名         : lcd1602WriteCmd
* 函数功能		   : 向LCD写入一个字节的命令
* 输    入         : uchar cmd
* 输    出         : void
*******************************************************************************/
void lcd1602WriteCmd(uchar cmd)	//写入命令
{
	LCD1602_E = 0;     			//使能
	LCD1602_RS = 0;	   			//选择发送命令
	LCD1602_RW = 0;	   			//选择写入
	LCD1602_DATAPINS = cmd;     //放入命令
	delay_us(100);

	LCD1602_E = 1;	          	//写入时序
	delay_us(100);
	LCD1602_E = 0;
	delay_ms(5);				//写完一个字节延时等待5ms，否则下次写入时LCD忙警告
}

/*******************************************************************************
* 函 数 名         : LcdWriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输    入         : uchar dat
* 输    出         : void
*******************************************************************************/		   		   
void lcd1602WriteData(uchar dat)	//写入数据
{
	LCD1602_E = 0;					//使能清零
	LCD1602_RS = 1;					//选择输入数据
	LCD1602_RW = 0;					//选择写入
	LCD1602_DATAPINS = dat;			//写入数据
	delay_us(100);

	LCD1602_E = 1;					//写入时序
	delay_us(100);
	LCD1602_E = 0;
	delay_ms(5);					//写完一个字节延时等待5ms，否则下次写入时LCD忙警告
}

/*******************************************************************************
* 函 数 名       : lcd1602_init()
* 函数功能		 : 初始化LCD屏
* 输    入       : void
* 输    出       : void
*******************************************************************************/		   
void lcd1602_init(void)		//LCD初始化
{
 	lcd1602WriteCmd(0x38);  //开显示
	lcd1602WriteCmd(0x0c);	//开显示不显示光标
	lcd1602WriteCmd(0x06);  //写一个指针加1
 	lcd1602_clear();		//清除显示
}

/*******************************************************************************
* 函 数 名       : lcd1602_clear()
* 函数功能		 : 清空LCD屏
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/		   
void lcd1602_clear(void)	//LCD清屏
{
 	lcd1602WriteCmd(0x01);  //清屏
	lcd1602WriteCmd(0x80);  //设置数据指针起点
}

/*******************************************************************************
* 函 数 名       : lcd1602_write_str
* 函数功能		 : 将数组内容显示到屏幕
* 输    入       : uchar *str, bit x, uchar y
* 输    出       : void
*******************************************************************************/
void lcd1602_write_str(uchar *str, bit x, uchar y)
{
	uchar address = 0x80;		//默认显示地址
	if(x)					 	//如果写入第2行
	{
		address += 0x40;	 	//写入地址+0x40
	}
	address += y;			 	//定位到显示位置
	lcd1602WriteCmd(address);	//设置写入指针位置
	while(*str)				  	//判断字符是否显示完
	{
		lcd1602WriteData(*str++);//显示字符
	}
}

/*******************************************************************************
* 函 数 名       : show_cursor
* 函数功能		 : 显示光标位置
* 输    入       : bit x, uchar y
* 输    出       : void
*******************************************************************************/
void show_cursor(bit x, uchar y)
{
	uchar address = 0x80;		//默认显示地址
	if(x)						//如果写入第2行
	{
		address += 0x40;		//写入地址+0x40
	}
	address += y;				//定位到显示位置
	lcd1602WriteCmd(address);	//写入显示位置
	lcd1602WriteCmd(0x0f);		//光标闪烁
}

/*******************************************************************************
* 函 数 名       : hide_cursor
* 函数功能		 : 隐藏光标
* 输    入       : void
* 输    出       : void
*******************************************************************************/
void hide_cursor(void)
{
	lcd1602WriteCmd(0x0c);		//隐藏光标
}
