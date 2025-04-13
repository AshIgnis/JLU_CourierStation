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
#include <math.h> // 添加头文件以支持保留小数
//
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

// 分段计费规则
double calculate_volume_fee(double volume) {
    if (volume <= 6000) {
        return 3.0;
    } else if (volume <= 15000) {
        return 3.0+(volume-6000) * 0.001; // 6000～15000每体积0.001
    } else {
        return 12.0 + (volume-15000) * 0.002; // 15000以上每体积0.002
    }
}

// 保留两位小数
double round_to_two_decimals(double value) {
    return round(value * 100.0) / 100.0;
}

// 修改后的寄件包裹运费计算
double calculate_send_package_fees(struct package_s *send_pkg, int customer_type,int ifDoorToDoor) {
        double shipping_fee = 0.0; // 初始化运费
        double Vbase = calculate_volume_fee(send_pkg->volume); // 使用分段计费规则
        double type_coeff = get_package_type_coefficient(send_pkg->package_type);
        double discount = get_customer_discount(customer_type);
        double random_factor = 0.8 + ((double)rand() / RAND_MAX) * 0.19; // 生成0.8到0.99之间的随机数
        shipping_fee = Vbase * type_coeff * discount * random_factor;

        // 判断是否抽中免单
        if (((double)rand() / RAND_MAX) < 0.05) { // 5%的概率免单
            shipping_fee = 0.0;
            printf("包裹抽中了免单！\n");
        }

        if(ifDoorToDoor) shipping_fee +=calculate_door_to_door(customer_type,send_pkg->package_type);
        shipping_fee = round_to_two_decimals(send_pkg->shipping_fee); // 保留两位小数
        return shipping_fee;
    
}

// 修改后的收件包裹运费计算
double calculate_receive_package_fees(struct package_r *recv_pkg, int customer_type,int ifDoorToDoor) {
        double shipping_fee = 0.0;
        if (recv_pkg->ifCollection == 1) { // 仅处理到付包裹
            double Vbase = calculate_volume_fee(recv_pkg->volume); // 使用分段计费规则
            double type_coeff = get_package_type_coefficient(recv_pkg->package_type);
            double discount = get_customer_discount(customer_type);
            double random_factor = 0.8 + ((double)rand() / RAND_MAX) * 0.19; // 生成0.8到0.99之间的随机数
            shipping_fee = Vbase * type_coeff * discount * random_factor;
            

           // printf("新店开张，随机折扣八折到九九折，其中包裹%s抽到的比例%lf\n", recv_pkg->package_id, random_factor);

            // 判断是否抽中免单
            if (((double)rand() / RAND_MAX) < 0.05) { // 5%的概率免单
                shipping_fee = 0.0;
                printf("包裹ID %s 抽中了免单！\n", recv_pkg->package_id);
            }
        }
        if(ifDoorToDoor) shipping_fee+=calculate_door_to_door(customer_type,recv_pkg->package_type);

        shipping_fee = round_to_two_decimals(recv_pkg->shipping_fee); // 保留两位小数
        return shipping_fee;        
    
}

double calculate_door_to_door(int customer_type,int package_type){
    return 5*get_customer_discount(customer_type)*get_package_type_coefficient(package_type);
}
