#ifndef _UART_H_
#define _UART_H_

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

void uart_init(void);
void uart_send_byte(uchar byte);
void uart_send_bytes(uchar *bytes, uchar length);
void uart_send_str(uchar *str);

#endif