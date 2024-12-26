#ifndef _DS18B20_H_
#define _DS18B20_H_

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit DQ = P2^2;

void ds18b20_temp_conver(void);
int ds18b20_read_temp(void);
void ds18b20_temp_change(int* tm);

#endif