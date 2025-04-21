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
    printf("2. 收件包裹管理\n");
    printf("3. 邮寄包裹管理\n");
    printf("4. 数据预测与统计\n");
    printf("5. 删除预测文件(无法删除本次应用启动时模式四预测出来的文件)\n");
    printf("0. 返回主菜单\n");
    printf("==============================\n");
    printf("请输入您的选择: ");
}

void displayPredictedModeMenu() {
    printf("\n========== 预测,统计模式菜单 ==========\n");
    printf("1. 预测下一天的驿站接受包裹数,弹出统计界面\n");
    printf("2. 预测下一周的驿站接受包裹数,弹出统计界面\n");
    printf("3. 预测下个月的驿站接受包裹数,弹出统计界面\n");
    printf("0. 返回管理员菜单\n");
    printf("==============================\n");
    printf("请输入您的选择: ");
}

void displayDeletePredictedFileMenu() {
    printf("\n========== 删除预测文件菜单 ==========\n");
    printf("1. 删除指定的一个预测文件\n");
    printf("2. 删除所有预测文件\n");
    printf("0. 返回管理员菜单\n");
    printf("==============================\n");
    printf("请输入您的选择: ");
}