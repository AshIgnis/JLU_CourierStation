#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "stdbool.h"
#include "ctype.h"
#include "structure.h"

// 用户相关功能声明

struct customer *addCustomer(struct customer *head, const char *username, const char *phone_number, const char *password, int customer_type);
void queryCustomer(struct customer *head);
struct customer *deleteCustomer(struct customer *head);
void freeCustomers(struct customer *head);
int isPhoneNumberDuplicate(struct customer *head, const char *phone_number);
void displayAllCustomers(struct customer *head);
void displayMenu_customer();
struct customer *customersCreating(struct customer *customerList);


#endif // CUSTOMER_H