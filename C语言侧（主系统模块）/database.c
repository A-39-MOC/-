//Coded By A-39
#include "database.h"
#include <stdio.h>

// ���ݷ��ʲ㣺��ʼ�����ݿ�����
DatabaseConnection* initDatabaseConnection() {
    DatabaseConnection *dbConn = (DatabaseConnection*) malloc(sizeof(DatabaseConnection));
    dbConn->conn = mysql_init(NULL);
    if (!mysql_real_connect(dbConn->conn, "localhost", "root", "1234", "gettemperature", 3306, NULL, 0)) {
        fprintf(stderr, "Error connecting to database: %s\n", mysql_error(dbConn->conn));
        free(dbConn);
        return NULL;
    }
    return dbConn;
}

// ���ݷ��ʲ㣺ִ�в�ѯ
MYSQL_RES* executeQuery(DatabaseConnection *dbConn, const char *query) {
    if (mysql_set_character_set(dbConn->conn, "GBK")) {
	    fprintf(stderr, "Error setting character set: %s\n", mysql_error(dbConn->conn));
	    return;
	}
    if (mysql_query(dbConn->conn, query)) {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(dbConn->conn));
        return NULL;
    }
    return mysql_store_result(dbConn->conn);
}

// ���ݷ��ʲ㣺�ر����ݿ�����
void closeDatabaseConnection(DatabaseConnection *dbConn) {
    mysql_close(dbConn->conn);
    free(dbConn);
}
//Coded By A-39

