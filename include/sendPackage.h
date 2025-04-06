#ifndef SENDPACKAGE_H
#define SENDPACKAGE_H

#include "structure.h"

// 包裹相关功能声明
void show_package_s(struct package_s* x,int if_id); // 展示包裹信息
void package_s_original_start(char* t); // 初始化界面
//t 为传入手机号码的字符数组头指针
void save_package_s(); // 包裹文件保存
struct package_s* load_package_s(); // 包裹文件读取
void find_package_s(char* p,struct package_s* head); // 寻找符合该用户手机号的寄件包裹
//p 为用户手机号码的字符数组头指针

#endif // SENDPACKAGE_H_H
