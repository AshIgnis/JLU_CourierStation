#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"
#include "fileOperations.h"
#include "receivedPackage.h"
#include "structure.h"

// 添加用户
struct customer *addCustomer(struct customer *head, const char *username, const char *phone_number, const char *password, int customer_type) {
    struct customer *newCustomer = (struct customer *)malloc(sizeof(struct customer));
    if (!newCustomer) {
        perror("Failed to allocate memory for new customer");
        return head;
    }

    strcpy(newCustomer->username, username);
    strcpy(newCustomer->phone_number, phone_number);
    strcpy(newCustomer->password, password);
    newCustomer->customer_type = customer_type;
    newCustomer->received_packages = NULL;
    newCustomer->send_packages = NULL;
    newCustomer->next = head;
    newCustomer->ticket = 3;

    printf("用户添加成功！\n");
    return newCustomer;
}

// 查询用户信息
void queryCustomer(struct customer *head) {
    if (!head) {
        printf("当前没有用户数据。\n");
        return;
    }
    char phone_number[MAX_LEN];
    printf("请输入要查询的用户电话号码: ");
    scanf("%s", phone_number);

    struct customer *current = head;
    while (current) {
        if (strcmp(current->phone_number , phone_number) == 0){
            printf("\n用户信息:\n");
            printf("用户名: %s\n", current->username);
            printf("电话号码: %s\n", current->phone_number);
            switch(current->customer_type){
                case 1:
                    printf("用户类型: 普通用户\n");
                    break;
                case 2:
                    printf("用户类型: VIP用户\n");
                    break;
                case 3:
                    printf("用户类型: 企业用户\n");
                    break;
                case 4:
                    printf("用户类型: 学生用户\n");
                    break;
                case 5:
                    printf("用户类型: 老年用户\n");
                    break;
                default:
                    printf("用户类型: 非法用户\n");
                    break;
            }
            return;
        }
        current = current->next;
    }

    printf("未找到电话号码为 %s 的用户。\n", phone_number);
}

// 删除用户
struct customer *deleteCustomer(struct customer *head) {
    if (!head) {
        printf("当前没有用户数据。\n");
        return head;
    }

    char phone_number[MAX_LEN];
    printf("请输入要删除用户的电话号码: ");
    scanf("%s", phone_number);

    struct customer *current = head, *prev = NULL;
    while (current) {
        if (strcmp(current->phone_number , phone_number) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            freeCustomers(current);
            printf("电话号码为 %s 的用户已删除。\n", phone_number);
            return head;
        }
        prev = current;
        current = current->next;
    }

    printf("未找到电话号码为 %s 的用户。\n", phone_number);
    return head;
}

// 释放用户链表
void freeCustomers(struct customer *head) {
    while (head) {
        struct customer *temp = head;
        head = head->next;
        free(temp);
    }
}

int isPhoneNumberDuplicate(struct customer *head, const char *phone_number) {
    struct customer *current = head;
    while (current) {
        if (strcmp(current->phone_number, phone_number) == 0) {
            return 1; // 电话号码已存在
        }
        current = current->next;
    }
    return 0; // 电话号码不存在
}

void displayMenu_customer() {
    printf("\n========== 包裹管理系统 ==========\n");
    printf("1. 添加用户\n");
    printf("2. 查询用户信息\n");
    printf("3. 删除用户\n");
    printf("4. 保存数据到文件\n");
    printf("5. 从文件加载数据\n");
    printf("0. 退出系统\n");
    printf("=================================\n");
}

struct customer *customersCreating(struct customer *customerList) {
    int choice;
    do {
        displayMenu_customer();
        printf("请输入您的选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char username[100], phone_number[100], password[100];
                int customer_type;

                // 输入电话号码并检查是否重复
                do {
                    printf("请输入电话号码: ");
                    scanf("%s", phone_number);
                    if (isPhoneNumberDuplicate(customerList, phone_number)) {
                        printf("电话号码已存在，请重新输入！\n");
                    }
                } while (isPhoneNumberDuplicate(customerList, phone_number));

                printf("请输入用户名: ");
                scanf("%s", username);
                printf("请输入密码: ");
                scanf("%s", password);
                printf("请输入用户类型 (1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户): ");
                scanf("%d", &customer_type);
                while (customer_type < 1 || customer_type > 5) {
                    printf("输入数据非法，请重新输入！\n");
                    printf("请输入用户类型 (1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户): ");
                    scanf("%d", &customer_type);
                }

                customerList = addCustomer(customerList, username, phone_number, password, customer_type);
                printf("用户添加成功！\n");
                break;
            }
            case 2:
                queryCustomer(customerList);
                break;
            case 3:
                customerList = deleteCustomer(customerList);
                break;
            case 4:
                saveCustomers(customerList);
                break;
            case 5:
                customerList = loadCustomers();
                break;
            case 0:
                printf("返回主菜单...\n");
                break;
            default:
                printf("无效的选择，请重试。\n");
        }
        system("pause");
        system("cls");
    } while (choice != 0);

    return customerList;
}