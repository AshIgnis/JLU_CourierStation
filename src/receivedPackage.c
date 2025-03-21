#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receivedPackage.h"

// 添加收件包裹
void addReceivedPackage(struct customer *user) {
    if (!user) {
        printf("用户不存在，无法添加收件包裹。\n");
        return;
    }

    struct package_r *newPackage = (struct package_r *)malloc(sizeof(struct package_r));
    if (!newPackage) {
        perror("Failed to allocate memory for new received package");
        return;
    }

    printf("请输入包裹ID: ");
    scanf("%s", newPackage->package_id);
    printf("请输入包裹体积 (立方厘米): ");
    scanf("%lf", &newPackage->volume);
    printf("请输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): ");
    scanf("%d", &newPackage->package_type);
    printf("是否需要到付 (0-不需要, 1-需要): ");
    scanf("%d", &newPackage->ifCollection);
    if(newPackage->ifCollection){
        printf("请输入运输费用: ");
        scanf("%lf", &newPackage->shipping_fee);
    }
    printf("请输入包裹状态 (1-正常, 2-损坏, 3-违禁品): ");
    scanf("%d", &newPackage->package_status);

    newPackage->next = user->received_packages;
    user->received_packages = newPackage;

    printf("收件包裹添加成功！\n");
}

// 查询用户的收件包裹
void queryReceivedPackages(struct customer *user) {
    if (!user) {
        printf("用户不存在，无法查询收件包裹。\n");
        return;
    }

    struct package_r *current = user->received_packages;
    if (!current) {
        printf("用户没有收件包裹。\n");
        return;
    }

    printf("用户 %s 的收件包裹列表:\n", user->username);
    while (current) {
        printf("包裹ID: %s, 体积: %.2f, 类型: %d,状态: %d, 到付: %d",
               current->package_id, current->volume, current->package_type,
               current->package_status,current->ifCollection);
        if(current->ifCollection){
            printf("运费:%.2f\n",current->shipping_fee);
        }else{
            printf("\n");
        }
        current = current->next;
    }
}

// 释放收件包裹链表
void freeReceivedPackages(struct package_r *head) {
    while (head) {
        struct package_r *temp = head;
        head = head->next;
        free(temp);
    }
}

// 显示收件包裹管理菜单
void displayMenu_receivedPackage() {
    printf("\n========== 收件包裹管理系统 ==========\n");
    printf("1. 添加收件包裹\n");
    printf("2. 查询收件包裹\n");
    printf("3. 保存收件包裹\n");
    printf("4. 加载收件包裹\n");
    printf("0. 返回主菜单\n");
    printf("=====================================\n");
}

// 收件包裹管理功能
void receivedPackagesCreating(struct customer *customerList) {
    int choice;
    do {
        displayMenu_receivedPackage();
        printf("请输入您的选择: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // 添加收件包裹
                int user_id;
                printf("请输入用户ID: ");
                scanf("%d", &user_id);

                struct customer *current = customerList;
                while (current && current->user_id != user_id) {
                    current = current->next;
                }

                if (!current) {
                    printf("未找到用户ID为 %d 的用户。\n", user_id);
                    break;
                }

                addReceivedPackage(current);
                break;
            }
            case 2: {
                // 查询收件包裹
                int user_id;
                printf("请输入用户ID: ");
                scanf("%d", &user_id);

                struct customer *current = customerList;
                while (current && current->user_id != user_id) {
                    current = current->next;
                }

                if (!current) {
                    printf("未找到用户ID为 %d 的用户。\n", user_id);
                    break;
                }

                queryReceivedPackages(current);
                break;
            }
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