#ifndef _KEY_H_
#define _KEY_H_

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

#define FILTER_INTERVAL 50	//¼üÅÌÊäÈëÂË²¨¼ä¸ô

sbit key1 = P3^6;
sbit key2 = P3^5;
sbit key3 = P3^4;
sbit key4 = P3^3;

extern uchar inputKey;//°´¼ü¼üÖµ

void key_check(void);

#endif