//Coded By A-39
#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#include "business.h"

// ��ӡ�¶��б�

void printProducts(Product *products, int count) {
    printf("%-10s\t%-8s\t%8s\t%-10s\n", "����", "ʱ��", "�¶�", "�豸��");
    int i; 
    for (i = 0; i < count; i++) {
        char timeWithoutSeconds[6];
        strncpy(timeWithoutSeconds, products[i].time, 5);
        timeWithoutSeconds[5] = '\0';

        printf("%-10s\t%-8s\t%8.2f\t%-10s\n", products[i].date, timeWithoutSeconds, products[i].celsius, products[i].device);
    }
}

// ��ӡ�豸�б�
void printDevices(Devices *devices, int count){
    printf("%-5s\t%-15s\t%-20s\n", "�豸ID", "�豸��", "��ע");
    int i;
    for (i = 0; i < count; i++) {
        printf("%-5d\t%-15s\t%-20s\n", devices[i].did, devices[i].device, devices[i].note);
    }
}

// ������
int main(void) {
    setlocale(LC_ALL, "zh_CN.UTF-8");
    printf("��ӭʹ����֮�����¶Ȳɼ�ϵͳ\n");
    DatabaseConnection *dbConn = initDatabaseConnection();
    if (dbConn == NULL) {
        return 1;
    }

    int mainchoice, choice, collect, update, delete;
    do {
        printf("1.��ѯ����\n");
        printf("2.¼������\n");
        printf("3.ɾ����Ŀ/�豸\n");
        printf("4.�޸���Ŀ/�豸\n");
        printf("0.�˳�ϵͳ\n");
        printf("��������Ҫѡ���ѡ�\n");
        if (scanf("%d", &mainchoice) != 1) {
            printf("��Ч���룬������һ�����֡�\n");
            while (getchar() != '\n'); // ��ջ�����
            continue;
        }
        switch (mainchoice) {
            case 1:
                do {
                    printf("1.��ѯ�����¶�����\n");
                    printf("2.�����ڡ�ʱ�䡢�¶Ⱦ�ȷ��ѯ\n");
                    printf("3.������ģ����ѯ\n");
                    printf("4.��ѯ�����豸\n");
                    printf("0.������һ��\n");
                    printf("��������Ҫѡ���ѡ�\n");
                    if (scanf("%d", &choice) != 1) {
                        printf("��Ч���룬������һ�����֡�\n");
                        while (getchar() != '\n'); // ��ջ�����
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
                            printf("�������¶ȣ� ");
                            if (scanf("%f", &celsius) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("��������ݣ� ");
                            if (scanf("%d", &year) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("�������·ݣ� ");
                            if (scanf("%d", &MONTH) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
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
                            printf("���������ڣ� ");
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
                    printf("1.�ֶ�¼������\n");
                    printf("2.���Զ�¼������\n");
                    printf("3.����豸\n");
                    printf("0.������һ��\n");
                    printf("��������Ҫѡ���ѡ�\n");
                    if (scanf("%d", &collect) != 1) {
                        printf("��Ч���룬������һ�����֡�\n");
                        while (getchar() != '\n'); // ��ջ�����
                        continue;
                    }
                    switch (collect) {
                        case 1: {
                            Product newProduct;
                            printf("������¼���¶ȵ����ڣ� ");
                            scanf("%s", newProduct.date);
                            printf("������¼���¶ȵ�ʱ�Σ� ");
                            scanf("%s", newProduct.time);
                            printf("������¼����¶ȣ� ");
                            if (scanf("%f", &newProduct.celsius) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("������¼����豸ID�� ");
                            if (scanf("%d", &newProduct.did) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("¼��ɹ�\n");
                            addProduct(dbConn, &newProduct);
                            break;
                        }
                        case 2: {
                            Product newProduct;
                            printf("����¼��...\n");
                            printf("�������¶ȣ�\n");
                            if (scanf("%f", &newProduct.celsius) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("�������豸�ţ�\n");
                            if (scanf("%d", &newProduct.did) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            autoaddProduct(dbConn, &newProduct);
                            printf("¼��ɹ�\n");
                            break;
                        }
                        case 3: {
                        	Devices newDevice;
                        	printf("�������豸����: ");
                        	 scanf("%s", newDevice.device);
							 printf("�������豸��ע: ");
							 scanf("%s", newDevice.note);
                             printf("��ӳɹ�\n");
                             addDevice(dbConn, &newDevice);
                             break;
                        }
                    }
                } while (collect != 0);
                break;

            case 3:
                do {
                    printf("1.��ʱ���ɾ����Ŀ\n");
                    printf("2.���豸����ɾ���豸\n");
                    printf("0.������һ��\n");
                    printf("��������Ҫѡ���ѡ�\n");
                    if (scanf("%d", &delete) != 1) {
                        printf("��Ч���룬������һ�����֡�\n");
                        while (getchar() != '\n'); // ��ջ�����
                        continue;
                    }
                    switch (delete) {
                        case 1: {
                            Product deletedProduct;
                            printf("������Ҫɾ������Ŀ���ڣ� ");
                            scanf("%s", deletedProduct.date);
                            printf("������Ҫɾ������Ŀʱ�Σ� ");
                            scanf("%s", deletedProduct.time);
                            deleteProduct(dbConn, &deletedProduct);
                            break;
                        }
                        case 2: {
                            Product deldProduct;
                            printf("������Ҫɾ�����豸���ƣ� ");
                            scanf("%s", deldProduct.device);
                            delProduct(dbConn, &deldProduct);
                            break;
                        }
                    }
                } while (delete != 0);
                break;

            case 4:
                do {
                    printf("1.����ʱ�������޸��¶�\n");
                    printf("2.�����豸ID�޸��豸��Ϣ\n");
                    printf("0.������һ��\n");
                    printf("��������Ҫѡ���ѡ�\n");
                    if (scanf("%d", &update) != 1) {
                        printf("��Ч���룬������һ�����֡�\n");
                        while (getchar() != '\n'); // ��ջ�����
                        continue;
                    }
                    switch (update) {
                        case 1: {
                            Product updatedProduct;
                            int hour;
                            int minute;
                            printf("�������¶ȣ� ");
                            if (scanf("%f", &updatedProduct.celsius) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("���������ڣ� ");
                            scanf("%s", updatedProduct.date);
                            printf("������Сʱ�� ");
                            if (scanf("%d", &hour) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("��������ӣ� ");
                            if (scanf("%d", &minute) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("�޸ĳɹ�\n");
                            updateProduct(dbConn, &updatedProduct, hour, minute);
                            break;
                        }
                        case 2: {
                            Product upgradedProduct;
                            printf("�������豸ID�� ");
                            if (scanf("%d", &upgradedProduct.did) != 1) {
                                printf("��Ч���룬������һ�����֡�\n");
                                while (getchar() != '\n'); // ��ջ�����
                                continue;
                            }
                            printf("�������µ��豸���ƣ� ");
                            scanf("%s", &upgradedProduct.device);
                            printf("�������µı�ע�� ");
                            scanf("%s", &upgradedProduct.note);
                            printf("�޸ĳɹ�\n");
                            upgradeProduct(dbConn, &upgradedProduct);
                            break;
                        }
                    }
                } while (update != 0);
                break;

            case 0:
                printf("�˳�ϵͳ\n");
                break;

            default:
                printf("��Чѡ����������롣\n");
                break;
        }
    } while (mainchoice != 0);

    closeDatabaseConnection(dbConn);
    return 0;
}
//Coded By A-39
