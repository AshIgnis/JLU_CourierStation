#ifndef STRUCTURE_H
#define STRUCTURE_H

#define RECEIVED_FILE "received_packages.txt" // 收件包裹文件
#define SEND_FILE "send_packages.txt" // 寄件包裹文件
#define CUSTOMER_FILE "customers.txt" // 用户文件
#define ADMIN_FILE "admin.txt" // 管理员文件

#define MAX_LEN 100 // 字符串最大长度
#define MAX_PACKAGE_NUM 10000 // 最大包裹数量
// 用户类型字符串数组
// 用户类型字符串数组
extern const char *cstmType[5];

// 包裹类型字符串数组
extern const char *pkgType[5];

// 包裹状态字符串数组
extern const char *pkgStatus[3];

// 是否到付字符串数组
extern const char *ifcollection[4];

// 收件包裹结构体
struct package_r {
    char package_id[MAX_LEN]; // 包裹ID
    double volume; // 体积
    int package_type; // 包裹类型
    int ifCollection; // 是否到付
    double shipping_fee; // 运费
    int package_status; // 包裹状态
    int user_id; // 用户ID
    struct package_r *next; // 下一个包裹
    char phone_number[MAX_LEN]; // 电话号码
    int day;//到达驿站时间
};

// 寄件包裹结构体
struct package_s {
    char phone_number[MAX_LEN]; // 电话号码
    char receiver_name[MAX_LEN]; // 收件人姓名
    char receiver_address[MAX_LEN]; // 收件人地址
    double volume; // 体积
    int package_type; // 包裹类型
    int day;
    int ifCollection; // 是否到付
    int package_status; // 包裹状态   

    double shipping_fee; // 运费    
    struct package_s *next; // 下一个包裹
    
};

// 用户结构体
struct customer {
    char username[MAX_LEN]; // 用户名
    char phone_number[MAX_LEN]; // 电话号码
    char password[MAX_LEN]; // 密码
    int customer_type; // 用户类型
    int tickets;
    struct customer *next; // 下一个用户
};

struct admin{
    char adminname[MAX_LEN]; // 管理员姓名
    char password[MAX_LEN]; // 管理员密码
    struct admin *next; // 下一个管理员
};

#endif // STRUCTURE_H