#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "structure.h"

// 用户相关功能声明
struct customer *addCustomer(struct customer *head, const char *username, const char *phone_number, const char *password, int customer_type); // 添加新用户
void queryCustomer(struct customer *head); // 查询用户信息
struct customer *deleteCustomer(struct customer *head); // 删除用户
void freeCustomers(struct customer *head); // 释放用户链表内存
struct customer *customersCreating(struct customer *customerList); // 创建用户
void displayMenu_customer(); // 显示用户菜单
int isPhoneNumberDuplicate(struct customer *head, const char *phone_number); // 检查电话号码是否重复
int get_customer_type(const char *phone_number);

#endif // CUSTOMER_H