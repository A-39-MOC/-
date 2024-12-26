//Coded By A-39

#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>

// 定义温度表与设备表结构体
typedef struct {
    int tid;//温度表id 
    char date[50];//日期字段 
    char time[50];
    float celsius;
    int did;
    char device[1024];
    char note[1024];
} Product;

//定义设备表结构体 

typedef struct {
	int did;//设备表id 
    char device[1024];//设备名字段 
    char note[1024];
} Devices; 

// 定义数据库连接结构体
typedef struct {
    MYSQL *conn;
} DatabaseConnection;

// 数据访问层：初始化数据库连接
DatabaseConnection* initDatabaseConnection();

// 数据访问层：执行查询
MYSQL_RES* executeQuery(DatabaseConnection *dbConn, const char *query);

// 数据访问层：关闭数据库连接
void closeDatabaseConnection(DatabaseConnection *dbConn);

#endif // DATABASE_H

//Coded By A-39
