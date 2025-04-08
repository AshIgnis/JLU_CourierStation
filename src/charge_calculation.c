#include "charge_calculation.h"
#include "user.h"
#include "fileOperations.h"
#include "structure.h"
#include "customer.h"
#include "sendPackage.h"
#include "receivedPackage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

//(1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户):
//(1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品)
//(1-正常, 2-损坏, 3-违禁品)

double get_package_type_coefficient(int package_type) {
    switch (package_type) {
        case 1: return 1.0; // 文件
        case 2: return 1.2; // 生鲜
        case 3: return 1.5; // 易碎品
        case 4: return 1.3; // 家电
        case 5: return 2.0; // 危险品
        default: return 1.0; // 默认
    }
}

// 获取用户类型的折扣
double get_customer_discount(int customer_type) {
    switch (customer_type) {
        case 1: return 1.0; // 普通用户
        case 2: return 0.8; // VIP
        case 3: return 0.9; // 企业
        case 4: return 0.85; // 学生
        case 5: return 0.85; // 老年
        default: return 1.0; // 默认
    }
}

// 计算所有包裹的运费
void calculate_shipping_fees(struct customer *customers) {
    struct customer *current_customer = customers;
    const double BASE_RATE = 10.0; // 基础运费率，10元/单位体积

    while (current_customer != NULL) {
        // 处理寄件包裹
        struct package_s *send_pkg = current_customer->send_packages;
        while (send_pkg != NULL) {
            double base = send_pkg->volume * BASE_RATE;
            double type_coeff = get_package_type_coefficient(send_pkg->package_type);
            double discount = get_customer_discount(current_customer->customer_type);
            send_pkg->shipping_fee = base * type_coeff * discount;
            send_pkg = send_pkg->next;
        }

        // 处理收件包裹（仅处理到付）
        struct package_r *recv_pkg = current_customer->received_packages;
        while (recv_pkg != NULL) {
            if (recv_pkg->ifCollection == 1) {
                double base = recv_pkg->volume * BASE_RATE;
                double type_coeff = get_package_type_coefficient(recv_pkg->package_type);
                double discount = get_customer_discount(current_customer->customer_type);
                recv_pkg->shipping_fee = base * type_coeff * discount;
            }
            recv_pkg = recv_pkg->next;
        }

        current_customer = current_customer->next;
    }
}