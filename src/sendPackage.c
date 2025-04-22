#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sendPackage.h"
#include "structure.h"
#include "charge_calculation.h"
#include "customer.h"
#define adminPhoneNumber "20245524116" // 增加管理员测试用的号码
// 添加包裹
void add_package_s(struct package_s *head, const char *phone_number)
{
    struct package_s *now = (struct package_s *)malloc(sizeof(struct package_s));
    memset(now, 0, sizeof(struct package_s)); // 初始化为 0

    strncpy(now->phone_number, phone_number, MAX_LEN - 1);

    // 输入收件人姓名
    printf("1.请输入收件人姓名 (不超过10个字符): \n");
    do
    {
        printf("> ");
        scanf("%s", now->receiver_name);
        while (getchar() != '\n')
            ; // 清空输入缓冲区
        if (strlen(now->receiver_name) > 0 && strlen(now->receiver_name) <= 10)
        {
            break;
        }
        printf("输入无效，请输入不超过10个字符的姓名: \n");
    } while (1);

    // 输入收件人地址
    printf("2.请输入收件人地址 (不超过20个字符): \n");
    do
    {
        printf("> ");
        scanf("%s", now->receiver_address);
        while (getchar() != '\n')
            ; // 清空输入缓冲区
        if (strlen(now->receiver_address) > 0 && strlen(now->receiver_address) <= 20)
        {
            break;
        }
        printf("输入无效，请输入不超过20个字符的地址: \n");
    } while (1);

    // 输入包裹体积
    printf("3.请输入包裹体积 (立方厘米): \n");
    do
    {
        printf("> ");
        char temp[MAX_LEN];
        scanf("%s", temp);
        while (getchar() != '\n')
            ; // 清空输入缓冲区
        if (isValidFloat(temp))
        {
            now->volume = atof(temp); // 将字符串转换为浮点数
            if (now->volume > 0 && now->volume <= 1000000)
            {
                break;
            }
        }
        printf("输入无效，请输入一个正数 (1-1000000): \n");
    } while (1);

    // 输入包裹类型
    printf("4.请输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
    do
    {
        printf("> ");
        char temp[MAX_LEN];
        scanf("%s", temp);
        while (getchar() != '\n')
            ; // 清空输入缓冲区
        if (isValidNumber(temp))
        {
            now->package_type = atoi(temp); // 将字符串转换为整数
            if (now->package_type >= 1 && now->package_type <= 5)
            {
                break;
            }
        }
        printf("输入无效，请输入1-5之间的数字: \n");
    } while (1);

    // 输入到付与上门服务类型
    printf("5.请选择服务类型 (0-不需费用, 1-需要到付, 2-需要上门, 3-到付上门): \n");
    do
    {
        printf("> ");
        char temp[MAX_LEN];
        scanf("%s", temp);
        while (getchar() != '\n')
            ; // 清空输入缓冲区
        if (isValidNumber(temp))
        {
            now->ifCollection = atoi(temp); // 将字符串转换为整数
            if (now->ifCollection >= 0 && now->ifCollection <= 3)
            {
                break;
            }
        }
        printf("输入无效，请输入0-3之间的数字: \n");
    } while (1);

    // 输入包裹状态
    printf("6.请输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
    do
    {
        printf("> ");
        char temp[MAX_LEN];
        scanf("%s", temp);
        while (getchar() != '\n')
            ; // 清空输入缓冲区
        if (isValidNumber(temp))
        {
            now->package_status = atoi(temp); // 将字符串转换为整数
            if (now->package_status >= 1 && now->package_status <= 3)
            {
                break;
            }
        }
        printf("输入无效，请输入1-3之间的数字: \n");
    } while (1);

    // 输入寄件时间
    printf("7.请输入包裹寄件时间 (1~366): \n");
    do
    {
        printf("> ");
        char temp[MAX_LEN];
        scanf("%s", temp);
        while (getchar() != '\n')
            ; // 清空输入缓冲区
        if (isValidNumber(temp))
        {
            now->day = atoi(temp); // 将字符串转换为整数
            if (now->day >= 1 && now->day <= 366)
            {
                break;
            }
        }
        printf("输入无效，请输入1~366之间的整数: \n");
    } while (1);

    // 确认信息
    char jdg[22];
    for (;;)
    {
        now->shipping_fee = calculate_send_package_fees(now, phone_number);
        printf("\n包裹信息展示如下: \n\n");
        printf("1. 收件人姓名: %s\n", now->receiver_name);
        printf("2. 收件人地址: %s\n", now->receiver_address);
        printf("3. 包裹体积: %.2lf cm³\n", now->volume);
        printf("4. 包裹类型: %s\n", pkgType[now->package_type - 1]);
        printf("5. 服务类型: %s\n", ifcollection[now->ifCollection]);
        printf("6. 包裹状态: %s\n", pkgStatus[now->package_status - 1]);
        printf("7. 寄件时间: %d\n", now->day);
        printf("8. 运费: %.2lf 元\n", now->shipping_fee);
        printf("\n请仔细核对包裹信息录入是否正确 Y/N\n");
        for (int i = 0;;)
        {
            scanf("%c", &jdg[i]);
            if (i == 0 && jdg[i] == '\n')
            {
                continue;
            }
            if (jdg[i] == '\n' && i <= 20)
            {
                jdg[i] = '\0';
                break;
            }
            if (i == 20)
            {
                for (;;)
                {
                    scanf("%c", &jdg[i]);
                    if (jdg[i] == '\n')
                    {
                        jdg[i] = '\0';
                        break;
                    }
                }
                break;
            }
            i++;
        }

        if (strlen(jdg) == 1 && (*jdg) == 'Y')
        {
            decrement_customer_tickets(phone_number); // 消耗一张优惠券
            printf("优惠卷成功使用！\n");
            break;
        }
        else if (strlen(jdg) == 1 && (*jdg) == 'N')
        {
            printf("请输入希望更改项的序号 (输入\"0\"以放弃本次修改):\n");
            int lsl;
            char temp[MAX_LEN];
            do
            {
                printf("> ");
                scanf("%s", temp);
                while (getchar() != '\n'); // 清空输入缓冲区
                if (isValidNumber(temp))
                {
                    lsl = atoi(temp); // 将字符串转换为整数
                    if (lsl >= 0 && lsl <= 7)
                    {
                        break;
                    }
                }
                printf("输入无效，请输入0-7之间的数字: \n");
            } while (1);
            if (lsl == 1)
            {
                printf("请重新输入收件人姓名: \n");
                do
                {
                    printf("> ");
                    scanf("%s", now->receiver_name);
                    while (getchar() != '\n')
                        ; // 清空输入缓冲区
                    if (strlen(now->receiver_name) > 0 && strlen(now->receiver_name) <= 10)
                    {
                        break;
                    }
                    printf("输入无效，请输入不超过10个字符的姓名: \n");
                } while (1);
            }
            if (lsl == 2)
            {
                printf("请重新输入收件人地址: \n");
                do
                {
                    printf("> ");
                    scanf("%s", now->receiver_address);
                    while (getchar() != '\n')
                        ; // 清空输入缓冲区
                    if (strlen(now->receiver_address) > 0 && strlen(now->receiver_address) <= 20)
                    {
                        break;
                    }
                    printf("输入无效，请输入不超过20个字符的地址: \n");
                } while (1);
            }
            if (lsl == 3)
            {
                printf("请重新输入包裹体积 (立方厘米): \n");
                do
                {
                    printf("> ");
                    char temp[MAX_LEN];
                    scanf("%s", temp);
                    while (getchar() != '\n')
                        ; // 清空输入缓冲区
                    if (isValidFloat(temp))
                    {
                        now->volume = atof(temp); // 将字符串转换为浮点数
                        if (now->volume > 0 && now->volume <= 1000000)
                        {
                            break;
                        }
                    }
                    printf("输入无效，请输入一个正数 (1-1000000): \n");
                } while (1);
            }
            if (lsl == 4)
            {
                printf("请重新输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
                do
                {
                    printf("> ");
                    char temp[MAX_LEN];
                    scanf("%s", temp);
                    while (getchar() != '\n')
                        ; // 清空输入缓冲区
                    if (isValidNumber(temp))
                    {
                        now->package_type = atoi(temp); // 将字符串转换为整数
                        if (now->package_type >= 1 && now->package_type <= 5)
                        {
                            break;
                        }
                    }
                    printf("输入无效，请输入1-5之间的数字: \n");
                } while (1);
            }
            if (lsl == 5)
            {
                printf("请重新输入服务类型 (0-不需费用, 1-需要到付, 2-需要上门, 3-到付上门): \n");
                do
                {
                    printf("> ");
                    char temp[MAX_LEN];
                    scanf("%s", temp);
                    while (getchar() != '\n')
                        ; // 清空输入缓冲区
                    if (isValidNumber(temp))
                    {
                        now->ifCollection = atoi(temp); // 将字符串转换为整数
                        if (now->ifCollection >= 0 && now->ifCollection <= 3)
                        {
                            break;
                        }
                    }
                    printf("输入无效，请输入0-3之间的数字: \n");
                } while (1);
            }
            if (lsl == 6)
            {
                printf("请重新输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
                do
                {
                    printf("> ");
                    char temp[MAX_LEN];
                    scanf("%s", temp);
                    while (getchar() != '\n')
                        ; // 清空输入缓冲区
                    if (isValidNumber(temp))
                    {
                        now->package_status = atoi(temp); // 将字符串转换为整数
                        if (now->package_status >= 1 && now->package_status <= 3)
                        {
                            break;
                        }
                    }
                    printf("输入无效，请输入1-3之间的数字: \n");
                } while (1);
            }
            if (lsl == 7)
            {
                printf("请重新输入寄件时间 (1~366): \n");
                do
                {
                    printf("> ");
                    char temp[MAX_LEN];
                    scanf("%s", temp);
                    while (getchar() != '\n')
                        ; // 清空输入缓冲区
                    if (isValidNumber(temp))
                    {
                        now->day = atoi(temp); // 将字符串转换为整数
                        if (now->day >= 1 && now->day <= 366)
                        {
                            break;
                        }
                    }
                    printf("输入无效，请输入1~366之间的整数: \n");
                } while (1);
            }
        }
        else
        {
            printf("输入\"Y\"以表示确认, \"N\"以表示需要修改包裹信息.\n");
        }
    }

   
    now->next = head->next;
    head->next = now;

    printf("寄件包裹添加成功！\n");
    save_package_s(head); // 保存包裹信息
}

void query_and_show_packages(struct package_s *head, const char *phone_number) {
    if (head == NULL) {
        printf("当前没有任何包裹信息。\n");
        return;
    }

    struct package_s *current = head->next; // 跳过头节点
    int total_count = 0;

    // 计算符合条件的包裹总数
    struct package_s *temp = current;
    while (temp != NULL) {
        if (strcmp(temp->phone_number, "1") == 0)
            break;
        if (phone_number == NULL || strcmp(temp->phone_number, phone_number) == 0) {
            total_count++;
        }
        temp = temp->next;
    }

    if (total_count == 0) {
        printf("没有找到符合条件的包裹。\n");
        return;
    }

    const int PAGE_SIZE = 15; // 每页最多显示 15 个包裹
    int total_pages = (total_count + PAGE_SIZE - 1) / PAGE_SIZE; // 计算总页数
    int current_page = 1;

    while (1) {
        system("cls"); // 清屏
        printf("\n========================== 寄件包裹信息 (第 %d/%d 页) ===========================\n", current_page, total_pages);
        printf("%-5s %-15s       %-12s       %-20s %-12s %-10s %-10s %-10s %-7s %-5s\n",
               "序号", "发件电话", "收件人姓名", "收件地址", "体积(cm³)", "类型", "服务类型", "状态", "费用(元)", "寄件时间");
        printf("----------------------------------------------------------------------------------------------------\n");

        // 定位到当前页的起始包裹
        temp = current;
        int start_index = (current_page - 1) * PAGE_SIZE;
        int end_index = start_index + PAGE_SIZE;
        int count = 0;

        while (temp != NULL) {
            if (strcmp(temp->phone_number, "1") == 0)
                break;
            if (phone_number == NULL || strcmp(temp->phone_number, phone_number) == 0) {
                if (count >= start_index && count < end_index) {
                    printf("%-5d %-15s %-12s %-20s %-12.2lf %-10s %-10s %-10s %-7.2lf 第%d天\n",
                           count + 1,
                           temp->phone_number,
                           temp->receiver_name,
                           temp->receiver_address,
                           temp->volume,
                           pkgType[temp->package_type - 1],     // 包裹类型中文
                           ifcollection[temp->ifCollection],    // 服务类型中文
                           pkgStatus[temp->package_status - 1], // 包裹状态中文
                           temp->shipping_fee,
                           temp->day);
                }
                count++;
                if (count >= end_index) {
                    break;
                }
            }
            temp = temp->next;
        }

        printf("----------------------------------------------------------------------------------------------------\n");
        printf("操作提示: N-下一页, B-上一页, Q-退出\n");
        printf("请输入操作: ");

        char choice;
        scanf(" %c", &choice);
        choice = toupper(choice); // 转换为大写

        if (choice == 'N') {
            if (current_page < total_pages) {
                current_page++;
            } else {
                printf("已经是最后一页！\n");
                system("pause");
            }
        } else if (choice == 'B') {
            if (current_page > 1) {
                current_page--;
            } else {
                printf("已经是第一页！\n");
                system("pause");
            }
        } else if (choice == 'Q') {
            printf("退出包裹信息显示。\n");
            break;
        } else {
            printf("无效的操作，请重新输入。\n");
            system("pause");
        }
    }
}

void delete_package_s(struct package_s *head, const char *phone_number)
{
    if (head == NULL)
    {
        printf("当前没有任何包裹信息。\n");
        return;
    }

    struct package_s *prev = head, *current = head->next;
    int count = 0;

    printf("\n========================== 删除包裹信息 ===========================\n");
    while (current != NULL)
    {
        // 如果是管理员，显示所有包裹；如果是普通用户，只显示与其电话号码匹配的包裹
        if (phone_number == NULL || strcmp(current->phone_number, phone_number) == 0)
        {
            count++;
            printf("序号: %-5d 发件电话：%-11s 收件人姓名: %-12s\t", count, current->phone_number, current->receiver_name);
            printf("收件地址: %-20s\n", current->receiver_address);
            printf("体积(cm³): %-12.2lf 类型: %-10s 服务类型: %-10s 状态: %-10s 费用(元): %-7.2lf 寄件时间: %-5d\n\n",
                   current->volume,
                   pkgType[current->package_type - 1],     // 包裹类型中文
                   ifcollection[current->ifCollection],    // 服务类型中文
                   pkgStatus[current->package_status - 1], // 包裹状态中文
                   current->shipping_fee,
                   current->day);
        }
        current = current->next;
    }

    if (count == 0)
    {
        printf("没有找到符合条件的包裹。\n");
    }
    else
    {
        printf("---------------------------------------------------------------------\n");
        printf("总计 %d 个寄件包裹。\n", count);
    }

    printf("请输入要删除的包裹序号: ");
    int delete_index;
    while (scanf("%d", &delete_index) != 1 || delete_index < 1 || delete_index > count)
    {
        printf("输入无效，请输入有效的序号: ");
        while (getchar() != '\n')
            ; // 清空输入缓冲区
    }

    // 确认是否删除
    char confirm;
    printf("是否确认删除包裹序号 %d？(Y/N): ", delete_index);
    while (1)
    {
        scanf(" %c", &confirm);
        if (confirm == 'Y' || confirm == 'y')
        {
            break; // 确认删除
        }
        else if (confirm == 'N' || confirm == 'n')
        {
            printf("取消删除操作。\n");
            return;
        }
        else
        {
            printf("输入无效，请输入 Y 或 N: ");
        }
    }

    prev = head;
    current = head->next;
    count = 0;
    while (current != NULL)
    {
        if (phone_number == NULL || strcmp(current->phone_number, phone_number) == 0)
        {
            count++;
            if (count == delete_index)
            {
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

struct package_s *load_package_s()
{
    FILE *file = fopen(SEND_FILE, "r");
    if (!file)
    {
        perror("No package file found");
        return NULL;
    }

    struct package_s *head = (struct package_s *)malloc(sizeof(struct package_s));
    head->next = NULL;
    struct package_s *lst = head;

    while (1)
    {
        struct package_s *now = (struct package_s *)malloc(sizeof(struct package_s));
        if (fscanf(file, "%s %s %s %lf %d %d %d %lf %d",
                   now->phone_number,
                   now->receiver_name,
                   now->receiver_address,
                   &now->volume,
                   &now->package_type,
                   &now->ifCollection,
                   &now->package_status,
                   &now->shipping_fee,
                   &now->day) != 9)
        {
            free(now);
            break;
        }

        // 校验数据合法性
        if (strlen(now->phone_number) != 11 || now->volume <= 0 ||
            now->package_type < 1 || now->package_type > 5 ||
            now->ifCollection < 0 || now->ifCollection > 3 ||
            now->package_status < 1 || now->package_status > 3 ||
            now->day < 1 || now->day > 366)
        {
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

void save_package_s(struct package_s *head)
{
    FILE *file = fopen(SEND_FILE, "w");
    if (!file)
    {
        perror("Failed to open file for saving packages");
        return;
    }

    struct package_s *lst = head->next; // 跳过头节点
    while (lst != NULL)
    {
        if (strlen(lst->phone_number) == 0 || strlen(lst->receiver_name) == 0 || lst->volume <= 0)
        {
            lst = lst->next;
            continue; // 跳过无效节点
        }

        fprintf(file, "%s %s %s %.2lf %d %d %d %.2lf %d\n",
                lst->phone_number,
                lst->receiver_name,
                lst->receiver_address,
                lst->volume,
                lst->package_type,
                lst->ifCollection,
                lst->package_status,
                lst->shipping_fee,
                lst->day);
        lst = lst->next;
    }

    fclose(file);
}

void free_package_s(struct package_s *head)
{
    struct package_s *lst, *now;
    lst = head;
    for (;;)
    {
        now = (*lst).next;
        free(lst);
        if (now == NULL)
        {
            break;
        }
        lst = now;
    }
    return;
}

void package_s_original_start(char *t)
{
    struct package_s *head = load_package_s();
    char nm[22]; // nm是电话号码
    for (int i = 0; i < 11; i++)
    {
        nm[i] = t[i];
    }
    nm[11] = '\0';
    int op;
    for (;;)
    {
        printf("\n========== 寄件包裹管理系统 ==========\n");
        printf("1. 添加寄件包裹\n");
        printf("2. 查询寄件包裹\n");
        printf("3. 删除寄件包裹\n");
        printf("0. 返回上一级\n");
        printf("=====================================\n");
        printf("请输入您的选择: ");
        while (scanf("%d", &op) != 1 || op < 0 || op > 3)
        {
            printf("输入无效，请输入0-3之间的数字: ");
            while (getchar() != '\n')
                ; // 清空输入缓冲区
        }
        if (op == 0)
        {
            free_package_s(head);
            break;
        }
        if (op == 1)
        { // 添加寄件包裹
            add_package_s(head, nm);
        }
        if (op == 2)
        { // 查询寄件包裹
            if (strcmp(nm, adminPhoneNumber) == 0)
            {
                query_and_show_packages(head, NULL); // 管理员查询所有包裹
            }
            else
            {
                query_and_show_packages(head, nm); // 普通用户查询自己的包裹
            }
        }
        if (op == 3)
        { // 删除寄件包裹
            if (strcmp(nm, adminPhoneNumber) == 0)
            {
                delete_package_s(head, NULL); // 管理员删除任意包裹
            }
            else
            {
                delete_package_s(head, nm); // 普通用户只能删除自己的包裹
            }
            save_package_s(head); // 隐式保存
        }
    }
    return;
}
