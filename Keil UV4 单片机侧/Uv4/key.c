#include <reg52.h>
#include "key.h"

uchar lastCheckVal=0;	//上次检测值
uchar filter=0;			//滤波状态
uchar inputKey=0;		//键值

/*******************************************************************************
* 函 数 名        : key_scan
* 函数功能		  : 键盘扫描，检查键盘接口状态
* 输    入        : void 
* 输    出        : uchar
*******************************************************************************/
uchar key_scan(void)
{
	uchar val = 0;
	if(key1 == 0)
	{
		val |= 0x01;
	}
	if(key2 == 0)
	{
		val |= 0x02;
	}
	if(key3 == 0)
	{
		val |= 0x04;
	}
	if(key4 == 0)
	{
		val |= 0x08;
	}
	return val;
}

/*******************************************************************************
* 函 数 名        : key_check
* 函数功能		  : 检测键盘状态，在主函数while(1)中调用
* 输    入        : void
* 输    出        : void
*******************************************************************************/
void key_check(void)
{
	uchar keyState;
	if(filter == 0)
	{
		keyState = key_scan();
		if(keyState != lastCheckVal)//输入状态改变
		{
			lastCheckVal = keyState;//保存最新检测值
			filter = 1;
		}
	}
	else if(filter >= FILTER_INTERVAL)
	{
		filter = 0;
		keyState = key_scan();
		if(keyState == lastCheckVal)//滤波后检测值等于滤波前检测值，标志按键确实按下了
		{
			inputKey=keyState;
		}
		lastCheckVal = keyState;//无论状态是否一致，都将最新的检测值保存
	}
	else
	{
		filter++;
	}
}
