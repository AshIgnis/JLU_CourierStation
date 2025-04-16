#ifndef RECEIVEDPACKAGE_H
#define RECEIVEDPACKAGE_H

#include "structure.h"

// 包裹相关功能声明
void show_package_r(struct package_r* x,int if_id); // 展示包裹信息
void package_r_original_start(); // 初始化界面
void save_package_r(); // 包裹文件保存
struct package_r* load_package_r(); // 包裹文件读取
void get_package_r_id(struct package_r* x); // 获取取件码
//参数 x 为包裹所对应的指针
void return_package_r_id(struct package_r* x); // 返回取件码
//参数 x 为包裹所对应的指针
void find_package_r(struct package_r **lstfin,struct package_r **fin,char* x,struct package_r* head); // 寻找对应序列号包裹
//参数 lstfin 为查询包裹的上个包裹对应的指针
//fin 为查询包裹对应的指针
//x 为查询包裹对应 id 字符串的首指针
//head 为包裹链表的 head 指针
//使用传参示例 find_package_r(&p1,&p2,cp,head);
void show_all_packages_r(struct package_r* head);//展示所有包裹
#endif // RECEIVEDPACKAGE_H
