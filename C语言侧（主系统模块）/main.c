//Coded By A-39
#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#include "business.h"

// 打印温度列表

void printProducts(Product *products, int count) {
    printf("%-10s\t%-8s\t%8s\t%-10s\n", "日期", "时间", "温度", "设备名");
    int i; 
    for (i = 0; i < count; i++) {
        char timeWithoutSeconds[6];
        strncpy(timeWithoutSeconds, products[i].time, 5);
        timeWithoutSeconds[5] = '\0';

        printf("%-10s\t%-8s\t%8.2f\t%-10s\n", products[i].date, timeWithoutSeconds, products[i].celsius, products[i].device);
    }
}

// 打印设备列表
void printDevices(Devices *devices, int count){
    printf("%-5s\t%-15s\t%-20s\n", "设备ID", "设备名", "备注");
    int i;
    for (i = 0; i < count; i++) {
        printf("%-5d\t%-15s\t%-20s\n", devices[i].did, devices[i].device, devices[i].note);
    }
}

// 主函数
int main(void) {
    setlocale(LC_ALL, "zh_CN.UTF-8");
    printf("欢迎使用月之暗面温度采集系统\n");
    DatabaseConnection *dbConn = initDatabaseConnection();
    if (dbConn == NULL) {
        return 1;
    }

    int mainchoice, choice, collect, update, delete;
    do {
        printf("1.查询数据\n");
        printf("2.录入数据\n");
        printf("3.删除条目/设备\n");
        printf("4.修改条目/设备\n");
        printf("0.退出系统\n");
        printf("请输入您要选择的选项：\n");
        if (scanf("%d", &mainchoice) != 1) {
            printf("无效输入，请输入一个数字。\n");
            while (getchar() != '\n'); // 清空缓冲区
            continue;
        }
        switch (mainchoice) {
            case 1:
                do {
                    printf("1.查询所有温度数据\n");
                    printf("2.按日期、时间、温度精确查询\n");
                    printf("3.按日期模糊查询\n");
                    printf("4.查询所有设备\n");
                    printf("0.返回上一级\n");
                    printf("请输入您要选择的选项：\n");
                    if (scanf("%d", &choice) != 1) {
                        printf("无效输入，请输入一个数字。\n");
                        while (getchar() != '\n'); // 清空缓冲区
                        continue;
                    }
                    switch (choice) {
                        case 1: {
                            int count;
                            Product *products = getAllProducts(dbConn, &count);
                            if (products != NULL) {
                                printProducts(products, count);
                                free(products);
                            }
                            break;
                        }
                        case 2: {
                            int count;
                            float celsius;
                            int year, MONTH;
                            printf("请输入温度： ");
                            if (scanf("%f", &celsius) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("请输入年份： ");
                            if (scanf("%d", &year) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("请输入月份： ");
                            if (scanf("%d", &MONTH) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            Product *products = getProducts(dbConn, &count, celsius, year, MONTH);
                            if (products != NULL) {
                                printProducts(products, count);
                                free(products);
                            }
                            break;
                        }
                        case 3: {
                            int count;
                            char name[50];
                            printf("请输入日期： ");
                            scanf("%s", name);
                            Product *products = getDateProducts(dbConn, &count, name);
                            if (products != NULL) {
                                printProducts(products, count);
                                free(products);
                            }
                            break;
                        }
                        case 4:{
                            int count;
                            Devices *devices = getAllDevices(dbConn, &count);
                            if (devices != NULL) {
                                printDevices(devices, count);
                                free(devices);
                            }
                            break;
                        } 
                    }
                } while (choice != 0);
                break;

            case 2:
                do {
                    printf("1.手动录入数据\n");
                    printf("2.半自动录入数据\n");
                    printf("3.添加设备\n");
                    printf("0.返回上一级\n");
                    printf("请输入您要选择的选项：\n");
                    if (scanf("%d", &collect) != 1) {
                        printf("无效输入，请输入一个数字。\n");
                        while (getchar() != '\n'); // 清空缓冲区
                        continue;
                    }
                    switch (collect) {
                        case 1: {
                            Product newProduct;
                            printf("请输入录入温度的日期： ");
                            scanf("%s", newProduct.date);
                            printf("请输入录入温度的时段： ");
                            scanf("%s", newProduct.time);
                            printf("请输入录入的温度： ");
                            if (scanf("%f", &newProduct.celsius) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("请输入录入的设备ID： ");
                            if (scanf("%d", &newProduct.did) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("录入成功\n");
                            addProduct(dbConn, &newProduct);
                            break;
                        }
                        case 2: {
                            Product newProduct;
                            printf("正在录入...\n");
                            printf("请输入温度：\n");
                            if (scanf("%f", &newProduct.celsius) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("请输入设备号：\n");
                            if (scanf("%d", &newProduct.did) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            autoaddProduct(dbConn, &newProduct);
                            printf("录入成功\n");
                            break;
                        }
                        case 3: {
                        	Devices newDevice;
                        	printf("请输入设备名称: ");
                        	 scanf("%s", newDevice.device);
							 printf("请输入设备备注: ");
							 scanf("%s", newDevice.note);
                             printf("添加成功\n");
                             addDevice(dbConn, &newDevice);
                             break;
                        }
                    }
                } while (collect != 0);
                break;

            case 3:
                do {
                    printf("1.按时间戳删除条目\n");
                    printf("2.按设备名称删除设备\n");
                    printf("0.返回上一级\n");
                    printf("请输入您要选择的选项：\n");
                    if (scanf("%d", &delete) != 1) {
                        printf("无效输入，请输入一个数字。\n");
                        while (getchar() != '\n'); // 清空缓冲区
                        continue;
                    }
                    switch (delete) {
                        case 1: {
                            Product deletedProduct;
                            printf("请输入要删除的条目日期： ");
                            scanf("%s", deletedProduct.date);
                            printf("请输入要删除的条目时段： ");
                            scanf("%s", deletedProduct.time);
                            deleteProduct(dbConn, &deletedProduct);
                            break;
                        }
                        case 2: {
                            Product deldProduct;
                            printf("请输入要删除的设备名称： ");
                            scanf("%s", deldProduct.device);
                            delProduct(dbConn, &deldProduct);
                            break;
                        }
                    }
                } while (delete != 0);
                break;

            case 4:
                do {
                    printf("1.根据时间日期修改温度\n");
                    printf("2.根据设备ID修改设备信息\n");
                    printf("0.返回上一级\n");
                    printf("请输入您要选择的选项：\n");
                    if (scanf("%d", &update) != 1) {
                        printf("无效输入，请输入一个数字。\n");
                        while (getchar() != '\n'); // 清空缓冲区
                        continue;
                    }
                    switch (update) {
                        case 1: {
                            Product updatedProduct;
                            int hour;
                            int minute;
                            printf("请输入温度： ");
                            if (scanf("%f", &updatedProduct.celsius) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("请输入日期： ");
                            scanf("%s", updatedProduct.date);
                            printf("请输入小时： ");
                            if (scanf("%d", &hour) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("请输入分钟： ");
                            if (scanf("%d", &minute) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("修改成功\n");
                            updateProduct(dbConn, &updatedProduct, hour, minute);
                            break;
                        }
                        case 2: {
                            Product upgradedProduct;
                            printf("请输入设备ID： ");
                            if (scanf("%d", &upgradedProduct.did) != 1) {
                                printf("无效输入，请输入一个数字。\n");
                                while (getchar() != '\n'); // 清空缓冲区
                                continue;
                            }
                            printf("请输入新的设备名称： ");
                            scanf("%s", &upgradedProduct.device);
                            printf("请输入新的备注： ");
                            scanf("%s", &upgradedProduct.note);
                            printf("修改成功\n");
                            upgradeProduct(dbConn, &upgradedProduct);
                            break;
                        }
                    }
                } while (update != 0);
                break;

            case 0:
                printf("退出系统\n");
                break;

            default:
                printf("无效选项，请重新输入。\n");
                break;
        }
    } while (mainchoice != 0);

    closeDatabaseConnection(dbConn);
    return 0;
}
//Coded By A-39
