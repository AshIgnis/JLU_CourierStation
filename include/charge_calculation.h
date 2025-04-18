#ifndef CHARGE_CALCULATION_H
#define CHARGE_CALCULATION_H
#include <stdbool.h>
#include "structure.h"
//用于计算包裹费用的函数
//(1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户):
//(1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品)
//(1-正常, 2-损坏, 3-违禁品)
void initialize_random_seed();
double generate_random_factor();
double get_package_type_coefficient(int package_type);
double get_customer_discount(int customer_type);
double calculate_volume_fee(double volume);
double round_to_two_decimals(double value);
double calculate_send_package_fees(struct package_s *send_pkg, int customer_type,int ifdoortodoor);//计算寄件包裹运费
int get_customer_type(const char *phone_number);
double DoorToDoorFee_r(const char* phone_number,double vol,int pkg_type);
#endif
