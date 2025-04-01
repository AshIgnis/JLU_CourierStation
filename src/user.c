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

    printf("请输入用户ID: ");
    scanf("%s", phone_number);
    printf("请输入密码: ");
    scanf("%s", password);

    int flag;
    flag = userLanding(head, phone_number, password);
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
                printf("退出系统...\n");
                system("cls");
                return; // 直接返回，退出函数
            default:
                printf("无效的选择，请重试。\n");
        }
    }

    // 用户登录后续操作
    userAction(head, phone_number);
}

// 用户登录界面
bool userLanding(struct customer *head,const char *phone_number, const char *password){
    struct customer *current = head;
    while (current) {
        if (strcmp(current->phone_number, phone_number) == 0 && strcmp(current->password, password) == 0) {
            printf("登陆成功！\n");
            printf("您的用户信息如下:");
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
            return true;
        }
        current = current->next;
    }

    printf("用户ID或密码错误!\n");
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
        printf("请输入您的选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {// 查看用户信息
                struct customer *current = head;
                while (current) {
                    if (strcmp(current->phone_number, phone_number) == 0) {
                        printf("\n用户信息:\n");
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
                        break;
                    }
                    current = current->next;
                }
                if (!current) {
                    printf("未找到用户信息。\n");
                }
                break;
            }
            case 2: {// 查询收件包裹信息
                userReceivedPackagesSearching(head, phone_number);
                int choice_package;
                printf("1. 取件\n");
                printf("0. 退出\n");
                scanf("%d", &choice_package);
                switch (choice_package) {
                    case 1: // 取件

                    case 0: // 退出
                        printf("返回主菜单...\n");
                        system("cls");
                        return; // 直接返回，退出函数
                    default:
                        printf("无效的选择，请重试。\n");
                break;
            }
                break;
            case 3: // 邮寄包裹

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

}

void userReceivedPackagesSearching(struct package_r *head,const char *phone_number){
    struct package_r *current = head;
    while (current) {
        if (strcmp(current->phone_number, phone_number) == 0) {
            struct package_r *received_packages = current->received_packages;
            struct package_s *send_packages = current->send_packages;

            printf("\n收件包裹信息:\n");
            while (received_packages) {
                printf("包裹ID: %s\n", received_packages->package_id);
                printf("体积: %.2lf\n", received_packages->volume);
                printf("包裹类型: %d\n", received_packages->package_type);
                printf("是否代收: %d\n", received_packages->ifCollection);
                printf("运费: %.2lf\n", received_packages->shipping_fee);
                printf("包裹状态: %d\n", received_packages->package_status);
                printf("用户ID: %d\n", received_packages->user_id);
                received_packages = received_packages->next;
            }
        }
        if (!current) {
            printf("未找到用户信息。\n");
        }
