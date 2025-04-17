#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "admin.h"
#include "structure.h"

bool adminLogin() {
    char input_name[MAX_LEN];
    char input_password[MAX_LEN];
    char admin_name[MAX_LEN];
    char admin_password[MAX_LEN];

    printf("========== 管理员登录 ==========\n");

    FILE *file = fopen(ADMIN_FILE, "r");
    if (!file) {
        perror("无法打开 admin.txt 文件");
        return false;
    }

    // 限制用户名输入次数
    int username_attempts = 3;
    bool username_found = false;
    while (username_attempts > 0) {
        printf("请输入管理员用户名:(按q返回) \n");
        scanf("%s", input_name);

        // 返回上一级菜单
        if (strcmp(input_name, "q") == 0) {
            printf("已返回上一级菜单。\n");
            fclose(file);
            system("pause");
            system("cls");
            return false;
        }

        // 检查用户名是否存在
        rewind(file); // 重置文件指针到文件开头
        while (fscanf(file, "%s %s", admin_name, admin_password) != EOF) {
            if (strcmp(input_name, admin_name) == 0) {
                username_found = true;
                break;
            }
        }

        if (username_found) {
            break;
        } else {
            username_attempts--;
            if (username_attempts > 0) {
                printf("用户名不存在！您还有 %d 次尝试机会。\n", username_attempts);
            } else {
                printf("用户名不存在！您已达到最大尝试次数，登录失败。\n");
                fclose(file);
                system("pause");
                system("cls");
                return false;
            }
        }
    }

    // 限制密码输入次数
    int password_attempts = 3;
    while (password_attempts > 0) {
        printf("请输入密码 (4-20 个字符):(按q返回) \n");
        scanf("%s", input_password);

        // 返回上一级菜单
        if (strcmp(input_password, "q") == 0) {
            printf("已返回上一级菜单。\n");
            fclose(file);
            system("pause");
            system("cls");
            return false;
        }

        int length = strlen(input_password);
        if (length < 4 || length > 20) {
            printf("密码长度必须在 4 到 20 个字符之间，请重新输入。\n");
            continue;
        }

        // 检查用户名和密码是否匹配
        if (strcmp(input_name, admin_name) == 0 && strcmp(input_password, admin_password) == 0) {
            fclose(file);
            printf("登录成功！欢迎管理员 %s\n", admin_name);
            system("pause");
            system("cls");
            return true;
        }

        // 如果密码错误
        password_attempts--;
        if (password_attempts > 0) {
            printf("密码错误！您还有 %d 次尝试机会。\n", password_attempts);
        } else {
            printf("密码错误！您已达到最大尝试次数，登录失败。\n");
            system("pause");
            system("cls");
        }
    }

    fclose(file);
    return false;
}