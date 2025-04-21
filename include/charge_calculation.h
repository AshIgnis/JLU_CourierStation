#ifndef CHARGE_CALCULATION_H
#define CHARGE_CALCULATION_H
#include <stdbool.h>
#include "structure.h"

// 用于计算包裹费用的函数
double get_package_type_coefficient(int package_type);
double get_customer_discount(int customer_type);
double calculate_volume_fee(double volume);
double round_to_two_decimals(double value);
double calculate_send_package_fees(struct package_s *send_pkg, const char *phone_number);
int get_customer_type(const char *phone_number);
bool get_customer_tickets(const char *phone_number, int *tickets); // 修改为 bool 类型
void decrement_customer_tickets(const char *phone_number);
double DoorToDoorFee_r(const char* phone_number, double vol, int pkg_type);

#endif
