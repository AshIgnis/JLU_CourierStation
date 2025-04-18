#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileOperations.h"

// 保存用户到文件
void saveCustomers(struct customer *head) {
    FILE *file = fopen(CUSTOMER_FILE, "w");
    if (!file) {
        perror("Failed to open file for saving customers");
        return;
    }

    struct customer *current = head;
    while (current) {
        fprintf(file, "%s %s %s %d\n",
                current->username, current->phone_number,
                current->password, current->customer_type);
        current = current->next;
    }

    fclose(file);
    printf("用户数据保存成功！\n");
}

//从文件读取用户
struct customer *loadCustomers() {
    FILE *file = fopen(CUSTOMER_FILE, "r");
    if (!file) {
        perror("No customers file found");
        return NULL;
    }

    struct customer *head = NULL, *tail = NULL;
    while (1) {
        struct customer *cust = (struct customer *)malloc(sizeof(struct customer));
        if (!cust) {
            perror("Memory allocation failed");
            break;
        }
        if (fscanf(file, "%s %s %s %d",
                   cust->username, cust->phone_number,
                   cust->password, &cust->customer_type) == 4) {
            cust->next = NULL;
            if (!head) {
                head = tail = cust;
            } else {
                tail->next = cust;
                tail = cust;
            }
        } else {
            free(cust);
            break;
        }
    }

    fclose(file);
    return head;
}
