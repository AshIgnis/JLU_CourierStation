#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "fileOperations.h"
#include "structure.h"
#include "customer.h"
#include "sendPackage.h"
#include "receivedPackage.h"
#include <stdbool.h>
#include <ctype.h>
#include "charge_calculation.h"
// 电话号码验证函数
bool isPhoneNumberValid(const char *phone_number)
{
    if (strlen(phone_number) != 11)
    {
        return false; // 长度不合法
    }
    for (int i = 0; i < 11; i++)
    {
        if (!isdigit(phone_number[i]))
        {
            return false; // 包含非数字字符
        }
    }
    return true; // 合法
}

// 用户操作界面
void userOperating(struct customer *head) {
    printf("欢迎使用用户操作系统！\n");
    printf("请登录您的账户。\n");

    char phone_number[MAX_LEN]; // 电话号码
    char password[MAX_LEN];     // 密码

    // 电话号码输入及验证
    printf("请输入电话号码 (按q退出): \n");
    do {
        scanf("%s", phone_number);
        if (strcmp(phone_number, "q") == 0) {
            printf("已退出用户操作系统。\n");
            return;
        }
        if (!isPhoneNumberValid(phone_number)) {
            printf("电话号码格式错误,必须为11位数字!\n");
        }
    } while (!isPhoneNumberValid(phone_number));

    // 密码输入及验证
    printf("请输入密码 (8-20 个字符, 按q退出): \n");
    do {
        scanf("%s", password);
        if (strcmp(password, "q") == 0) {
            printf("已退出用户操作系统。\n");
            return;
        }
        int length = strlen(password);
        if (length < 8 || length > 20) {
            printf("密码长度必须在 8 到 20 个字符之间，请重新输入: \n");
        } else {
            break; // 密码长度合法，退出循环
        }
    } while (1);

    int flag = userLanding(head, phone_number, password);
    system("pause");
    system("cls");

    while (!flag) { // 登录失败
        int choice;
        printf("电话号或密码错误！\n");
        printf("1. 注册\n");
        printf("2. 重新输入\n");
        printf("0. 退出\n");
        printf("请输入您的选择 : \n");

        char temp_choice[MAX_LEN];
        scanf("%s", temp_choice);
        if (strcmp(temp_choice, "q") == 0) {
            printf("已退出用户操作系统。\n");
            return;
        }
        choice = atoi(temp_choice);

        switch (choice) {
        case 1: { // 注册新用户
            head = addCustomer(head); // 调用 customer.c 中的 addCustomer 函数
            saveCustomers(head);      // 保存用户到文件
            printf("注册成功！\n");
            flag = 1; // 注册成功后退出循环
            system("pause");
            system("cls");
            break;
        }
        case 2: { // 重新输入
            printf("请重新输入电话号码和密码。\n");

            char temp_phone[MAX_LEN];
            do {
                printf("请输入电话号码 (按q退出): \n");
                scanf("%s", temp_phone);
                if (strcmp(temp_phone, "q") == 0) {
                    printf("已退出重新输入操作。\n");
                    return;
                }
                if (!isPhoneNumberValid(temp_phone)) {
                    printf("电话号码格式错误,必须为11位数字\n");
                }
            } while (!isPhoneNumberValid(temp_phone));
            strcpy(phone_number, temp_phone);

            printf("请输入密码 (8-20 个字符, 按q退出): \n");
            do {
                scanf("%s", password);
                if (strcmp(password, "q") == 0) {
                    printf("已退出重新输入操作。\n");
                    return;
                }
                int length = strlen(password);
                if (length < 8 || length > 20) {
                    printf("密码长度必须在 8 到 20 个字符之间，请重新输入: \n");
                } else {
                    break; // 密码长度合法，退出循环
                }
            } while (1);

            flag = userLanding(head, phone_number, password);
            system("pause");
            system("cls");
            break;
        }
        case 0: // 退出
            printf("退出系统\n");
            system("pause");
            system("cls");
            return; // 直接返回，退出函数
        default:
            printf("无效的选择，请重试。\n");
            system("pause");
            system("cls");
        }
    }
    userAction(head, phone_number);
}

