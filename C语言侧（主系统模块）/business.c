//Coded By A-39

#include "business.h"

#include <stdio.h>

// 业务逻辑层：获取所有数据 
Product* getAllProducts(DatabaseConnection *dbConn, int *count) {
    MYSQL_RES *res = executeQuery(dbConn, "SELECT date,time,celsius,device FROM temp INNER JOIN devices ON temp.did=devices.did ORDER BY date,time ASC ");
    if (res == NULL) {
        *count = 0;
        return NULL;
    }

    *count = mysql_num_rows(res);; 
    Product *products = (Product*) malloc(*count * sizeof(Product));
    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))!= NULL) {
	    strcpy(products[i].date, row[0]);
		strcpy(products[i].time, row[1]);
        products[i].celsius = atof(row[2]);
        strcpy(products[i].device, row[3]);
        i++;
    }

    mysql_free_result(res);
    return products;
}

//设备逻辑层： 获取所有设备
Devices* getAllDevices(DatabaseConnection *dbConn, int *count) {
    MYSQL_RES *res = executeQuery(dbConn, "SELECT devices.did,device,note FROM devices ");
    if (res == NULL) {
        *count = 0;
        return NULL;
    }

    *count = mysql_num_rows(res);; 
    Devices *devices = (Devices*) malloc(*count * sizeof(Devices));
    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))!= NULL) {
	    devices[i].did = atoi(row[0]);  //strcpy(products[i].did, row[0]);
		strcpy(devices[i].device, row[1]);
        strcpy(devices[i].note, row[2]);
        i++;
    }

    mysql_free_result(res);
    return devices;
}
 

//业务逻辑层：根据日期查询数据
Product* getDateProducts(DatabaseConnection *dbConn, int *count,char date[]) {
	char sql[1024]; 
	snprintf(sql, sizeof(sql), "SELECT date,time,celsius,device FROM temp INNER JOIN devices ON temp.did=devices.did WHERE date='%s' ORDER BY date,time ASC ", date);
    MYSQL_RES *res = executeQuery(dbConn, sql);
    if (res == NULL) {
        *count = 0;
        return NULL;
    }

    *count = mysql_num_rows(res);; 
    Product *products = (Product*) malloc(*count * sizeof(Product));
    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))!= NULL) {
        strcpy(products[i].date, row[0]);
        strcpy(products[i].time, row[1]);
        products[i].celsius = atof(row[2]);
        strcpy(products[i].device, row[3]);
        i++;
    }

    mysql_free_result(res);
    return products;
}

//业务逻辑层：根据实际年月和温度查询
Product* getProducts(DatabaseConnection *dbConn, int *count,float celsius,int YEAR,int MONTH) {
	char sql[1024]; 
	snprintf(sql, sizeof(sql), "SELECT date,time,celsius,device FROM temp INNER JOIN devices ON temp.did=devices.did WHERE celsius='%f' AND YEAR(date)='%d' AND MONTH(date)='%d' ORDER BY date,time ASC ", celsius,YEAR,MONTH);
    MYSQL_RES *res = executeQuery(dbConn, sql);
    if (res == NULL) {
        *count = 0;
        return NULL;
    }
    0
    

    *count = mysql_num_rows(res);; 
    Product *products = (Product*) malloc(*count * sizeof(Product));
    int i = 0;
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))!= NULL) {
        strcpy(products[i].date, row[0]);
        strcpy(products[i].time, row[1]);
        products[i].celsius = atof(row[2]);
        strcpy(products[i].device, row[3]);
        i++;
    }

    mysql_free_result(res);
    return products;
}

// 业务逻辑层：录入数据 
void addProduct(DatabaseConnection *dbConn, const Product *product) {
    char query[100];
    sprintf(query, "INSERT INTO temp (date,time,celsius,did) VALUES ('%s','%s','%f','%d')", product->date, product->time, product->celsius, product->did);
    executeQuery(dbConn, query);
}

//业务逻辑层：自动抓取数据
void autoaddProduct(DatabaseConnection *dbConn, const Product *product) {
    char query[100];
    sprintf(query, "INSERT INTO temp VALUES (NULL,NOW(),NOW(),'%f', '%d')", product->celsius, product->did);
    executeQuery(dbConn, query);
}

//设备逻辑层：添加设备
void addDevice(DatabaseConnection *dbConn, const Devices *device) {
    char query[200]; // 增加缓冲区大小以防止溢出
    sprintf(query, "INSERT INTO devices (did, device, note) VALUES (NULL, '%s', '%s')", device->device, device->note);
    executeQuery(dbConn, query);
}

// 业务逻辑层：根据日期时间修改温度
void updateProduct(DatabaseConnection *dbConn, const Product *product, int hour, int minute) {
    char query[512]; // 增加缓冲区大小以防止溢出
    snprintf(query, sizeof(query), "UPDATE temp SET celsius=%f WHERE DATE='%s' AND hour(time)=%d AND minute(time)=%d",product->celsius, product->date, hour, minute);
    executeQuery(dbConn, query);
}

//设备逻辑层：根据设备ID修改设备名称，添加备注 
void upgradeProduct(DatabaseConnection *dbConn, const Product *product) {
    char query[1024];
    sprintf(query, "UPDATE devices SET device='%s', note='%s' WHERE did=%d", product->device, product->note, product->did);
    executeQuery(dbConn, query);
}

// 业务逻辑层：删除数据/条目 
void deleteProduct(DatabaseConnection *dbConn, const Product *product) {
    char query[1024];
    sprintf(query, "DELETE FROM temp WHERE DATE='%s'AND HOUR(TIME)='%s'", product->date, product->time);
    executeQuery(dbConn, query);
}

//设备逻辑层：删除设备
void delProduct(DatabaseConnection *dbConn, const Product *product) {
    char query[1024];
    sprintf(query, "DELETE FROM devices WHERE device='%s'", product->device);
    executeQuery(dbConn, query);
}

//Coded By RedHonker3911 
