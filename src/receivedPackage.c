#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receivedPackage.h"
#include "structure.h"
#include "user.h"

int count_day[400] = {0}; // 预计存储365个数据
//每录入一个包裹，就重写huise.txt文件，第i+1行的内容为第i天到达驿站的包裹数
// 从文件 "huise.txt" 中读取数据并初始化(修改) count_day 数组
void load_data() {
    FILE* file = fopen("huise.txt", "r");
    if (file == NULL) {
        printf("无法打开文件 huise.txt\n");
        return;
    }
    for (int i = 0; i < 400; ++i) {
        count_day[i] = 0; // 初始化数组
    }
    //读取文件内容到 count_day 数组直到 EOF
    for (int i = 0; !feof(file) && fscanf(file, "%d", &count_day[i]) == 1; ++i);
    fclose(file);
}

void save_data() {
    FILE* file = fopen("huise.txt", "w");
    if (file == NULL) {
        printf("无法保存文件 huise.txt\n");
        return;
    }
    for (int i = 0; i <= 366; i++) {
        fprintf(file, "%d\n", count_day[i]);
    }
    fclose(file);
    // printf("数据已保存到文件: %s\n", filename);
}


void free_package_r(struct package_r* head) {
    struct package_r *lst, *now;
    lst = head;
    for (;;) {
        now = (*lst).next;
        free(lst);
        if (now == NULL) {
            break;
        }
        lst = now;
    }
    return;
}

void get_package_r_id(struct package_r* x) {
    struct node {
        int id;
        struct node *next;
    };
    FILE *file = fopen("id_box.txt", "r");
    if (!file) {
        perror("ID_File is Lost. Please call the manager.");
        return;
    }

    int tmp, dmp;
    fscanf(file, "%d", &tmp);

    dmp = tmp / 10000 + 1;

    if ((*x).volume > 15000) {
        (*x).package_id[0] = 'C';
    } else {
        if ((*x).volume > 6000) {
            (*x).package_id[0] = 'B';
        } else {
            (*x).package_id[0] = 'A';
        }
    }
    (*x).package_id[1] = dmp / 10 + '0';
    (*x).package_id[2] = dmp % 10 + '0';
    (*x).package_id[3] = '-';
    (*x).package_id[7] = tmp % 10 + '0';
    tmp /= 10;
    (*x).package_id[6] = tmp % 10 + '0';
    tmp /= 10;
    (*x).package_id[5] = tmp % 10 + '0';
    tmp /= 10;
    (*x).package_id[4] = tmp % 10 + '0';
    (*x).package_id[8] = '\0';

    struct node *start = (struct node*)malloc(sizeof(struct node));
    struct node *now, *nxt;
    now = start;
    (*start).next = NULL;
    while (!feof(file)) {
        nxt = (struct node*)malloc(sizeof(struct node));
        (*now).next = nxt;
        fscanf(file, "%d", &(*nxt).id);
        (*nxt).next = NULL;
        now = nxt;
    }
    fclose(file);
    now = (*start).next;
    file = fopen("id_box.txt", "w");
    while (now != NULL) {
        fprintf(file, " %d", (*now).id);
        now = (*now).next;
    }
    fclose(file);
    now = start;
    nxt = (*now).next;
    while (now == NULL) {
        free(now);
        now = nxt;
        nxt = (*now).next;
    }
    return;
}

void return_package_r_id(struct package_r *x) {
    FILE *file = fopen("id_box.txt", "a");

    int tmp;
    tmp = (*x).package_id[1] - '0';
    tmp *= 10;
    tmp += (*x).package_id[2] - '0';
    tmp -= 1;
    tmp *= 10;
    tmp += (*x).package_id[7] - '0';
    tmp *= 10;
    tmp += (*x).package_id[6] - '0';
    tmp *= 10;
    tmp += (*x).package_id[5] - '0';
    tmp *= 10;
    tmp += (*x).package_id[4] - '0';

    fprintf(file, " %d", tmp);
    fclose(file);
    return;
}

