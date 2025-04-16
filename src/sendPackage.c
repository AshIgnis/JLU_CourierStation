#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sendPackage.h"
#include "structure.h"
#include "charge_calculation.h"
#include "customer.h"

void free_package_s(struct package_s* head){
    struct package_s *lst,*now;
    lst=head;
    for(; ;){
        now=(*lst).next;
        free(lst);
        if(now==NULL){
            break;
        }lst=now;
    }return;
}

void show_package_s(struct package_s* now){
    printf("1.收件人姓名: \n");
    printf("%s\n",(*now).receiver_name);
    printf("2.收件人地址: \n");
    printf("%s\n",(*now).receiver_address);    
    printf("3.包裹体积 (立方厘米): ");
    printf("%.3lf\n",(*now).volume);
    printf("4.包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): ");
    printf("%d\n",(*now).package_type);
    printf("5.是否需要到付 (0-不需要, 1-需要): ");
    printf("%d\n",(*now).ifCollection);
    printf("6.包裹状态 (1-正常, 2-损坏, 3-违禁品): ");
    printf("%d\n",(*now).package_status);
    return;
}

void find_package_s(char* p,struct package_s* head){
    struct package_s *lst,*now;
    lst=head;
    int jd=0;
    for(; ;){
        now=(*lst).next;
        if(now==NULL){
            break;
        }if(strcmp((*now).phone_number,p)==0){
            jd+=1;
            printf("包裹%3d: \n",jd);
            show_package_s(now);
            printf("\n");
        }lst=now;
    }
    if(jd!=0){
        printf("您的寄件信息展示如上.\n");
    }else{
        printf("您还未曾进行寄件.\n");
    }
    return;
}

struct package_s* load_package_s(){
    FILE *file = fopen("send_packages.txt", "r");
    if (!file) {
        perror("No package file found");
        return NULL;
    }
    struct package_s *head,*lst;
    head=(struct package_s*)malloc(sizeof(struct package_s));
    (*head).next=NULL;
    lst=head;
    while(!feof(file)){
        struct package_s* now=(struct package_s*)malloc(sizeof(struct package_s));
        fscanf(file,"%s%s%s%lf%d%d",(*now).phone_number,(*now).receiver_name,(*now).receiver_address,&(*now).volume,&(*now).package_type,&(*now).ifCollection);
        fscanf(file,"%lf",&(*now).shipping_fee);
        fscanf(file,"%d",&(*now).package_status);
        (*lst).next=now;
        lst=now;
        (*lst).next=NULL;
    }
    fclose(file);
    return head;
}

void save_package_s(struct package_s* head){
    FILE *file = fopen("send_packages.txt", "w");
    if (!file) {
        perror("Failed to open file for saving packages");
        return;
    }
    struct package_s *lst,*now;
    lst=head;
    for(; ;){
        now=(*lst).next;
        if(now==NULL){
            break;
        }fprintf(file,"%s %s %s %lf %d %d",(*now).phone_number,(*now).receiver_name,(*now).receiver_address,(*now).volume,(*now).package_type,(*now).ifCollection);
        if((*now).ifCollection==1){
            fprintf(file," %.2lf",(*now).shipping_fee);
        }else{
            fprintf(file," 0");
        }fprintf(file," %d",(*now).package_status);
        if((*now).receiver_name[0]=='D'){
            break;
        }fprintf(file,"\n");
        lst=now;
    }
    fclose(file);
}

