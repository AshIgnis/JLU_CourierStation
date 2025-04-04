#ifndef STRUCTURE_H
#define STRUCTURE_H

#define RECEIVED_FILE "received_packages.txt" // 收件包裹文件
#define SEND_FILE "send_packages.txt" // 寄件包裹文件
#define CUSTOMER_FILE "customers.txt" // 用户文件

#define MAX_LEN 100 // 字符串最大长度
#define MAX_PACKAGE_NUM 10000 // 最大包裹数量

// 收件包裹结构体
struct package_r {
    char package_id[MAX_LEN]; // 包裹ID
    double volume; // 体积
    int package_type; // 包裹类型
    int ifCollection; // 是否代付
    double shipping_fee; // 运费
    int package_status; // 包裹状态
    int user_id; // 用户ID
    struct package_r *next; // 下一个包裹
    char phone_number[MAX_LEN]; // 电话号码
};

// 寄件包裹结构体
struct package_s {
    char package_id[MAX_LEN]; // 包裹ID
    char sender_name[MAX_LEN]; // 寄件人姓名
    char receiver_name[MAX_LEN]; // 收件人姓名
    char sender_address[MAX_LEN]; // 寄件人地址
    char receiver_address[MAX_LEN]; // 收件人地址
    double volume; // 体积
    int package_type; // 包裹类型
    char order_time[MAX_LEN]; // 下单时间
    double shipping_fee; // 运费
    int package_status; // 包裹状态
    struct package_s *next; // 下一个包裹
    char phone_number[MAX_LEN]; // 电话号码
};

// 用户结构体
struct customer {
    char username[MAX_LEN]; // 用户名
    char phone_number[MAX_LEN]; // 电话号码
    char password[MAX_LEN]; // 密码
    int customer_type; // 用户类型
    struct package_r *received_packages; // 收件包裹
    struct package_s *send_packages; // 寄件包裹
    struct customer *next; // 下一个用户
    int ticket;
};

#endif // STRUCTURE_H