#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "fileOperations.h"
#include "menu.h"
#include "receivedPackage.h"
#include "sendPackage.h"
#include "structure.h"
#include "user.h"
#include "admin.h"
#include "huise.h"
#include "warehouse_explosion_warning.h"
#define adminPhoneNumber "20245524116" //增加管理员测试用的号码

int main() {
    struct customer *customerList = NULL; // 初始化客户列表
    char command[200];

    int choice;
    do {
        displayMainMenu();

        // 验证输入是否为有效整数
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入数字。\n");
            // 清理输入缓冲区
            while (getchar() != '\n');
            choice = -1; // 设置为无效值以继续循环
        }

        switch (choice) {
            case 1:{ // 用户操作
                while (getchar() != '\n'); // 清理输入缓冲区
                customerList = loadCustomers(); // 从文件加载用户数据
                if (customerList == NULL) {
                    printf("加载用户数据失败，系统将退出。\n");
                    return 1; // 退出程序
                }
                userOperating(customerList);
                break;
            }
            case 2:{ // 管理员操作
                //管理员登录
                if (!adminLogin()) {
                    printf("管理员登录失败，返回主菜单。\n");
                    break;
                }
            
                int adminChoice;
                do{
                    lastpage:

                    displayAdminMenu();

                    // 验证输入是否为有效整数
                    if (scanf("%d", &adminChoice) != 1) {
                        printf("输入无效，请输入数字。\n");
                        // 清理输入缓冲区
                        while (getchar() != '\n');
                        continue;
                    }

                    switch(adminChoice){
                        case 1:{ //用户管理
                            customerList = customersOperation(customerList);
                            break;
                        }
                        case 2:{ //收取包裹管理
                            package_r_original_start();
                            break;
                        }
                        case 3:{ //邮寄包裹管理
                            package_s_original_start(adminPhoneNumber);
                            break;
                        }
                        case 4:{ // 数据预测与统计
                            int real_time_left = 0;
                            int real_time_right = 0;// 真实时间范围
                            printf("请输入起始时间天数1~365\n");
                            if (scanf("%d", &real_time_left) != 1) { // 验证输入是否为有效整数
                                printf("输入无效,请重新输入。\n");
                                while (getchar() != '\n'); // 清理输入缓冲区
                                break;
                            }
                            else if(real_time_left < 1 || real_time_left > 365){
                                printf("输入时间超出范围,请重新输入。\n");
                                while (getchar() != '\n'); // 清理输入缓冲区
                                break;
                            }
                            printf("请输入结束时间天数2~366\n");
                            if (scanf("%d", &real_time_right) != 1) { // 验证输入是否为有效整数
                                printf("输入无效,请重新输入。\n");
                                while (getchar() != '\n'); // 清理输入缓冲区
                                break;
                            }
                            else if(real_time_right < 2 || real_time_right > 366){
                                printf("输入时间超出范围,请重新输入。\n");
                                while (getchar() != '\n'); // 清理输入缓冲区
                                break;
                            }
                            else if(real_time_left >= real_time_right){
                                printf("输入时间范围错误,请重新输入。\n");
                                while (getchar() != '\n'); // 清理输入缓冲区
                                break;
                            }

                            int choice_son4;
                            do{
                                displayPredictedModeMenu();
                            
                                // 验证输入是否为有效整数
                                if (scanf("%d", &choice_son4) != 1) {
                                    printf("输入无效，请输入数字。\n");
                                    while (getchar() != '\n');// 清理输入缓冲区
                                    continue;
                                }
                                switch (choice_son4)
                                {
                                case 1:
                                    goto nextpage;
                                case 2:
                                    switch (real_time_right - real_time_left >= 14)
                                    {
                                    case 1:
                                        goto nextpage;
                                        break;
                                    default:
                                        printf("输入时间不足14天(两周),无法进行周预测\n");
                                        break;
                                    }
                                    break;
                                case 3:
                                    switch (real_time_right - real_time_left >= 60)
                                    {
                                    case 1:
                                        goto nextpage;
                                        break;
                                    default:
                                        printf("输入时间不足60天(两个月),无法进行周预测\n");
                                        break;
                                    }
                                    break;
                                case 0:
                                    printf("返回管理员菜单\n");
                                    goto lastpage;
                                    break;
                                default:
                                    printf("无效的选择，请重试。\n");
                                    break;
                                }
                            } while (choice_son4 != 0);
                            nextpage:
                            // 构建命令字符串，将 real_time 作为参数传递给 Python 脚本
                            snprintf(command, sizeof(command),
                                    "start /B C:/Users/25531/AppData/Local/Programs/Python/Python312/python.exe C:/Users/25531/Desktop/programe0/JLU_CourierStation/src/Data_prediction_and_statistics.py %d %d %d >nul 2>nul",
                                     real_time_left,real_time_right,choice_son4);

                            // 执行命令
                            int result = system(command);
                            if (result == -1) {
                                printf("无法启动命令处理器\n");
                            } else {
                                printf("Python脚本已启动,返回值: %d\n", result);
                            }

                            // 提示用户按回车键返回管理员菜单
                            printf("按回车键返回上一级菜单。\n");
                            while (getchar() != '\n'); // 清理缓冲区
                            while (getchar() != '\n');
                            break;
                        }
                        case 5:{ // 删除预测文件
                            int choice_son5;
                            int real_time_left = 0;
                            int real_time_right = 0;// 真实时间范围                            
                            do{
                                displayDeletePredictedFileMenu();
                            
                                // 验证输入是否为有效整数
                                if (scanf("%d", &choice_son5) != 1) {
                                    printf("输入无效，请输入数字。\n");
                                    while (getchar() != '\n');// 清理输入缓冲区
                                    continue;
                                }
                                switch (choice_son5)
                                {
                                case 1:
                                    printf("请输入起始时间天数1~365\n");
                                    if (scanf("%d", &real_time_left) != 1) { // 验证输入是否为有效整数
                                        printf("输入无效,请重新输入。\n");
                                        while (getchar() != '\n'); // 清理输入缓冲区
                                        break;
                                    }
                                    else if(real_time_left < 1 || real_time_left > 365){
                                        printf("输入时间超出范围,请重新输入。\n");
                                        while (getchar() != '\n'); // 清理输入缓冲区
                                        break;
                                    }
                                    printf("请输入结束时间天数2~366\n");
                                    if (scanf("%d", &real_time_right) != 1) { // 验证输入是否为有效整数
                                        printf("输入无效,请重新输入。\n");
                                        while (getchar() != '\n'); // 清理输入缓冲区
                                        break;
                                    }
                                    else if(real_time_right < 2 || real_time_right > 366){
                                        printf("输入时间超出范围,请重新输入。\n");
                                        while (getchar() != '\n'); // 清理输入缓冲区
                                        break;
                                    }
                                    else if(real_time_left >= real_time_right){
                                        printf("输入时间范围错误,请重新输入。\n");
                                        while (getchar() != '\n'); // 清理输入缓冲区
                                        break;
                                    }
                                    delete_predicted_file(real_time_left,real_time_right);
                                    break;
                                case 2:
                                    const char* directory = "."; // 当前目录
                                    delete_all_predicted_files(directory);
                                    break;
                                case 0:
                                    printf("返回管理员菜单\n");
                                    break;
                                default:
                                    printf("无效的选择，请重试。\n");
                                    break;
                                }
                            } while (choice_son5 != 0);
                            break;
                        }
                        case 0:
                            printf("返回管理员菜单\n");
                            break;
                        default:
                            printf("无效的选择，请重试。\n");
                            break;
                    }
                } while (adminChoice != 0);
                break;
            }

            case 0:
                printf("退出系统...\n");
                return 0;
            default:
                printf("无效的选择，请重试。\n");
        }
        system("pause");
        system("cls");
    } while (choice != 0);

    freeCustomers(customerList);

    return 0;
}