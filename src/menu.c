#include "menu.h"
#include <stdio.h>

void showMenu_main(){
    //程序主页面
    printf("*************************\n");
    printf("***** 请选择用户类型 *****\n");
    printf("***** 1. 管理人员   *****\n");
    printf("***** 2. 用户       *****\n");
    printf("***** 3. 退出       *****\n");
    printf("*************************\n");
}

void showMenu_userLanding(){
    //用户登录页面
    printf("*************************\n");
    printf("***** 请选择操作类型 *****\n");
    printf("***** 1. 登录       *****\n");
    printf("***** 2. 退出       *****\n");
    printf("*************************\n");
}

void showMenu_uerOperating(){
    //用户操作页面
    printf("*************************\n");
    printf("***** 请选择操作类型 *****\n");
    printf("***** 1. 寄件       *****\n");
    printf("***** 2. 收件       *****\n");
    printf("***** 3. 退出       *****\n");
    printf("*************************\n");
}

void showMenu_adminOperating(){
    //管理员操作页面
    printf("*************************\n");
    printf("***** 请选择操作类型  *****\n");
    printf("***** 1. 寄件包裹管理 *****\n");
    printf("***** 2. 收件包裹管理 *****\n");
    printf("***** 3. 用户管理     *****\n");
    printf("***** 4. 退出        *****\n");
    printf("*************************\n");
}