void package_s_original_start(char* t){
    struct package_s *head=load_package_s();
    char nm[22];//nm是电话号码
    for(int i=0;i<11;i++){
        nm[i]=t[i];
    }nm[11]='\0';
    int op;
    for(; ;){
        printf("\n========== 寄件包裹管理系统 ==========\n");
        printf("1. 添加寄件包裹\n");
        printf("2. 查询寄件包裹\n");
        printf("3. 保存寄件包裹\n");
        printf("0. 返回主菜单\n");
        printf("=====================================\n");    
        printf("请输入您的选择: ");
        while(scanf("%d",&op)!=1 || op<0 || op>3){
            printf("输入无效，请输入0-3之间的数字: ");
            while(getchar()!='\n'); // 清空输入缓冲区
        }
        if(op==0){
            save_package_s(head);
            free_package_s(head);
            break;
        }
        if(op==1){
            struct package_s* now=(struct package_s*)malloc(sizeof(struct package_s));
            for(int i=0;i<=13;i++){
                (*now).phone_number[i]=nm[i];
            }
            printf("1.请输入收件人姓名: \n");
            scanf("%s",(*now).receiver_name);
            printf("2.请输入收件人地址: \n");
            scanf("%s",(*now).receiver_address);
            printf("3.请输入包裹体积 (立方厘米): \n");
            while(scanf("%lf",&(*now).volume)!=1 || (*now).volume<=0){
                printf("输入无效，请输入一个正数: ");
                while(getchar()!='\n');
            }
            printf("4.请输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
            while(scanf("%d",&(*now).package_type)!=1 || (*now).package_type<1 || (*now).package_type>5){
                printf("输入无效，请输入1-5之间的数字: ");
                while(getchar()!='\n');
            }
            printf("5.在此缴纳运费 (0-不, 1-是): \n");
            while(scanf("%d",&(*now).ifCollection)!=1 || ((*now).ifCollection!=0 && (*now).ifCollection!=1)){
                printf("输入无效，请输入0或1: ");
                while(getchar()!='\n');
            }
            printf("6.请输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
            while(scanf("%d", &(*now).package_status)!=1 || (*now).package_status<1 || (*now).package_status>3){
                printf("输入无效，请输入1-3之间的数字: ");
                while(getchar()!='\n');
            }
            
            double shipping_fee = calculate_send_package_fees(now, get_customer_type(nm)); // 计算一次运费
            printf("包裹的运费计算为：%.2lf\n", shipping_fee);

            char jdg[22];
            for (;;) {
                printf("\n请仔细核对包裹信息录入是否正确 Y/N\n");
                for (int i = 0;;) {
                    scanf("%c", &jdg[i]);
                    if (i == 0 && jdg[i] == '\n') {
                        continue;
                    }
                    if (jdg[i] == '\n' && i <= 20) {
                        jdg[i] = '\0';
                        break;
                    }
                    if (i == 20) {
                        for (;;) {
                            scanf("%c", &jdg[i]);
                            if (jdg[i] == '\n') {
                                jdg[i] = '\0';
                                break;
                            }
                        }
                        break;
                    }
                    i++;
                }

                if (strlen(jdg) == 1 && (*jdg) == 'Y') {
                    break; // 用户确认信息正确，退出循环
                } else if (strlen(jdg) == 1 && (*jdg) == 'N') {
                    printf("请输入希望更改项的序号 (输入\"0\"以放弃本次修改):\n");
                    int lsl;
                    while (scanf("%d", &lsl) != 1 || lsl < 0 || lsl > 6) {
                        printf("输入无效，请输入0-6之间的数字: ");
                        while (getchar() != '\n');
                    }
                    if (lsl == 1) {
                        printf("请重新输入收件人姓名: \n");
                        scanf("%s", (*now).receiver_name);
                    }
                    if (lsl == 2) {
                        printf("请重新输入收件人地址: \n");
                        scanf("%s", (*now).receiver_address);
                    }
                    if (lsl == 3) {
                        printf("请重新输入包裹体积 (立方厘米): \n");
                        while (scanf("%lf", &(*now).volume) != 1 || (*now).volume <= 0) {
                            printf("输入无效，请输入一个正数: ");
                            while (getchar() != '\n');
                        }
                    }
                    if (lsl == 4) {
                        printf("请重新输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
                        while (scanf("%d", &(*now).package_type) != 1 || (*now).package_type < 1 || (*now).package_type > 5) {
                            printf("输入无效，请输入1-5之间的数字: ");
                            while (getchar() != '\n');
                        }
                    }
                    if (lsl == 5) {
                        printf("请重新输入是否在此缴纳运费 (0-不, 1-是): \n");
                        while (scanf("%d", &(*now).ifCollection) != 1 || ((*now).ifCollection != 0 && (*now).ifCollection != 1)) {
                            printf("输入无效，请输入0或1: ");
                            while (getchar() != '\n');
                        }
                    }
                    if (lsl == 6) {
                        printf("请重新输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
                        while (scanf("%d", &(*now).package_status) != 1 || (*now).package_status < 1 || (*now).package_status > 3) {
                            printf("输入无效，请输入1-3之间的数字: ");
                            while (getchar() != '\n');
                        }
                    }
                } else {
                    printf("输入\"Y\"以表示确认, \"N\"以表示需要修改包裹信息.\n");
                }
            }

            (*now).next=(*head).next;
            (*head).next=now;
           

            printf("寄件包裹添加成功！\n");
            save_package_s(head);
        }
        if(op==2){
            find_package_s(nm,head);
        }
        if(op==3){
            save_package_s(head);
            printf("收件包裹保存成功！\n");
        }
    }
    return;
}
