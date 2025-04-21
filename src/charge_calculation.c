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
#include <stdbool.h> // 确保包含 bool 类型的头文件

// 定义用户类型折扣数组
static const double customer_discounts[] = {
    1.0,  // 默认
    1.0,  // 普通用户
    0.8,  // VIP
    0.9,  // 企业
    0.85, // 学生
    0.85  // 老年
};

// 获取用户类型的折扣
double get_customer_discount(int customer_type) {
    if (customer_type >= 1 && customer_type <= 5) {
        return customer_discounts[customer_type];
    }
    return customer_discounts[0]; // 默认折扣
}

// 定义包裹类型费用系数数组
static const double package_type_coefficients[] = {
    1.0,  // 默认
    1.0,  // 文件
    1.2,  // 生鲜
    1.5,  // 易碎品
    1.3,  // 家电
    2.0   // 危险品
};

// 获取包裹类型的费用系数
double get_package_type_coefficient(int package_type) {
    if (package_type >= 1 && package_type <= 5) {
        return package_type_coefficients[package_type];
    }
    return package_type_coefficients[0]; // 默认系数
}

// 分段计费规则
double calculate_volume_fee(double volume) {
    if (volume <= 6000) {
        return 3.0;
    } else if (volume <= 15000) {
        return 3.0 + (volume - 6000) * 0.001; // 6000～15000每体积0.001
    } else {
        return 12.0 + (volume - 15000) * 0.002; // 15000以上每体积0.002
    }
}

// 保留两位小数
double round_to_two_decimals(double value) {
    return round(value * 100.0) / 100.0;
}

// 修改后的寄件包裹运费计算
double calculate_send_package_fees(struct package_s *send_pkg, const char *phone_number) {
    double shipping_fee = 0.0; // 初始化运费
    if (send_pkg == NULL) {
        printf("包裹信息无效，无法计算运费。\n");
        return shipping_fee; // 返回 0.0 表示无效运费
    }

    double Vbase = calculate_volume_fee(send_pkg->volume); // 使用分段计费规则
    double type_coeff = get_package_type_coefficient(send_pkg->package_type);
    printf("包裹类型系数为：%.2lf\n", type_coeff);
    double discount = get_customer_discount(get_customer_type(phone_number));
    printf("用户折扣系数为：%.2lf\n", discount);

    // 计算基础运费
    if (send_pkg->ifCollection == 1 || send_pkg->ifCollection == 3) { // 需要到付或到付上门
        shipping_fee = Vbase * type_coeff * discount;

        // 检测优惠券状态
        int tickets = 0;
        if (get_customer_tickets(phone_number, &tickets)) {
            if (tickets > 0 && send_pkg->day < 91) { // 判断优惠券是否有效
                printf("检测到您有八折券，已自动使用\n");
                shipping_fee *= 0.8; // 应用八折优惠
                
            } else if (tickets > 0) {
                printf("检测到您有八折券，但已过期，无法使用\n");
            } else {
                printf("没有可用的八折券。\n");
            }
        }
        printf("基础运费为：%.2lf\n", shipping_fee);
    }

    // 计算上门服务费用
    if (send_pkg->ifCollection == 2 || send_pkg->ifCollection == 3) { // 需要上门或到付上门
        double doorfee = discount * Vbase * type_coeff * 0.7;
        printf("上门服务费用为：%.2lf\n", doorfee);
        shipping_fee += doorfee; // 累加上门服务费用
    }

    shipping_fee = round_to_two_decimals(shipping_fee); // 保留两位小数
    send_pkg->shipping_fee = shipping_fee;
    return shipping_fee;
}

int get_customer_type(const char *phone_number)
{
    FILE *file = fopen("customers.txt", "r");
    if (!file)
    {
        perror("无法打开 customers.txt 文件");
        return -1; // 返回 -1 表示文件打开失败
    }

    char current_phone[MAX_LEN];
    char current_name[MAX_LEN];
    char current_password[MAX_LEN];
    int current_type;
    int current_tickets;
    // 遍历文件内容，查找匹配的电话号码
    while (fscanf(file, "%s %s %s %d %d", current_name, current_phone, current_password, &current_type,&current_tickets) != EOF)
    {
        if (strcmp(current_phone, phone_number) == 0)
        {
            fclose(file);
            return current_type; // 返回匹配的用户类型
        }
    }

    fclose(file);
    return -1; // 返回 -1 表示未找到匹配的用户
}

bool get_customer_tickets(const char *phone_number, int *tickets) {
    FILE *file = fopen(CUSTOMER_FILE, "r");
    if (!file) {
        perror("无法打开 customers.txt 文件");
        return false; // 文件打开失败
    }

    char current_phone[MAX_LEN];
    char current_name[MAX_LEN];
    char current_password[MAX_LEN];
    int current_type;
    int current_tickets;

    // 遍历文件内容，查找匹配的电话号码
    while (fscanf(file, "%s %s %s %d %d", current_name, current_phone, current_password, &current_type, &current_tickets) != EOF) {
        if (strcmp(current_phone, phone_number) == 0) {
            *tickets = current_tickets; // 将找到的票数存储到参数中
            fclose(file);
            return true; // 找到用户，返回 true
        }
    }

    fclose(file);
    *tickets = 0; // 未找到用户，票数设置为 0
    return false; // 未找到用户
}

void decrement_customer_tickets(const char *phone_number) {
    FILE *file = fopen(CUSTOMER_FILE, "r");
    if (!file) {
        perror("无法打开 customers.txt 文件");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        perror("无法创建临时文件 temp.txt");
        fclose(file);
        return;
    }

    char current_phone[MAX_LEN];
    char current_name[MAX_LEN];
    char current_password[MAX_LEN];
    int current_type;
    int current_tickets;

    // 遍历文件内容，查找匹配的电话号码
    while (fscanf(file, "%s %s %s %d %d", current_name, current_phone, current_password, &current_type, &current_tickets) != EOF) {
        if (strcmp(current_phone, phone_number) == 0 && current_tickets > 0) {
            current_tickets--; // 使用票数后减 1
        }
        fprintf(temp, "%s %s %s %d %d\n", current_name, current_phone, current_password, current_type, current_tickets);
    }

    fclose(file);
    fclose(temp);

    // 替换原文件
    if (remove(CUSTOMER_FILE) != 0) {
        perror("删除原文件失败");
        return;
    }
    if (rename("temp.txt", CUSTOMER_FILE) != 0) {
        perror("重命名临时文件失败");
        return;
    }
}

double DoorToDoorFee_r(const char *phone_number, double vol, int pkg_type)
{
    double fee = 0.7 * get_customer_discount(get_customer_type(phone_number)) * calculate_volume_fee(vol) * get_package_type_coefficient(pkg_type);
    fee = round_to_two_decimals(fee); // 保留两位小数
    return fee;
}
// 修改后的收件包裹运费计算

// 收件只是输入，不需要计算
// 寄件则需要计算费用，从文件中读写寄件的包裹
// 上门费用与体积有关