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
struct customer *addCustomer(struct customer *head, const char *username, const char *phone_number, const char *password, int customer_type)
{
    struct customer *newCustomer = (struct customer *)malloc(sizeof(struct customer)); // 新建用户结点
    if (!newCustomer)
    { // 构建失败返回
        perror("Failed to allocate memory for new customer");
        return head;
    }
    // 值拷贝
    strcpy(newCustomer->username, username);
    strcpy(newCustomer->phone_number, phone_number);
    strcpy(newCustomer->password, password);
    newCustomer->customer_type = customer_type;
    newCustomer->received_packages = NULL;
    newCustomer->send_packages = NULL;
    newCustomer->next = head;
    newCustomer->ticket = 10;

    printf("用户添加成功！\n");
    return newCustomer; // 返回新结点地址
}

// 查询用户信息
void queryCustomer(struct customer *head)
{
    if (!head)
    { // 处理空链表报错
        printf("当前没有用户数据。\n");
        return;
    }
    char temp_phone[MAX_LEN];
    char phone_number[11];
    do
    {
        printf("请输入要查询的电话号码 (按q退出): \n");
        scanf("%s", temp_phone);
        if (strcmp(temp_phone, "q") == 0)
        {
            printf("已退出查询操作。\n");
            return;
        }
        if (!isPhoneNumberValid(temp_phone))
        {
            printf("电话号码格式错误,必须为11位数字!\n");
        }
    } while (!isPhoneNumberValid(temp_phone));
    strcpy(phone_number, temp_phone);

    struct customer *current = head;
    while (current)
    { // 遍历链表
        if (strcmp(current->phone_number, phone_number) == 0)
        {
            printf("\n用户信息:\n");
            printf("用户名: %s\n", current->username);
            printf("电话号码: %s\n", current->phone_number);
            switch (current->customer_type)
            {
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
struct customer *deleteCustomer(struct customer *head)
{
    if (!head)
    {
        printf("当前没有用户数据。\n");
        return head;
    }

    char temp_phone[MAX_LEN];
    char phone_number[11];
    do
    {
        printf("请输入要删除的电话号码 (按q退出): \n");
        scanf("%s", temp_phone);
        if (strcmp(temp_phone, "q") == 0)
        {
            printf("已退出删除操作。\n");
            return head;
        }
        if (!isPhoneNumberValid(temp_phone))
        {
            printf("电话号码格式错误,必须为11位数字!\n");
        }
    } while (!isPhoneNumberValid(temp_phone));
    strcpy(phone_number, temp_phone);

    struct customer *current = head, *prev = NULL;
    while (current)
    { // 遍历链表查找用户
        if (strcmp(current->phone_number, phone_number) == 0)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
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
void freeCustomers(struct customer *head)
{
    while (head)
    { // 释放函数
        struct customer *temp = head;
        head = head->next;
        free(temp);
    }
}

// 判断电话号码是否重复（以电话号为唯一身份标识）
int isPhoneNumberDuplicate(struct customer *head, const char *phone_number)
{
    struct customer *current = head;
    while (current)
    {
        if (strcmp(current->phone_number, phone_number) == 0)
        {
            return 1; // 电话号码已存在
        }
        current = current->next;
    }
    return 0; // 电话号码不存在
}

// 显示所有用户信息
void displayAllCustomers(struct customer *head)
{
    if (!head)
    {
        printf("当前没有用户数据。\n");
        return;
    }

    printf("\n========== 所有用户信息 ==========\n");
    printf("%-5s %-18s %-15s %-10s\n", "序号", "电话号码", "用户名", "用户类型");
    printf("----------------------------------------------------------\n");

    struct customer *current = head;
    int count = 0;

    while (current)
    {
        count++;
        char *customer_type;
        switch (current->customer_type)
        {
        case 1:
            customer_type = "普通用户";
            break;
        case 2:
            customer_type = "VIP用户";
            break;
        case 3:
            customer_type = "企业用户";
            break;
        case 4:
            customer_type = "学生用户";
            break;
        case 5:
            customer_type = "老年用户";
            break;
        default:
            customer_type = "非法用户";
            break;
        }
        printf("%-5d %-15s %-10s %-10s\n", count, current->phone_number, current->username, customer_type);
        current = current->next;
    }

    printf("----------------------------------------------------------\n");
    printf("总计 %d 个用户。\n", count);
}

void displayMenu_customer()
{
    printf("\n========== 包裹管理系统 ==========\n");
    printf("1. 添加用户\n");
    printf("2. 查询用户信息\n");
    printf("3. 显示所有用户\n");
    printf("4. 删除用户\n");
    printf("0. 退出系统\n");
    printf("=================================\n");
}

// 集成函数，便于调用
struct customer *customersCreating(struct customer *customerList)
{
    customerList = loadCustomers();
    int choice;
    do
    {
        displayMenu_customer();
        printf("请输入您的选择 (按q退出): ");
        char temp_choice[10];
        scanf("%s", temp_choice);
        if (strcmp(temp_choice, "q") == 0)
        {
            printf("已退出用户管理系统。\n");
            return customerList;
        }
        choice = atoi(temp_choice); // 将输入转换为整数

        switch (choice)
        {
        case 1:
        { // 添加新用户（管理员权限）
            char username[100], phone_number[100], password[100];
            int customer_type;

            // 输入电话号码并检查是否重复
            do
            {
                char temp_phone[MAX_LEN];
                do
                {
                    printf("请输入电话号码 (按q退出): \n");
                    scanf("%s", temp_phone);
                    if (strcmp(temp_phone, "q") == 0)
                    {
                        printf("已退出添加用户操作。\n");
                        return customerList;
                    }
                    if (!isPhoneNumberValid(temp_phone))
                    {
                        printf("电话号码格式错误,必须为11位数字!\n");
                    }
                } while (!isPhoneNumberValid(temp_phone));
                strcpy(phone_number, temp_phone);
                if (isPhoneNumberDuplicate(customerList, phone_number))
                {
                    printf("电话号码已存在，请重新输入！\n");
                }
            } while (isPhoneNumberDuplicate(customerList, phone_number));

            printf("请输入用户名 (1-10 个字符, 按q退出): \n");
            do
            {
                scanf("%s", username);
                if (strcmp(username, "q") == 0)
                {
                    printf("已退出添加用户操作。\n");
                    return customerList;
                }
                int length = strlen(username);
                if (length < 1 || length > 10)
                {
                    printf("用户名长度必须在 1 到 10 个字符之间，请重新输入: \n");
                }
                else
                {
                    break; // 用户名长度合法，退出循环
                }
            } while (1);

            printf("请输入密码 (8-20 个字符, 按q退出): \n");
            do
            {
                scanf("%s", password);
                if (strcmp(password, "q") == 0)
                {
                    printf("已退出用户添加。\n");
                    return customerList;
                }
                int length = strlen(password);
                if (length < 8 || length > 20)
                {
                    printf("密码长度必须在 8 到 20 个字符之间，请重新输入: \n");
                }
                else
                {
                    break; // 密码长度合法，退出循环
                }
            } while (1);

            printf("请输入用户类型 (1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户, 按q退出): ");
            char temp_type[10];
            scanf("%s", temp_type);
            if (strcmp(temp_type, "q") == 0)
            {
                printf("已退出添加用户操作。\n");
                return customerList;
            }
            customer_type = atoi(temp_type);
            while (customer_type < 1 || customer_type > 5)
            {
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
        case 3:                                // 显示所有用户
            displayAllCustomers(customerList); // 调用新增函数
            break;
        case 4: // 删除用户
            customerList = deleteCustomer(customerList);
            saveCustomers(customerList);
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