#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "stdbool.h"
#include "ctype.h"
#include "structure.h"

// 用户相关功能声明

struct customer *addCustomer(struct customer *head);
void queryCustomer(struct customer *head);
void displayAllCustomers(struct customer *head);
struct customer *modifyCustomer(struct customer *head);
struct customer *deleteCustomer(struct customer *head);

void freeCustomers(struct customer *head);
int isPhoneNumberDuplicate(struct customer *head, const char *phone_number);
void displayMenu_customer();
struct customer *customersOperation(struct customer *customerList);


#endif // CUSTOMER_H