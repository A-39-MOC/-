#ifndef _LCD1602_H_
#define _LCD1602_H_

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN口定义
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^5;
sbit LCD1602_RW=P1^1;
sbit LCD1602_RS=P1^0;

/**********************************
函数声明
**********************************/	
void lcd1602_init(void);
void lcd1602_clear(void);
void lcd1602_write_str(uchar *str, bit x, uchar y);
void show_cursor(bit x, uchar y);
void hide_cursor(void);

#endif
