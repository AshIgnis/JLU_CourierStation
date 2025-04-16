#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"
#include "fileOperations.h"
#include "receivedPackage.h"
#include "structure.h"
#include "stdbool.h"
#include "ctype.h"
#include "user.h"

// 添加用户
struct customer *addCustomer(struct customer *head, const char *username, const char *phone_number, const char *password, int customer_type) {
    struct customer *newCustomer = (struct customer *)malloc(sizeof(struct customer)); // 新建用户结点
    if (!newCustomer) { //构建失败返回
        perror("Failed to allocate memory for new customer");
        return head;
    }
    //值拷贝
    strcpy(newCustomer->username, username);
    strcpy(newCustomer->phone_number, phone_number);
    strcpy(newCustomer->password, password);
    newCustomer->customer_type = customer_type;
    newCustomer->received_packages = NULL;
    newCustomer->send_packages = NULL;
    newCustomer->next = head;
    newCustomer->ticket = 3;

    printf("用户添加成功！\n");
    return newCustomer; //返回新结点地址
}

// 查询用户信息
void queryCustomer(struct customer *head) {
    if (!head) { // 处理空链表报错
        printf("当前没有用户数据。\n");
        return;
    }
    char temp_phone[MAX_LEN];
    char phone_number[11];
    do {
        printf("请输入要查询的电话号码: \n");
        scanf("%s", temp_phone);
        if (!isPhoneNumberValid(temp_phone)) {
            printf("电话号码格式错误,必须为11位数字!\n");
            }
        } while (!isPhoneNumberValid(temp_phone));
        strcpy(phone_number, temp_phone);

    struct customer *current = head;
    while (current) { // 遍历链表
                      // 判断电话号码是否匹配
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

    char temp_phone[MAX_LEN];
    char phone_number[11];
    do {
        printf("请输入要删除的电话号码: \n");
        scanf("%s", temp_phone);
        if (!isPhoneNumberValid(temp_phone)) {
            printf("电话号码格式错误,必须为11位数字!\n");
            }
        } while (!isPhoneNumberValid(temp_phone));
        strcpy(phone_number, temp_phone);

    struct customer *current = head, *prev = NULL;
    while (current) { // 遍历链表查找用户
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
    while (head) { // 释放函数
        struct customer *temp = head;
        head = head->next;
        free(temp);
    }
}

//判断电话号码是否重复（以电话号为唯一身份标识）
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

//集成函数，便于调用
struct customer *customersCreating(struct customer *customerList) {
    customerList = loadCustomers();
    int choice;
    do {
        displayMenu_customer();
        printf("请输入您的选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: { // 添加新用户（管理员权限）
                char username[100], phone_number[100], password[100];
                int customer_type;

                // 输入电话号码并检查是否重复
                do {
                    char temp_phone[MAX_LEN];
                    
                    do {
                        printf("请输入电话号码: \n");
                        scanf("%s", temp_phone);
                        if (!isPhoneNumberValid(temp_phone)) {
                            printf("电话号码格式错误,必须为11位数字!\n");
                            }
                        } while (!isPhoneNumberValid(temp_phone));
                        strcpy(phone_number, temp_phone);
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
                saveCustomers(customerList);
                break;
            }
            case 2: // 查询用户信息
                queryCustomer(customerList);
                break;
            case 3: // 删除用户
                customerList = deleteCustomer(customerList);
                break;
            case 4: // 保存到文件中
                saveCustomers(customerList);
                break;
            case 5: // 从文件中读取数据
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

//获取用户类型
int get_customer_type(const char *phone_number) {
    FILE *file = fopen("customers.txt", "r");
    if (!file) {
        perror("无法打开 customers.txt 文件");
        return -1; // 返回 -1 表示文件打开失败
    }

    char current_phone[MAX_LEN];
    char current_name[MAX_LEN];
    char current_password[MAX_LEN];
    int current_type;

    // 遍历文件内容，查找匹配的电话号码
    while (fscanf(file, "%s %s %s %d", current_name, current_phone, current_password,&current_type) != EOF) {
        if (strcmp(current_phone, phone_number) == 0) {
            fclose(file);
            return current_type; // 返回匹配的用户类型
        }
    }

    fclose(file);
    return -1; // 返回 -1 表示未找到匹配的用户
}