// 用户登录界面
bool userLanding(struct customer *head, const char *phone_number, const char *password)
{
    struct customer *current = head;
    while (current)
    {
        if (strcmp(current->phone_number, phone_number) == 0 && strcmp(current->password, password) == 0)
        {
            printf("登陆成功！\n");
            printf("您的用户信息如下:\n");
            printf("用户信息:\n");
            printf("用户名: %s\n", current->username);
            printf("电话号码: %s\n", current->phone_number);
            printf("用户类型: %s\n", cstmType[current->customer_type - 1]); // 使用字符串数组
            printf("票数: %d\n", current->tickets);
            return true;
        }
        current = current->next;
    }

    printf("电话号码或密码错误!\n");
    return false;
}

void displayMenu_user()
{
    printf("\n========== 用户操作系统 ==========\n");
    printf("1. 查看用户信息\n");
    printf("2. 查询收件包裹信息\n");
    printf("3. 邮寄包裹\n");
    printf("0. 返回主菜单\n");
    printf("=================================\n");
}

// 登录成功后续功能
void userAction(struct customer *head, const char *phone_number)
{
    int choice;
    do {
        displayMenu_user();
        printf("请输入您的选择:\n ");
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入数字。\n");
            while (getchar() != '\n')
                ; // 清理输入缓冲区
            system("pause");
            system("cls");
            continue;
        }

        switch (choice) {
        case 1: { // 查看用户信息
            // 重新加载用户数据
            head = loadCustomers();

            struct customer *current = head;
            int found = 0;
            while (current) {
                if (strcmp(current->phone_number, phone_number) == 0) {
                    printf("用户信息:\n");
                    printf("用户名: %s\n", current->username);
                    printf("电话号码: %s\n", current->phone_number);
                    printf("用户类型: %s\n", cstmType[current->customer_type - 1]); // 使用字符串数组
                    printf("票数: %d\n", current->tickets);
                    found = 1;
                    break;
                }
                current = current->next;
            }
            if (!found) {
                printf("未找到用户信息。\n");
            }
            freeCustomers(head); // 释放重新加载的链表
            system("pause");
            system("cls");
            break;
        }
        case 2: { // 查询收件包裹信息
            printf("您的包裹信息如下:\n");
            userReceivedPackagesSearching(phone_number);

            int choice_package;
            printf("1. 取件\n");
            printf("0. 退出\n");
            printf("请输入您的选择: ");

            if (scanf("%d", &choice_package) != 1) {
                printf("输入无效，请输入数字。\n");
                while (getchar() != '\n')
                    ; // 清理输入缓冲区
                system("pause");
                system("cls");
                continue;
            }

            switch (choice_package) {
            case 1: // 取件
                userTakePackage(phone_number);
                break;
            case 0: // 退出
                printf("返回主菜单\n");
                system("pause");
                system("cls");
                continue; // 直接返回，退出函数
            default:
                printf("无效的选择，请重试。\n");
                system("pause");
                system("cls");
            }
            break;
        }
        case 3: { // 邮寄包裹
            printf("进入寄件包裹管理系统...\n");
            package_s_original_start((char *)phone_number);
            system("pause");
            system("cls");
            break;
        }
        case 0:
            printf("返回主菜单\n");
            system("pause");
            system("cls");
            return;
        default:
            printf("无效的选择，请重试。\n");
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

// 用户查找包裹
void userReceivedPackagesSearching(const char *phone_number)
{
    FILE *file = fopen(RECEIVED_FILE, "r");
    if (!file)
    {
        perror("无法打开文件");
        return;
    }

    char package_id[MAX_LEN];
    char package_phone[MAX_LEN];
    double volume;
    int package_type, ifCollection, package_status;
    double shipping_fee;
    int found = 0;

    while (fscanf(file, "%s %lf %d %d %lf %d %s", package_phone, &volume, &package_type, &ifCollection, &shipping_fee, &package_status, package_id) != EOF)
    {
        if (strcmp(package_phone, phone_number) == 0)
        {
            printf("包裹取件码: %s\n", package_id);
            found = 1;
        }
    }

    if (!found)
    {
        printf("未找到与您相关的包裹。\n");
    }

    fclose(file);
}

// 用户取走包裹
void userTakePackage(const char *phone_number)
{
    char package_id[MAX_LEN];
    printf("请输入取件码('q'退出): ");
    scanf("%s", package_id);

    if (strcmp(package_id, "q") == 0)
    {
        printf("已退出取件操作。\n");
        return;
    }

    int ifdoortodoor = 0;
    printf("是否需要上门服务 (0-不需要, 1-需要): ");
    while (scanf("%d", &ifdoortodoor) != 1 || (ifdoortodoor != 0 && ifdoortodoor != 1))
    {
        printf("输入无效，请输入0或1: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }

    FILE *file = fopen(RECEIVED_FILE, "r");
    if (!file)
    {
        perror("无法打开 received_packages.txt 文件");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp)
    {
        perror("无法创建临时文件 temp.txt");
        fclose(file);
        return;
    }

    FILE *box = fopen("id_box.txt", "a");
    if (!box)
    {
        perror("无法打开 id_box.txt 文件");
        fclose(file);
        fclose(temp);
        return;
    }

    char current_package_id[MAX_LEN];
    char current_phone[MAX_LEN];
    double current_volume;
    int current_package_type;
    int current_ifCollection;
    double current_shipping_fee;
    int current_package_status;
    int found = 0;

    int return_num = 0;
    double door_to_door_fee = 0.0; // 上门服务费用
    double total_fee = 0.0;        // 总费用

    // 遍历文件内容，找到并跳过目标包裹
    while (fscanf(file, "%s %lf %d %d %lf %d %s", current_phone, &current_volume, &current_package_type, &current_ifCollection, &current_shipping_fee, &current_package_status, current_package_id) != EOF)
    {
        if (strcmp(current_package_id, package_id) == 0 && strcmp(current_phone, phone_number) == 0)
        {
            found = 1; // 找到包裹，跳过写入
            return_num = convertStringToInt(current_package_id);
            fprintf(box, " %d", return_num);

            // 计算上门服务费用
            if (ifdoortodoor == 1)
            {
                door_to_door_fee = DoorToDoorFee_r(current_phone,current_volume,current_package_type); 
                printf("上门服务费用: %.2lf 元\n", door_to_door_fee);
            }

            // 计算总费用
            total_fee = current_shipping_fee + door_to_door_fee;

            // 输出费用信息
            printf("到付运费: %.2lf 元\n", current_shipping_fee);
            printf("总费用: %.2lf 元\n", total_fee);
        }
        else
        {
            fprintf(temp, "%s %lf %d %d %lf %d %s\n", current_phone, current_volume, current_package_type, current_ifCollection, current_shipping_fee, current_package_status, current_package_id);
        }
    }

    fclose(file);
    fclose(temp);
    fclose(box);

    // 删除原文件并重命名临时文件
    if (remove(RECEIVED_FILE) != 0)
    {
        perror("删除 receivedPackage.txt 文件失败");
        return;
    }

    if (rename("temp.txt", RECEIVED_FILE) != 0)
    {
        perror("重命名 temp.txt 文件失败");
        return;
    }

    if (found)
    {
        printf("包裹取走成功！\n");
    }
    else
    {
        printf("未找到对应的包裹。\n");
    }
}

// 字符串转整数
int convertStringToInt(const char *input)
{
    // 检查输入是否为空
    if (input == NULL)
    {
        printf("输入为空！\n");
        return -1;
    }

    // 提取字符串中的数字部分
    int part1 = 0, part2 = 0;
    char a;
    sscanf(input, "%c%2d-%4d", &a, &part1, &part2);

    // 计算结果
    int result = (part1 - 1) * 10000 + part2;
    return result;
}