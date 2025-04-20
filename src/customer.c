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
struct customer *addCustomer(struct customer *head) {
    char username[100], phone_number[100], password[100];
    int customer_type;

    // 输入电话号码并检查是否重复
    do {
        char temp_phone[MAX_LEN];
        do {
            printf("请输入电话号码 (按q退出): \n");
            scanf("%s", temp_phone);
            clearInputBuffer();
            if (strcmp(temp_phone, "q") == 0) {
                printf("已退出添加用户操作。\n");
                return head;
            }
            if (!isPhoneNumberValid(temp_phone)) {
                printf("电话号码格式错误,必须为11位数字!\n");
            }
        } while (!isPhoneNumberValid(temp_phone));
        strcpy(phone_number, temp_phone);
        if (isPhoneNumberDuplicate(head, phone_number)) {
            printf("电话号码已存在，请重新输入！\n");
        }
    } while (isPhoneNumberDuplicate(head, phone_number));

    // 输入用户名
    printf("请输入用户名 (1-10 个字符, 按q退出): \n");
    do {
        scanf("%s", username);
        clearInputBuffer();
        if (strcmp(username, "q") == 0) {
            printf("已退出添加用户操作。\n");
            return head;
        }
        int length = strlen(username);
        if (length < 1 || length > 10) {
            printf("用户名长度必须在 1 到 10 个字符之间，请重新输入: \n");
        } else {
            break; // 用户名长度合法，退出循环
        }
    } while (1);

    // 输入密码
    printf("请输入密码 (8-20 个字符, 按q退出): \n");
    do {
        scanf("%s", password);
        clearInputBuffer();
        if (strcmp(password, "q") == 0) {
            printf("已退出用户添加。\n");
            return head;
        }
        int length = strlen(password);
        if (length < 8 || length > 20) {
            printf("密码长度必须在 8 到 20 个字符之间，请重新输入: \n");
        } else {
            break; // 密码长度合法，退出循环
        }
    } while (1);

    // 输入用户类型
    printf("请输入用户类型 (1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户, 按q退出): ");
    char temp_type[10];
    scanf("%s", temp_type);
    clearInputBuffer();
    if (strcmp(temp_type, "q") == 0) {
        printf("已退出添加用户操作。\n");
        return head;
    }
    customer_type = atoi(temp_type);
    while (customer_type < 1 || customer_type > 5) {
        printf("输入数据非法，请重新输入！\n");
        printf("请输入用户类型 (1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户): ");
        scanf("%d", &customer_type);
        clearInputBuffer();
    }

    // 创建新用户节点
    struct customer *newCustomer = (struct customer *)malloc(sizeof(struct customer)); // 新建用户结点
    if (!newCustomer) { // 构建失败返回
        perror("Failed to allocate memory for new customer");
        return head;
    }

    // 值拷贝
    strcpy(newCustomer->username, username);
    strcpy(newCustomer->phone_number, phone_number);
    strcpy(newCustomer->password, password);
    newCustomer->tickets = 10; // 初始化票数为10
    newCustomer->customer_type = customer_type;
    newCustomer->next = head;

    printf("用户添加成功!新用户赠送10张八折卷,可用于运费!\n");
    return newCustomer; // 返回新结点地址
}

// 查询用户信息
void queryCustomer(struct customer *head) {
    if (!head) {
        printf("当前没有用户数据。\n");
        return;
    }

    char temp_phone[MAX_LEN];
    char phone_number[11];
    do {
        printf("请输入要查询的电话号码 (按q退出): \n");
        scanf("%s", temp_phone);
        clearInputBuffer();
        if (strcmp(temp_phone, "q") == 0) {
            printf("已退出查询操作。\n");
            return;
        }
        if (!isPhoneNumberValid(temp_phone)) {
            printf("电话号码格式错误,必须为11位数字!\n");
        }
    } while (!isPhoneNumberValid(temp_phone));
    strcpy(phone_number, temp_phone);

    struct customer *current = head;
    while (current) {
        if (strcmp(current->phone_number, phone_number) == 0) {
            printf("\n用户信息:\n");
            printf("用户名: %s\n", current->username);
            printf("电话号码: %s\n", current->phone_number);
            if (current->customer_type >= 1 && current->customer_type <= 5) {
                printf("用户类型: %s\n", cstmType[current->customer_type - 1]); // 使用字符串数组
            } else {
                printf("用户类型: 非法用户\n");
            }
            printf("票数: %d\n", current->tickets);
            printf("----------------------------------------------------------\n");
            return;
        }
        current = current->next;
    }

    printf("未找到电话号码为 %s 的用户。\n", phone_number);
}

// 显示所有用户信息
void displayAllCustomers(struct customer *head) {
    if (!head) {
        printf("当前没有用户数据。\n");
        return;
    }

    printf("\n===================== 所有用户信息 =======================\n");
    printf("%-5s %-18s %-15s %-10s %-5s\n", "序号", "电话号码", "用户名", "用户类型","票数");
    printf("----------------------------------------------------------\n");

    struct customer *current = head;
    int count = 0;

    while (current) {
        if (strcmp(current->phone_number, "1") == 0) continue; // 跳过空用户
        count++;
        const char *customer_type = (current->customer_type >= 1 && current->customer_type <= 5)
                                        ? cstmType[current->customer_type - 1]
                                        : "非法用户"; // 使用字符串数组
        printf("%-5d %-15s %-10s %-10s %-5d\n", count, current->phone_number, current->username, customer_type,current->tickets);
        current = current->next;
    }

    printf("----------------------------------------------------------\n");
    printf("总计 %d 个用户。\n", count);
}

