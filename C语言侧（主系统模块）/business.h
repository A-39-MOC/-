//Coded By A-39

#ifndef BUSINESS_H
#define BUSINESS_H

#include "database.h"

// ҵ���߼��㣺��ȡ�������� 
Product* getAllProducts(DatabaseConnection *dbConn, int *count);

//�豸�߼��㣺 ��ȡ�����豸
Devices* getAllDevices(DatabaseConnection *dbConn, int *count);

//ҵ���߼��㣺�������ڲ�ѯ����
Product* getDateProducts(DatabaseConnection *dbConn, int *count,char date[]);

//ҵ���߼��㣺����ʵ�����º��¶Ȳ�ѯ
Product* getProducts(DatabaseConnection *dbConn, int *count,float celsius,int YEAR,int MONTH);

// ҵ���߼��㣺¼������ 
void addProduct(DatabaseConnection *dbConn, const Product *product);

//ҵ���߼��㣺�Զ�ץȡ����
void autoaddProduct(DatabaseConnection *dbConn, const Product *product);

//�豸�߼��㣺����豸
void addDevice(DatabaseConnection *dbConn, const Devices *device); 

// ҵ���߼��㣺��������ʱ���޸��¶�
void updateProduct(DatabaseConnection *dbConn, const Product *product, int hour, int minute);

//ҵ���߼��㣺�����豸ID�޸��豸������ӱ�ע
void upgradeProduct(DatabaseConnection *dbConn, const Product *product);

// ҵ���߼��㣺ɾ������/��Ŀ 
void deleteProduct(DatabaseConnection *dbConn, const Product *product);

//�豸�߼��㣺ɾ���豸
void delProduct(DatabaseConnection *dbConn, const Product *product);

#endif // BUSINESS_H

//Coded By A-39
