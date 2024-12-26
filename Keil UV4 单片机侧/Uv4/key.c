#include <reg52.h>
#include "key.h"

uchar lastCheckVal=0;	//�ϴμ��ֵ
uchar filter=0;			//�˲�״̬
uchar inputKey=0;		//��ֵ

/*******************************************************************************
* �� �� ��        : key_scan
* ��������		  : ����ɨ�裬�����̽ӿ�״̬
* ��    ��        : void 
* ��    ��        : uchar
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
* �� �� ��        : key_check
* ��������		  : ������״̬����������while(1)�е���
* ��    ��        : void
* ��    ��        : void
*******************************************************************************/
void key_check(void)
{
	uchar keyState;
	if(filter == 0)
	{
		keyState = key_scan();
		if(keyState != lastCheckVal)//����״̬�ı�
		{
			lastCheckVal = keyState;//�������¼��ֵ
			filter = 1;
		}
	}
	else if(filter >= FILTER_INTERVAL)
	{
		filter = 0;
		keyState = key_scan();
		if(keyState == lastCheckVal)//�˲�����ֵ�����˲�ǰ���ֵ����־����ȷʵ������
		{
			inputKey=keyState;
		}
		lastCheckVal = keyState;//����״̬�Ƿ�һ�£��������µļ��ֵ����
	}
	else
	{
		filter++;
	}
}
