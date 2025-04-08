#ifndef CHARGE_CALCULATION_H
#define CHARGE_CALCULATION_H
#include <stdbool.h>
#include "structure.h"
void calculate_send_package_fees(struct package_s *send_pkg, int customer_type);
void calculate_receive_package_fees(struct package_r *recv_pkg, int customer_type);

#endif