void show_package_r(struct package_r* now, int if_id) {
    printf("1.包裹所属用户电话: \n");
    printf("%s\n", (*now).phone_number);
    printf("2.包裹体积 (立方厘米): \n");
    printf("%lf\n", (*now).volume);
    printf("3.包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
    printf("%d\n", (*now).package_type);
    printf("4.是否需要到付 (0-不需要, 1-需要): \n");
    printf("%d\n", (*now).ifCollection);
    if ((*now).ifCollection) {
        printf("运输费用: \n");
        printf("%lf\n", (*now).shipping_fee);
    }
    printf("5.包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
    printf("%d\n", (*now).package_status);

    if (if_id == 1) {
        printf("6.包裹序列号: \n");
        printf("%s\n", (*now).package_id);
    }
    printf("7.包裹到达时间: \n");
    printf("%d\n", (*now).day);
    return;
}

void find_package_r(struct package_r **lstfind, struct package_r **find, char* p, struct package_r* head) {
    struct package_r *lst, *now;
    lst = head;

    for (;;) {
        now = (*lst).next;
        if (now == NULL) {
            (*lstfind) = NULL;
            (*find) = NULL;
            break;
        }
        if (strcmp((*now).package_id, p) == 0) {
            (*lstfind) = lst;
            (*find) = now;
            break;
        }
        lst = now;
    }

    return;
}

struct package_r* load_package_r() {
    FILE *file = fopen("received_packages.txt", "r");
    if (!file) {
        perror("No package file found");
        return NULL;
    }

    struct package_r *head, *lst;
    head = (struct package_r*)malloc(sizeof(struct package_r));
    (*head).next = NULL;
    lst = head;

    while (!feof(file)) {
        struct package_r* now = (struct package_r*)malloc(sizeof(struct package_r));
        fscanf(file, "%s%lf%d%d", (*now).phone_number, &(*now).volume, &(*now).package_type, &(*now).ifCollection);
        fscanf(file, "%lf", &(*now).shipping_fee);
        fscanf(file, "%d", &(*now).package_status);
        fscanf(file, "%s", (*now).package_id);
        fscanf(file, "%d", &(*now).day);

        (*lst).next = now;
        lst = now;
        (*lst).next = NULL;
    }

    fclose(file);

    return head;
}

void save_package_r(struct package_r* head) {
    FILE *file = fopen("received_packages.txt", "w");
    if (!file) {
        perror("Failed to open file for saving packages");
        return;
    }

    struct package_r *lst, *now;
    lst = head;

    for (;;) {
        now = (*lst).next;
        if (now == NULL) {
            break;
        }
        fprintf(file, "%s %lf %d %d", (*now).phone_number, (*now).volume, (*now).package_type, (*now).ifCollection);
        if ((*now).ifCollection == 1) {
            fprintf(file, " %lf", (*now).shipping_fee);
        } else {
            fprintf(file, " 0");
        }
        fprintf(file, " %d", (*now).package_status);
        fprintf(file, " %s", (*now).package_id);
        fprintf(file, " %d", (*now).day);
        if ((*now).package_id[0] == 'S') {
            break;
        }
        fprintf(file, "\n");

        lst = now;
    }

    fclose(file);
}

void add_received_package(struct package_r *head) {
    struct package_r* now = (struct package_r*)malloc(sizeof(struct package_r));

    // 输入手机号并验证合法性
    printf("1.请输入包裹所属用户电话 (11位数字): \n");
    do {
        printf("> ");
        scanf("%s", now->phone_number);
        if (!isPhoneNumberValid(now->phone_number)) {
            printf("电话号码格式错误，必须为11位数字！请重新输入。\n");
        } else {
            break;
        }
    } while (1);

    // 输入包裹体积并验证合法性
    printf("2.请输入包裹体积 (立方厘米): \n");
    do {
        char buffer[50];
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%lf", &now->volume) == 1 && now->volume > 0) {
            break;
        }
        printf("输入无效，请输入一个正数: \n");
    } while (1);

    // 输入包裹类型并验证合法性
    printf("3.请输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
    do {
        char buffer[50];
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &now->package_type) == 1 && now->package_type >= 1 && now->package_type <= 5) {
            break;
        }
        printf("输入无效，请输入1-5之间的数字: \n");
    } while (1);

    // 输入是否到付并验证合法性
    printf("4.是否需要到付 (0-不需要, 1-需要): \n");
    do {
        char buffer[50];
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &now->ifCollection) == 1 && (now->ifCollection == 0 || now->ifCollection == 1)) {
            break;
        }
        printf("输入无效，请输入0或1: \n");
    } while (1);

    // 如果需要到付，输入运费并验证合法性
    if (now->ifCollection) {
        printf("请输入运输费用 (非负数): \n");
        do {
            char buffer[50];
            printf("> ");
            fgets(buffer, sizeof(buffer), stdin);
            if (sscanf(buffer, "%lf", &now->shipping_fee) == 1 && now->shipping_fee >= 0) {
                break;
            }
            printf("输入无效，请输入一个非负数: \n");
        } while (1);
    } else {
        now->shipping_fee = 0.0; // 如果不需要到付，运费为0
    }

    // 输入包裹状态并验证合法性
    printf("5.请输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
    do {
        char buffer[50];
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &now->package_status) == 1 && now->package_status >= 1 && now->package_status <= 3) {
            break;
        }
        printf("输入无效，请输入1-3之间的数字: \n");
    } while (1);

    // 输入包裹到达时间并验证合法性
    printf("6.请重新输入包裹到达时间(1~366): \n");
    while (scanf("%d", &(*now).day) != 1 || (*now).day < 0 || (*now).day > 366) {
        printf("输入无效，请输入一个1~366的整数数: ");
        while (getchar() != '\n'); // 清空输入缓冲区
    }
    char jdg[22];
    for (;;) {
        printf("\n包裹信息展示如下: \n\n");
        show_package_r(now, 0);
        printf("\n请仔细核对包裹信息录入是否正确 Y/N\n");
        for (int i = 0;;) {
            scanf("%c", &jdg[i]);
            if (i == 0 && jdg[i] == '\n') {
                continue;
            }
            if (jdg[i] == '\n' && i <= 20) {
                jdg[i] = '\0';
                break;
            }
            if (i == 20) {
                for (;;) {
                    scanf("%c", &jdg[i]);
                    if (jdg[i] == '\n') {
                        jdg[i] = '\0';
                        break;
                    }
                }
                break;
            }
            i++;
        }

        if (strlen(jdg) == 1 && (*jdg) == 'Y') {
            break;
        } else {
            if (strlen(jdg) == 1 && (*jdg) == 'N') {
                printf("请输入希望更改项的序号 (输入\"0\"以放弃本次修改):\n");
                int lsl;
                while (scanf("%d", &lsl) != 1 || lsl < 0 || lsl > 6) {
                    printf("输入无效，请输入0-6之间的数字: ");
                    while (getchar() != '\n'); // 清空输入缓冲区
                }
                if (lsl == 1) {
                    do {
                        printf("请重新输入包裹所属用户电话: \n");
                        scanf("%s", (*now).phone_number);
                        if (!isPhoneNumberValid((*now).phone_number)) {
                            printf("电话号码格式错误，必须为11位数字！\n");
                        }
                    } while (!isPhoneNumberValid((*now).phone_number));
                }
                if (lsl == 2) {
                    printf("请重新输入包裹体积 (立方厘米): \n");
                    while (scanf("%lf", &(*now).volume) != 1 || (*now).volume <= 0) {
                        printf("输入无效，请输入一个正数: ");
                        while (getchar() != '\n'); // 清空输入缓冲区
                    }
                }
                if (lsl == 3) {
                    printf("请重新输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
                    while (scanf("%d", &(*now).package_type) != 1 || (*now).package_type < 1 || (*now).package_type > 5) {
                        printf("输入无效，请输入1-5之间的数字: ");
                        while (getchar() != '\n'); // 清空输入缓冲区
                    }
                }
                if (lsl == 4) {
                    printf("请重新输入是否需要到付 (0-不需要, 1-需要): \n");
                    while (scanf("%d", &(*now).ifCollection) != 1 || ((*now).ifCollection != 0 && (*now).ifCollection != 1)) {
                        printf("输入无效，请输入0或1: ");
                        while (getchar() != '\n'); // 清空输入缓冲区
                    }
                    if ((*now).ifCollection) {
                        printf("请重新输入运输费用: \n");
                        while (scanf("%lf", &(*now).shipping_fee) != 1 || (*now).shipping_fee < 0) {
                            printf("输入无效，请输入一个非负数: ");
                            while (getchar() != '\n'); // 清空输入缓冲区
                        }
                    }
                }
                if (lsl == 5) {
                    printf("请重新输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
                    while (scanf("%d", &(*now).package_status) != 1 || (*now).package_status < 1 || (*now).package_status > 3) {
                        printf("输入无效，请输入1-3之间的数字: ");
                        while (getchar() != '\n'); // 清空输入缓冲区
                    }
                }
                if (lsl == 6)
                {
                    printf("请重新输入包裹到达时间: \n");
                    while (scanf("%d", &(*now).day) != 1 || (*now).day < 0 || (*now).day > 366) {
                        printf("输入无效，请输入一个1~366的整数数: ");
                        while (getchar() != '\n'); // 清空输入缓冲区
                    }
                }
                
            } else {
                printf("输入\"Y\"以表示确认, \"N\"以表示需要修改包裹信息.\n");
            }
        }
    }

    (*now).next = (*head).next;
    (*head).next = now;

    // 自动分配包裹序列号
    get_package_r_id(now);
    load_data();
    count_day[(*now).day]++;
    save_data();
    printf("收件包裹添加成功！\n");
    printf("已自动为您分配包裹序列号：%s\n", now->package_id);

    // 保存包裹信息
    save_package_r(head);
}

void query_received_package(struct package_r *head) {
    printf("请输入希望查询的包裹序列号: \n");
    char p[22];
    for (int i = 0;;) {
        scanf("%c", &p[i]);
        if (i == 0 && p[i] == '\n') {
            continue;
        }
        if (p[i] == '\n' && i <= 20) {
            p[i] = '\0';
            break;
        }
        if (i == 20) {
            for (;;) {
                scanf("%c", &p[i]);
                if (p[i] == '\n') {
                    p[i] = '\0';
                    break;
                }
            }
            break;
        }
        i++;
    }

    struct package_r *lstfin, *fin;
    find_package_r(&lstfin, &fin, p, head);

    if (fin == NULL) {
        printf("未查询到相应包裹, 请检查序列号拼写是否正确.\n");
        return;
    }
    printf("\n查询到包裹信息如下:\n\n");

    show_package_r(fin, 1);
}

void delete_received_package(struct package_r *head) {
    show_all_packages_r(head);
    printf("请注意, 删除包裹后将无法恢复.\n");
    printf("请输入希望删除的包裹序列号: \n");
    char p[22];
    for (int i = 0;;) {
        scanf("%c", &p[i]);
        if (i == 0 && p[i] == '\n') {
            continue;
        }
        if (p[i] == '\n' && i <= 20) {
            p[i] = '\0';
            break;
        }
        if (i == 20) {
            for (;;) {
                scanf("%c", &p[i]);
                if (p[i] == '\n') {
                    p[i] = '\0';
                    break;
                }
            }
            break;
        }
        i++;
    }

    struct package_r *lstfin, *fin;
    find_package_r(&lstfin, &fin, p, head);

    if (fin == NULL) {
        printf("未查询到相应包裹, 请检查序列号拼写是否正确.\n");
        return;
    }

    printf("请慎重选择, 确认删除此包裹吗? Y/N\n");

    for (;;) {
        for (int i = 0;;) {
            scanf("%c", &p[i]);
            if (i == 0 && p[i] == '\n') {
                continue;
            }
            if (p[i] == '\n' && i <= 20) {
                p[i] = '\0';
                break;
            }
            if (i == 20) {
                for (;;) {
                    scanf("%c", &p[i]);
                    if (p[i] == '\n') {
                        p[i] = '\0';
                        break;
                    }
                }
                break;
            }
            i++;
        }

        if (strlen(p) == 1 && (*p) == 'Y') {
            return_package_r_id(fin);
            (*lstfin).next = (*fin).next;
            free(fin);
            printf("包裹已删除.\n");
            save_package_r(head);
            break;
        } else {
            if (strlen(p) == 1 && (*p) == 'N') {
                printf("操作已取消.\n");
                break;
            } else {
                printf("输入\"Y\"以表示确认删除此包裹, \"N\"以取消删除操作.\n");
            }
        }
    }
    save_package_r(head);
}

void show_all_packages_r(struct package_r* head) {
    if (head == NULL || head->next == NULL) {
        printf("当前没有收件包裹记录。\n");
        return;
    }

    struct package_r* current = head->next; // 跳过头节点
    int count = 0;

    printf("\n======================= 所有收件包裹信息 ============================\n");
    printf("%-5s %-18s %-15s %-10s %-10s %-10s %-15s %d\n", 
           "序号", "用户电话", "体积(cm³)", "类型", "到付", "状态", "序列号", "到达时间");
    printf("---------------------------------------------------------------------\n");

    while (current != NULL) {
        if (strcmp(current->phone_number, "1") == 0) break;
        count++;
        printf("%-5d %-15s %-9.2lf %-8s %-14s %-10s %-15s %d\n", 
               count, 
               current->phone_number, 
               current->volume, 
               pkgType[current->package_type - 1], // 显示包裹类型的中文描述
               ifcollection[current->ifCollection], // 显示是否到付的中文描述
               pkgStatus[current->package_status - 1], // 显示包裹状态的中文描述
               current->package_id);
        current = current->next;
    }

    printf("---------------------------------------------------------------------\n");
    printf("总计 %d 个收件包裹。\n", count);
}

// 收件管理的主函数
void package_r_original_start() {
    struct package_r *head = load_package_r();
    int op;

    for (;;) {
        printf("\n========== 收件包裹管理系统 ==========\n");
        printf("1. 添加收件包裹\n");
        printf("2. 查询指定收件包裹\n");
        printf("3. 删除指定包裹\n");
        printf("4. 显示所有包裹\n");
        printf("0. 返回主菜单\n");
        printf("=====================================\n");
        printf("请输入您的选择: ");

        while (scanf("%d", &op) != 1 || op < 0 || op > 5) {
            printf("输入无效，请输入0-5之间的数字: ");
            while (getchar() != '\n'); // 清空输入缓冲区
        }

        if (op == 0) {
            free_package_r(head);
            break;
        }

        if (op == 1) {
            add_received_package(head);
        }

        if (op == 2) {
            query_received_package(head);
        }

        if (op == 3) {
            delete_received_package(head);
        }

        if (op == 4) {
            show_all_packages_r(head);
        }
    }
}
