#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sendPackage.h"
#include "structure.h"
#include "charge_calculation.h"
#include "customer.h"
#define adminPhoneNumber "20245524116" //增加管理员测试用的号码
//添加包裹
void add_package_s(struct package_s *head, const char *phone_number) {
    struct package_s* now = (struct package_s*)malloc(sizeof(struct package_s));
    memset(now, 0, sizeof(struct package_s)); // 初始化为 0

    strncpy(now->phone_number, phone_number, MAX_LEN - 1);

    int ifdoortodoor = 0; // 是否上门服务
    printf("1.请输入收件人姓名 (不超过10个字符,不含空格): \n");
    while (scanf("%10s", now->receiver_name) != 1 || strlen(now->receiver_name) > 10) {
        printf("输入无效，请输入不超过10个字符的姓名: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    printf("2.请输入收件人地址 (不超过20个字符,不含空格): \n");
    while (scanf("%20s", now->receiver_address) != 1 || strlen(now->receiver_address) > 20) {
        printf("输入无效，请输入不超过20个字符的地址: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    printf("3.请输入包裹体积 (立方厘米): \n");
    while (scanf("%lf", &now->volume) != 1 || now->volume <= 0||now->volume > 1000000) {
        printf("输入无效，请输入一个正数: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    printf("4.请输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
    while (scanf("%d", &now->package_type) != 1 || now->package_type < 1 || now->package_type > 5) {
        printf("输入无效，请输入1-5之间的数字: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    printf("5.在此缴纳运费 (0-不, 1-是): \n");
    while (scanf("%d", &now->ifCollection) != 1 || (now->ifCollection != 0 && now->ifCollection != 1)) {
        printf("输入无效，请输入0或1: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    printf("6.请输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
    while (scanf("%d", &now->package_status) != 1 || now->package_status < 1 || now->package_status > 3) {
        printf("输入无效，请输入1-3之间的数字: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    printf("7.是否需要上门服务 (0-不, 1-是): \n");
    while (scanf("%d", &ifdoortodoor) != 1 || (ifdoortodoor != 0 && ifdoortodoor != 1)) {
        printf("输入无效，请输入0或1: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    double shipping_fee = calculate_send_package_fees(now, ifdoortodoor, phone_number); // 计算运费
    printf("包裹的总计算为：%.2lf\n", shipping_fee);

    now->next = head->next;
    head->next = now;

    printf("寄件包裹添加成功！\n");
    save_package_s(head); // 隐式保存
}

void query_and_show_packages(struct package_s *head, const char *phone_number) {
    if (head == NULL || head->next == NULL) {
        printf("当前没有任何包裹信息。\n");
        return;
    }

    struct package_s *current = head->next; // 跳过头节点
    int count = 0;

    printf("\n========================== 寄件包裹信息 ===========================\n");
    while (current != NULL) {
        if(strcmp(current->phone_number,"1")==0) break;
        // 如果是管理员，显示所有包裹；如果是普通用户，只显示与其电话号码匹配的包裹
        if (phone_number == NULL || strcmp(current->phone_number, phone_number) == 0) {
            count++;
            printf("序号: %-5d 发件电话：%-11s 收件人姓名: %-12s\n", count, current->phone_number, current->receiver_name);
            printf("收件地址: %-20s\n", current->receiver_address);
            printf("体积(cm³): %-12.2lf 类型: %-10s 到付: %-10s 状态: %-10s 运费(元): %-7.2lf\n\n",
                   current->volume,
                   pkgType[current->package_type - 1], // 包裹类型中文
                   ifcollection[current->ifCollection], // 是否到付中文
                   pkgStatus[current->package_status - 1], // 包裹状态中文
                   current->shipping_fee);
        }
        current = current->next;
    }

    if (count == 0) {
        printf("没有找到符合条件的包裹。\n");
    } else {
        printf("---------------------------------------------------------------------\n");
        printf("总计 %d 个寄件包裹。\n", count);
    }
}

void delete_package_s(struct package_s* head, const char* phone_number) {
    if (head == NULL || head->next == NULL) {
        printf("当前没有任何包裹信息。\n");
        return;
    }

    struct package_s *prev = head, *current = head->next;
    int count = 0;

    printf("\n========================== 删除包裹信息 ===========================\n");
    while (current != NULL) {
        // 如果是管理员，显示所有包裹；如果是普通用户，只显示与其电话号码匹配的包裹
        if (phone_number == NULL || strcmp(current->phone_number, phone_number) == 0) {
            count++;
            printf("序号: %-5d 发件电话：%-11s 收件人姓名: %-12s\n", count, current->phone_number, current->receiver_name);
            printf("收件地址: %-20s\n", current->receiver_address);
            printf("体积(cm³): %-12.2lf 类型: %-10s 到付: %-10s 状态: %-10s 运费(元): %-7.2lf\n\n",
                   current->volume,
                   pkgType[current->package_type - 1], // 包裹类型中文
                   ifcollection[current->ifCollection], // 是否到付中文
                   pkgStatus[current->package_status - 1], // 包裹状态中文
                   current->shipping_fee);
        }
        current = current->next;
    }

    if (count == 0) {
        printf("没有找到符合条件的包裹。\n");
    } else {
        printf("---------------------------------------------------------------------\n");
        printf("总计 %d 个寄件包裹。\n", count);
    }

    printf("请输入要删除的包裹序号: ");
    int delete_index;
    while (scanf("%d", &delete_index) != 1 || delete_index < 1 || delete_index > count) {
        printf("输入无效，请输入有效的序号: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    // 确认是否删除
    char confirm;
    printf("是否确认删除包裹序号 %d？(Y/N): ", delete_index);
    while (1) {
        scanf(" %c", &confirm);
        if (confirm == 'Y' || confirm == 'y') {
            break; // 确认删除
        } else if (confirm == 'N' || confirm == 'n') {
            printf("取消删除操作。\n");
            return;
        } else {
            printf("输入无效，请输入 Y 或 N: ");
        }
    }

    prev = head;
    current = head->next;
    count = 0;
    while (current != NULL) {
        if (phone_number == NULL || strcmp(current->phone_number, phone_number) == 0) {
            count++;
            if (count == delete_index) {
                prev->next = current->next;
                free(current);
                printf("包裹已成功删除！\n");
                return;
            }
        }
        prev = current;
        current = current->next;
    }
}

struct package_s* load_package_s() {
    FILE *file = fopen(SEND_FILE, "r");
    if (!file) {
        perror("No package file found");
        return NULL;
    }

    struct package_s *head = (struct package_s*)malloc(sizeof(struct package_s));
    head->next = NULL;
    struct package_s *lst = head;

    while (1) {
        struct package_s* now = (struct package_s*)malloc(sizeof(struct package_s));
        if (fscanf(file, "%s %s %s %lf %d %d %d %lf",
                   now->phone_number,
                   now->receiver_name,
                   now->receiver_address,
                   &now->volume,
                   &now->package_type,
                   &now->ifCollection,
                   &now->package_status,
                   &now->shipping_fee) != 8) {
            free(now);
            break;
        }

        // 校验数据合法性
        if (strlen(now->phone_number) != 11 || now->volume <= 0 || 
            now->package_type < 1 || now->package_type > 5 || 
            now->ifCollection < 0 || now->ifCollection > 1 || 
            now->package_status < 1 || now->package_status > 3) {
            free(now);
            continue; // 跳过无效数据
        }

        lst->next = now;
        lst = now;
        lst->next = NULL;
    }

    fclose(file);
    return head;
}

void save_package_s(struct package_s* head) {
    FILE *file = fopen(SEND_FILE, "w");
    if (!file) {
        perror("Failed to open file for saving packages");
        return;
    }

    struct package_s *lst = head->next; // 跳过头节点
    while (lst != NULL) {
        if (strlen(lst->phone_number) == 0 || strlen(lst->receiver_name) == 0 || lst->volume <= 0) {
            lst = lst->next;
            continue; // 跳过无效节点
        }

        fprintf(file, "%s %s %s %.2lf %d %d %d %.2lf\n",
                lst->phone_number,
                lst->receiver_name,
                lst->receiver_address,
                lst->volume,
                lst->package_type,
                lst->ifCollection,
                lst->package_status,
                lst->shipping_fee);
        lst = lst->next;
    }

    fclose(file);
}

void free_package_s(struct package_s* head){
    struct package_s *lst,*now;
    lst=head;
    for(; ;){
        now=(*lst).next;
        free(lst);
        if(now==NULL){
            break;
        }lst=now;
    }return;
}

void package_s_original_start(char* t) {
    struct package_s *head = load_package_s();
    char nm[22]; // nm是电话号码
    for (int i = 0; i < 11; i++) {
        nm[i] = t[i];
    }
    nm[11] = '\0';
    int op;
    for (;;) {
        printf("\n========== 寄件包裹管理系统 ==========\n");
        printf("1. 添加寄件包裹\n");
        printf("2. 查询寄件包裹\n");
        printf("3. 删除寄件包裹\n");
        printf("0. 返回上一级\n");
        printf("=====================================\n");
        printf("请输入您的选择: ");
        while (scanf("%d", &op) != 1 || op < 0 || op > 3) {
            printf("输入无效，请输入0-3之间的数字: ");
            while (getchar() != '\n'); // 清空输入缓冲区
        }
        if (op == 0) {
            free_package_s(head);
            break;
        }
        if (op == 1) { // 添加寄件包裹
            add_package_s(head, nm);
        }
        if (op == 2) { // 查询寄件包裹
            if (strcmp(nm, adminPhoneNumber) == 0) {
                query_and_show_packages(head, NULL); // 管理员查询所有包裹
            } else {
                query_and_show_packages(head, nm); // 普通用户查询自己的包裹
            }
        }
        if (op == 3) { // 删除寄件包裹
            if (strcmp(nm, adminPhoneNumber) == 0) {
                delete_package_s(head, NULL); // 管理员删除任意包裹
            } else {
                delete_package_s(head, nm); // 普通用户只能删除自己的包裹
            }
            save_package_s(head); // 隐式保存
        }
    }
    return;
}
