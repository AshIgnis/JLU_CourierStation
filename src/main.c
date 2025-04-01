#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include "fileOperations.h"
#include "receivedPackage.h"
#include "structure.h"
#include "menu.h"
#include "user.h"

int main() {
    struct customer *customerList = NULL;

    int choice;
    do {
        displayMainMenu();
        printf("请输入您的选择: ");

        // 验证输入是否为有效整数
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，请输入数字。\n");
            // 清理输入缓冲区
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:{
                FILE *file = fopen(CUSTOMER_FILE, "r");
                userOperating(file);
                break;
            }
            case 2:{
                int adminChoice;
                do{
                    displayAdminMenu();
                    printf("请输入您的选择: ");
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
                        }
                        case 2:{ //收取包裹管理
                            package_r_original_start();
                        }
                        case 3:{ //邮寄包裹管理

                        }
                        case 0:
                            printf("返回主菜单\n");
                            break;
                        default:
                            printf("无效的选择，请重试。\n");
                            break;
                    }
                } while (adminChoice != 0);
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