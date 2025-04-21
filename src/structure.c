#include "structure.h"
#include <ctype.h>
// 用户类型字符串数组
const char *cstmType[5] = {
    "普通用户", 
    "VIP用户", 
    "企业用户", 
    "学生用户", 
    "老年用户"
};

// 包裹类型字符串数组
const char *pkgType[5] = {
    "文件", 
    "生鲜", 
    "易碎品", 
    "家电", 
    "危险品"
};

// 包裹状态字符串数组
const char *pkgStatus[3] = {
    "正常", 
    "损坏", 
    "违禁品"
};

// 是否到付字符串数组
const char *ifcollection[4] = {
    "不需费用", 
    "需要到付",
    "需要上门",
    "到付上门"
};
int isValidFloat(const char *str) {
    int dot_count = 0; // 用于记录小数点的数量
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dot_count++;
            if (dot_count > 2) {
                return 0; // 如果小数点超过2个，则不是有效的浮点数
            }
        } else if (!isdigit(str[i])) {
            return 0; // 非数字字符
        }
    }
    return 1; // 全部是数字或最多一个小数点
}

int isValidNumber(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0; // 非数字字符
        }
    }
    return 1; // 全部是数字
}