// 修改用户信息
struct customer *modifyCustomer(struct customer *head) {
    if (!head) {
        printf("当前没有用户数据。\n");
        return head;
    }

    char temp_phone[MAX_LEN];
    char phone_number[11];
    do {
        printf("请输入要修改的用户电话号码 (按q退出): \n");
        scanf("%s", temp_phone);
        if (strcmp(temp_phone, "q") == 0) {
            printf("已退出修改操作。\n");
            return head;
        }
        if (!isPhoneNumberValid(temp_phone)) {
            printf("电话号码格式错误,必须为11位数字!\n");
        }
    } while (!isPhoneNumberValid(temp_phone));
    strcpy(phone_number, temp_phone);

    struct customer *current = head;
    while (current) {
        if (strcmp(current->phone_number, phone_number) == 0) {
            printf("\n找到用户，当前信息如下:\n");
            printf("用户名: %s\n", current->username);
            printf("电话号码: %s\n", current->phone_number);
            printf("用户类型: %s\n", cstmType[current->customer_type - 1]);
            printf("票数: %d\n", current->tickets);

            // 修改用户名
            printf("请输入新的用户名 (1-10 个字符, 按q跳过): \n");
            char new_username[MAX_LEN];
            scanf("%s", new_username);
            if (strcmp(new_username, "q") != 0) {
                int length = strlen(new_username);
                if (length >= 1 && length <= 10) {
                    strcpy(current->username, new_username);
                } else {
                    printf("用户名长度非法，未修改。\n");
                }
            }

            // 修改密码
            printf("请输入新的密码 (8-20 个字符, 按q跳过): \n");
            char new_password[MAX_LEN];
            scanf("%s", new_password);
            if (strcmp(new_password, "q") != 0) {
                int length = strlen(new_password);
                if (length >= 8 && length <= 20) {
                    strcpy(current->password, new_password);
                } else {
                    printf("密码长度非法，未修改。\n");
                }
            }

            // 修改用户类型
            printf("请输入新的用户类型 (1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户, 按q跳过): \n");
            char temp_type[10];
            scanf("%s", temp_type);
            if (strcmp(temp_type, "q") != 0) {
                int new_type = atoi(temp_type);
                if (new_type >= 1 && new_type <= 5) {
                    current->customer_type = new_type;
                } else {
                    printf("用户类型非法，未修改。\n");
                }
            }

            // 修改票数
            printf("请输入新的票数 (0-100, 按q跳过): \n");
            char temp_tickets[10];
            scanf("%s", temp_tickets);
            if (strcmp(temp_tickets, "q") != 0) {
                int new_tickets = atoi(temp_tickets);
                if (new_tickets >= 0 && new_tickets <= 100) {
                    current->tickets = new_tickets;
                } else {
                    printf("票数非法，未修改。\n");
                }
            }

            printf("用户信息修改成功！\n");
            return head;
        }
        current = current->next;
    }

    printf("未找到电话号码为 %s 的用户。\n", phone_number);
    return head;
}

// 删除用户
struct customer *deleteCustomer(struct customer *head)
{
    if (!head)
    {
        printf("当前没有用户数据。\n");
        return head;
    }
    displayAllCustomers(head); // 显示所有用户信息
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

    printf("未找到电话号码为 %s 的用户。\n",phone_number);
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

void displayMenu_customer() {
    printf("\n========== 用户管理系统 ==========\n");
    printf("1. 添加用户\n");
    printf("2. 查询用户信息\n");
    printf("3. 显示所有用户\n");
    printf("4. 修改用户信息\n");
    printf("5. 删除用户\n");
    printf("6. 更新用户信息\n");
    printf("0. 退出系统\n");
    printf("=================================\n");
}

// 集成函数，便于调用
struct customer *customersOperation(struct customer *customerList) {
    customerList = loadCustomers();
    int choice;
    do {
        displayMenu_customer();
        printf("请输入您的选择 (按q退出): ");
        char temp_choice[10];
        scanf("%s", temp_choice);
        if (strcmp(temp_choice, "q") == 0) {
            printf("已退出用户管理系统。\n");
            return customerList;
        }
        choice = atoi(temp_choice); // 将输入转换为整数

        switch (choice) {
        case 1: // 添加新用户（管理员权限）
            customerList = addCustomer(customerList);
            saveCustomers(customerList);
            break;
        case 2: // 查询用户信息
            queryCustomer(customerList);
            break;
        case 3: // 显示所有用户
            displayAllCustomers(customerList);
            break;
        case 4: // 修改用户信息
            customerList = modifyCustomer(customerList);
            saveCustomers(customerList);
            break;
        case 5: // 删除用户
            customerList = deleteCustomer(customerList);
            saveCustomers(customerList);
            break;
        case 6: // 更新用户信息
            printf("从文件中重新载入用户数据...\n");
            // 释放当前链表
            if (customerList) {
                freeCustomers(customerList);
                customerList = NULL; // 避免悬空指针
            }
            // 从文件重新加载用户数据
            customerList = loadCustomers();
            if (customerList) {
                printf("用户数据已成功从文件中更新！\n");
            } else {
                printf("未能从文件中加载用户数据，可能文件不存在或为空。\n");
            }
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