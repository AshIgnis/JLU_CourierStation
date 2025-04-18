#ifndef SENDPACKAGE_H
#define SENDPACKAGE_H

#include "structure.h"

// 包裹相关功能声明
void add_package_s(struct package_s *head, const char *phone_number);
void query_and_show_packages(struct package_s *head, const char *phone_number);
// phone_number 为 NULL 时，管理员查询所有包裹；否则查询指定用户的包裹
void delete_package_s(struct package_s* head, const char* phone_number);
void package_s_original_start(char *t);   // 初始化界面
//t 为传入手机号码的字符数组头指针

//File I/O相关功能声明
void save_package_s(struct package_s* head); // 包裹文件保存
struct package_s* load_package_s(); // 包裹文件读取
void free_package_s(struct package_s* head); // 释放包裹链表

#endif // SENDPACKAGE_H_H
