#include "user.h"
#include "fileOperations.h"
#include "structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 用户操作界面
void userOperating(struct customer *head) {
    char phone_number[MAX_LEN]; // 电话号码
    char password[MAX_LEN]; // 密码

    printf("请输入电话号码: ");
    fgets(phone_number, MAX_LEN, stdin);
    phone_number[strcspn(phone_number, "\n")] = '\0'; // 去除换行符

    printf("请输入密码: ");
    fgets(password, MAX_LEN, stdin);
    password[strcspn(password, "\n")] = '\0'; // 去除换行符

    int flag = userLanding(head, phone_number, password);
    while (!flag) {
        int choice;
        printf("1. 注册\n");
        printf("2. 重新输入\n");
        printf("0. 退出\n");
        printf("请输入您的选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // 注册新用户
                char username[MAX_LEN];
                int customer_type;

                printf("请输入用户名: ");
                scanf("%s", username);
                printf("请输入用户类型 (1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户): ");
                scanf("%d", &customer_type);
                while (customer_type < 1 || customer_type > 5) {
                    printf("输入数据非法，请重新输入！\n");
                    printf("请输入用户类型 (1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户): ");
                    scanf("%d", &customer_type);
                }

                // 添加新用户到链表
                head = addCustomer(head, username, phone_number, password, customer_type);

                // 保存用户到文件
                saveCustomers(head);

                printf("注册成功！\n");
                flag = 1; // 注册成功后退出循环
                system("pause");
                system("cls");
                break;
            }
            case 2:
                // 重新输入
                printf("请重新输入电话号码和密码。\n");
                printf("请输入电话号码: ");
                scanf("%s", phone_number);
                printf("请输入密码: ");
                scanf("%s", password);
                flag = userLanding(head, phone_number, password);
                system("pause");
                system("cls");
                break;
            case 0:
                // 退出
                printf("退出系统\n");
                system("cls");
                return; // 直接返回，退出函数
            default:
                printf("无效的选择，请重试。\n");
        }
    }
    userAction(head, phone_number);
}

// 用户登录界面
bool userLanding(struct customer *head,const char *phone_number, const char *password){
    struct customer *current = head;
    while (current) {
        if (strcmp(current->phone_number, phone_number) == 0 && strcmp(current->password, password) == 0) {
            printf("登陆成功！\n");
            printf("您的用户信息如下:\n");
            printf("用户信息:\n");
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
            return true;
        }
        current = current->next;
    }

    printf("电话号码或密码错误!\n");
    return false;
}

void displayMenu_user(){
    printf("\n========== 用户操作系统 ==========\n");
    printf("1. 查看用户信息\n");
    printf("2. 查询包裹信息\n");
    printf("3. 邮寄包裹\n");
    printf("0. 返回主菜单\n");
    printf("=================================\n");
}

void userAction(struct customer *head,const char *phone_number){
    int choice;
    do {
        displayMenu_user();
        printf("请输入您的选择:\n ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {// 查看用户信息
                struct customer *current = head;
                int found = 0;
                while (current) {
                    if (strcmp(current->phone_number, phone_number) == 0) {
                        printf("用户信息:\n");
                        printf("用户名: %s\n", current->username);
                        printf("电话号码: %s\n", current->phone_number);
                        switch (current->customer_type) {
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
                        found = 1;
                        break;
                    }
                    current = current->next;
                }
                if (!found) {
                    printf("未找到用户信息。\n");
                }
                break;
            }
            case 2: {// 查询收件包裹信息
                printf("您的包裹信息如下:\n");
                userReceivedPackagesSearching(head, phone_number);

                int choice_package;
                printf("1. 取件\n");
                printf("0. 退出\n");
                printf("请输入您的选择: ");
                scanf("%d", &choice_package);

                switch (choice_package) {
                    case 1: // 取件
                        userTakePackage(phone_number);
                        break;
                    case 0: // 退出
                        printf("返回主菜单\n");
                        system("cls");
                        return; // 直接返回，退出函数
                    default:
                        printf("无效的选择，请重试。\n");
                }
                break;
            }
            case 3: // 邮寄包裹

                break;
            case 0:
                printf("返回主菜单\n");
                break;
            default:
                printf("无效的选择，请重试。\n");
        }
        system("pause");
        system("cls");
    } while (choice != 0);

}

void userReceivedPackagesSearching(const char *phone_number) {
    FILE *file = fopen("receivedPackage.txt", "r");
    if (!file) {
        perror("无法打开文件");
        return;
    }

    char package_id[MAX_LEN];
    char package_phone[MAX_LEN];
    int found = 0;

    while (fscanf(file, "%s %s", package_id, package_phone) != EOF) {
        if (strcmp(package_phone, phone_number) == 0) {
            printf("包裹取件码: %s\n", package_id);
            found = 1;
        }
    }

    if (!found) {
        printf("未找到与您相关的包裹。\n");
    }

    fclose(file);
}


void userTakePackage(const char *phone_number) {
    char package_id[MAX_LEN];
    printf("请输入取件码: ");
    scanf("%s", package_id);

    FILE *file = fopen("receivedPackage.txt", "r");
    if (!file) {
        perror("无法打开 receivedPackage.txt 文件");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        perror("无法创建临时文件 temp.txt");
        fclose(file);
        return;
    }


    char current_package_id[MAX_LEN];
    char current_phone[MAX_LEN];
    int found = 0;

    while (fscanf(file, "%s %s", current_package_id, current_phone) != EOF) {
        if (strcmp(current_package_id, package_id) == 0 && strcmp(current_phone, phone_number) == 0) {
            found = 1; // 找到包裹，跳过写入
        } else {
            fprintf(temp, "%s %s\n", current_package_id, current_phone);
        }
    }

    fclose(file);
    fclose(temp);

    if (remove("receivedPackage.txt") != 0) {
        perror("删除 receivedPackage.txt 文件失败");
        return;
    }

    if (rename("temp.txt", "receivedPackage.txt") != 0) {
        perror("重命名 temp.txt 文件失败");
        return;
    }

    if (found) {
        printf("包裹取走成功！\n");
    } else {
        printf("未找到对应的包裹。\n");
    }
}