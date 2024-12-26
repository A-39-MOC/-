//Coded By A-39

#ifndef DATABASE_H
#define DATABASE_H

#include <mysql.h>

// �����¶ȱ����豸��ṹ��
typedef struct {
    int tid;//�¶ȱ�id 
    char date[50];//�����ֶ� 
    char time[50];
    float celsius;
    int did;
    char device[1024];
    char note[1024];
} Product;

//�����豸��ṹ�� 

typedef struct {
	int did;//�豸��id 
    char device[1024];//�豸���ֶ� 
    char note[1024];
} Devices; 

// �������ݿ����ӽṹ��
typedef struct {
    MYSQL *conn;
} DatabaseConnection;

// ���ݷ��ʲ㣺��ʼ�����ݿ�����
DatabaseConnection* initDatabaseConnection();

// ���ݷ��ʲ㣺ִ�в�ѯ
MYSQL_RES* executeQuery(DatabaseConnection *dbConn, const char *query);

// ���ݷ��ʲ㣺�ر����ݿ�����
void closeDatabaseConnection(DatabaseConnection *dbConn);

#endif // DATABASE_H

//Coded By A-39
