#include "menu.h"

void displayMainMenu() {
    printf("\n========== 主菜单 ==========\n");
    printf("1. 用户界面\n");
    printf("2. 管理员界面\n");
    printf("0. 退出系统\n");
    printf("===========================\n");
    printf("请输入您的选择: ");
}

void displayAdminMenu() {
    printf("\n========== 管理员菜单 ==========\n");
    printf("1. 用户管理\n");
    printf("2. 收取包裹管理\n");
    printf("3. 邮寄包裹管理\n");
    printf("4. 数据预测与统计\n");
    printf("0. 返回主菜单\n");
    printf("==============================\n");
    printf("请输入您的选择: ");
}