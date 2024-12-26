#ifndef _DELAY_H_
#define _DELAY_H_

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

void delay_ms(uchar i);
void delay_us(uchar i);

#endif