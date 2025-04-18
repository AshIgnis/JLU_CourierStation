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
void initialize_random_seed()
{
    srand((unsigned int)time(NULL)); // 使用当前时间作为种子
}

double generate_random_factor()
{
    return 0.8 + ((double)rand() / RAND_MAX) * 0.19; // 生成0.8到0.99之间的随机数
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

    // 遍历文件内容，查找匹配的电话号码
    while (fscanf(file, "%s %s %s %d", current_name, current_phone, current_password, &current_type) != EOF)
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

double get_package_type_coefficient(int package_type)
{
    switch (package_type)
    {
    case 1:
        return 1.0; // 文件
    case 2:
        return 1.2; // 生鲜
    case 3:
        return 1.5; // 易碎品
    case 4:
        return 1.3; // 家电
    case 5:
        return 2.0; // 危险品
    default:
        return 1.0; // 默认
    }
}

// 获取用户类型的折扣
double get_customer_discount(int customer_type)
{
    switch (customer_type)
    {
    case 1:
        return 1.0; // 普通用户
    case 2:
        return 0.8; // VIP
    case 3:
        return 0.9; // 企业
    case 4:
        return 0.85; // 学生
    case 5:
        return 0.85; // 老年
    default:
        return 1.0; // 默认
    }
}

// 分段计费规则
double calculate_volume_fee(double volume)
{
    if (volume <= 6000)
    {
        return 3.0;
    }
    else if (volume <= 15000)
    {
        return 3.0 + (volume - 6000) * 0.001; // 6000～15000每体积0.001
    }
    else
    {
        return 12.0 + (volume - 15000) * 0.002; // 15000以上每体积0.002
    }
}

// 保留两位小数
double round_to_two_decimals(double value)
{
    return round(value * 100.0) / 100.0;
}

// 修改后的寄件包裹运费计算
double calculate_send_package_fees(struct package_s *send_pkg, int customer_type, int ifdoortodoor)
{
    double shipping_fee = 0.0; // 初始化运费
    double Vbase = calculate_volume_fee(send_pkg->volume); // 使用分段计费规则
    double type_coeff = get_package_type_coefficient(send_pkg->package_type);
    double discount = get_customer_discount(customer_type);
    
    if (send_pkg->ifCollection)
    {       
        shipping_fee = Vbase * type_coeff * discount;

        // 等待时间函数
        double random_factor = generate_random_factor(); // 生成0.8到0.99之间的随机数
        printf("新店开张，三个月随机减免，此包裹抽中了%lf的打折比例\n", random_factor);
        shipping_fee *= random_factor;

        // 判断是否抽中免单
        if (((double)rand() / RAND_MAX) < 0.05)
        { // 5%的概率免单
            shipping_fee = 0.0;
            printf("包裹抽中了免单！\n");
        }

        printf("包裹运费为：%.2lf\n", shipping_fee);
    }
    if (ifdoortodoor)
    {
        double doorfee = discount * Vbase * type_coeff * 0.7;
        printf("上门服务费用为：%.2lf\n", doorfee);
        shipping_fee += doorfee; // 上门服务费用
    }
    shipping_fee = round_to_two_decimals(shipping_fee); // 保留两位小数
    send_pkg->shipping_fee = shipping_fee;
    return shipping_fee;
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