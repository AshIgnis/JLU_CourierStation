#ifndef CHARGE_CALCULATION_H
#define CHARGE_CALCULATION_H
#include <stdbool.h>
#include "structure.h"
//用于计算包裹费用的函数
//(1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户):
//(1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品)
//(1-正常, 2-损坏, 3-违禁品)
#define base_rate 0.0001
void calculate_send_package_fees(struct package_s *send_pkg, int customer_type);
void calculate_receive_package_fees(struct package_r *recv_pkg, int customer_type);
double calculate_door_to_door(int customer_type,int package_type);

#endif
