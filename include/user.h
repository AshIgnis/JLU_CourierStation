#ifndef USER_H
#define USER_H
#include <stdbool.h>
#include "structure.h"
#include <ctype.h>

bool isPhoneNumberValid(const char *phone_number);
void userOperating(struct customer *head); // 用户操作集成
void userReceivedPackagesSearching(const char *phone_number); // 用户收件包裹查询
void userAction(struct customer *head,const char *phone_number); // 用户登录后续操作
void displayMenu_user(); // 用户菜单显示
bool userLanding(struct customer *head,const char *phone_number, const char *password); // 用户登录判断
void userTakePackage(const char *phone_number); // 用户取件函数
int convertStringToInt(const char *input); // 取件码归还函数
void clearInputBuffer();

#endif