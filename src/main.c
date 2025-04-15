#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "fileOperations.h"
#include "menu.h"
#include "receivedPackage.h"
#include "sendPackage.h"
#include "structure.h"
#include "user.h"
#include "warehouse_explosion_warning.h"

int main() {
    struct customer *customerList = NULL; // 初始化客户列表

    int choice;
    do {
        displayMainMenu();

        // 验证输入是否为有效整数
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入数字。\n");
            // 清理输入缓冲区
            while (getchar() != '\n');
            continue;
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
                int adminChoice;
                do{
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
                            customerList = customersCreating(customerList);
                            break;
                        }
                        case 2:{ //收取包裹管理
                            package_r_original_start();
                            break;
                        }
                        case 3:{ //邮寄包裹管理
                            package_s_original_start(NULL);
                            break;
                        }
                        case 4:{ // 数据预测与统计
                            int real_time = 0; // 示例值，可以根据实际情况动态设置
                            printf("请输入真实时间\n");
                            if (scanf("%d", &real_time) != 1) { // 验证输入是否为有效整数
                                printf("输入无效，请输入数字。\n");
                                while (getchar() != '\n'); // 清理输入缓冲区
                                break;
                            }

                            // 构建命令字符串，将 real_time 作为参数传递给 Python 脚本
                            char command[512];
                            snprintf(command, sizeof(command),
                                     "start /B C:/Users/25531/AppData/Local/Programs/Python/Python312/python.exe C:/Users/25531/Desktop/programe/JLU_CourierStation/src/Data_prediction_and_statistics.py %d >nul 2>nul",
                                     real_time);

                            // 执行命令
                            int result = system(command);
                            if (result == -1) {
                                printf("无法启动命令处理器\n");
                            } else {
                                printf("Python脚本已启动,返回值: %d\n", result);
                            }

                            // 提示用户按回车键返回管理员菜单
                            printf("按回车键返回管理员菜单。\n");
                            while (getchar() != '\n'); // 清理缓冲区
                            while (getchar() != '\n');
                            break;
                        }
                        case 0:
                            printf("返回主菜单\n");
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
                break;
            default:
                printf("无效的选择，请重试。\n");
        }
        system("pause");
        system("cls");
    } while (choice != 0);

    freeCustomers(customerList);

    return 0;
}