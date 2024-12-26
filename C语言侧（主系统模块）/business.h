//Coded By A-39

#ifndef BUSINESS_H
#define BUSINESS_H

#include "database.h"

// 业务逻辑层：获取所有数据 
Product* getAllProducts(DatabaseConnection *dbConn, int *count);

//设备逻辑层： 获取所有设备
Devices* getAllDevices(DatabaseConnection *dbConn, int *count);

//业务逻辑层：根据日期查询数据
Product* getDateProducts(DatabaseConnection *dbConn, int *count,char date[]);

//业务逻辑层：根据实际年月和温度查询
Product* getProducts(DatabaseConnection *dbConn, int *count,float celsius,int YEAR,int MONTH);

// 业务逻辑层：录入数据 
void addProduct(DatabaseConnection *dbConn, const Product *product);

//业务逻辑层：自动抓取数据
void autoaddProduct(DatabaseConnection *dbConn, const Product *product);

//设备逻辑层：添加设备
void addDevice(DatabaseConnection *dbConn, const Devices *device); 

// 业务逻辑层：根据日期时间修改温度
void updateProduct(DatabaseConnection *dbConn, const Product *product, int hour, int minute);

//业务逻辑层：根据设备ID修改设备名，添加备注
void upgradeProduct(DatabaseConnection *dbConn, const Product *product);

// 业务逻辑层：删除数据/条目 
void deleteProduct(DatabaseConnection *dbConn, const Product *product);

//设备逻辑层：删除设备
void delProduct(DatabaseConnection *dbConn, const Product *product);

#endif // BUSINESS_H

//Coded By A-39
