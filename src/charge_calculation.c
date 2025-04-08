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
#include <time.h> // 添加头文件以支持随机数生成

//(1-普通用户, 2-VIP用户, 3-企业用户, 4-学生用户, 5-老年用户):
//(1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品)
//(1-正常, 2-损坏, 3-违禁品)
#define base_rate 0.0001

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

// 计算寄件包裹的运费
void calculate_send_package_fees(struct package_s *send_pkg, int customer_type) {
    while (send_pkg != NULL) {
        double base = send_pkg->volume * base_rate;
        double type_coeff = get_package_type_coefficient(send_pkg->package_type);
        double discount = get_customer_discount(customer_type);
        double random_factor = 0.8 + ((double)rand() / RAND_MAX) * 0.19; // 生成0.8到0.99之间的随机数
        send_pkg->shipping_fee = base * type_coeff * discount * random_factor;
        printf("新店开张，随机折扣八折到九九折，其中包裹%s抽到的比例%lf\n", send_pkg->package_id, random_factor);

        // 判断是否抽中免单
        if (((double)rand() / RAND_MAX) < 0.05) { // 5%的概率免单
            send_pkg->shipping_fee = 0.0;
            printf("包裹ID %s 抽中了免单！\n", send_pkg->package_id);
        }

        send_pkg = send_pkg->next;
    }
}

// 计算收件包裹的运费
void calculate_receive_package_fees(struct package_r *recv_pkg, int customer_type) {
    while (recv_pkg != NULL) {
        if (recv_pkg->ifCollection == 1) { // 仅处理到付包裹
            double base = recv_pkg->volume * base_rate;
            double type_coeff = get_package_type_coefficient(recv_pkg->package_type);
            double discount = get_customer_discount(customer_type);
            double random_factor = 0.8 + ((double)rand() / RAND_MAX) * 0.19; // 生成0.8到0.99之间的随机数
            recv_pkg->shipping_fee = base * type_coeff * discount * random_factor;
            printf("新店开张，随机折扣八折到九九折，其中包裹%s抽到的比例%lf\n", recv_pkg->package_id, random_factor);

            // 判断是否抽中免单
            if (((double)rand() / RAND_MAX) < 0.05) { // 5%的概率免单
                recv_pkg->shipping_fee = 0.0;
                printf("包裹ID %s 抽中了免单！\n", recv_pkg->package_id);
            }
        } else {
            recv_pkg->shipping_fee = 0;
        }

        recv_pkg = recv_pkg->next;
    }
}
