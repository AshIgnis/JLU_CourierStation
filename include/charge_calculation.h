#ifndef CHARGE_CALCULATION_H
#define CHARGE_CALCULATION_H
#include <stdbool.h>
#include "structure.h"

double get_package_type_coefficient(int package_type);
double get_customer_discount(int customer_type);
void calculate_shipping_fees(struct customer *customers);

#endif
