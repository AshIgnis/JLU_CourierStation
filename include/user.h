#ifndef USER_H
#define USER_H
#include <stdbool.h>
#include "structure.h"

void userOperating(struct customer *head);
void userReceivedPackagesSearching(const char *phone_number);
void userAction(struct customer *head,const char *phone_number);
void displayMenu_user();
bool userLanding(struct customer *head,const char *phone_number, const char *password);
void userTakePackage(const char *phone_number);
int convertStringToInt(const char *input);

#endif