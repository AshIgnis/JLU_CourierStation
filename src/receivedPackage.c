#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receivedPackage.h"

void get_package_r_id(struct package_r* x){
	struct node{
		int id;
		struct node *next;
	};FILE *file = fopen("id_box.txt", "r");
	if(!file){
		perror("ID_File is Lost. Please call the manager.");
		return;
	}
	
	int tmp,dmp;
	fscanf(file,"%d",&tmp);
	
	dmp=tmp/10000+1;
	
	if((*x).volume>15000){
		(*x).package_id[0]='C';
	}else{
		if((*x).volume>6000){
			(*x).package_id[0]='B';
		}else{
			(*x).package_id[0]='A';
		}
	}(*x).package_id[1]=dmp/10+'0';
	(*x).package_id[2]=dmp%10+'0';
	(*x).package_id[3]='-';
	(*x).package_id[7]=tmp%10+'0';
	tmp/=10;
	(*x).package_id[6]=tmp%10+'0';
	tmp/=10;
	(*x).package_id[5]=tmp%10+'0';
	tmp/=10;
	(*x).package_id[4]=tmp%10+'0';
	(*x).package_id[8]='\0';
		
	struct node *start = (struct node*)malloc(sizeof(struct node));
	struct node *now,*nxt;
	now=start;
	(*start).next=NULL;
	while(!feof(file)){
		nxt = (struct node*)malloc(sizeof(struct node));
		(*now).next=nxt;
		fscanf(file,"%d",&(*nxt).id);
		(*nxt).next=NULL;
		now=nxt;
	}fclose(file);
	now=(*start).next;
	file = fopen("id_box.txt", "w");
	while(now!=NULL){
		fprintf(file, " %d",(*now).id);
		now=(*now).next;
	}fclose(file);
	now=start;
	nxt=(*now).next;
	while(now==NULL){
		free(now);
		now=nxt;
		nxt=(*now).next;
	}return;
}

void return_package_r_id(struct package_r* x){
	FILE *file = fopen("id_box.txt", "a");
	
	int tmp;
	tmp=(*x).package_id[1]-'0';
	tmp*=10;
	tmp+=(*x).package_id[2]-'0';
	tmp-=1;tmp*=10;
	tmp+=(*x).package_id[7]-'0';
	tmp*=10;
	tmp+=(*x).package_id[6]-'0';
	tmp*=10;
	tmp+=(*x).package_id[5]-'0';
	tmp*=10;
	tmp+=(*x).package_id[4]-'0';
	
	fprintf(file, " %d",tmp);
	fclose(file);
	return;
}

void show_package_r(struct package_r* now,int if_id){
	
	printf("1.包裹所属用户电话: \n");
	printf("%s\n",(*now).phone_number);
	printf("2.包裹体积 (立方厘米): \n");
	printf("%lf\n",(*now).volume);
	printf("3.包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
	printf("%d\n",(*now).package_type);
	printf("4.是否需要到付 (0-不需要, 1-需要): \n");
	printf("%d\n",(*now).ifCollection);
	if((*now).ifCollection){
		printf("运输费用: \n");
		printf("%lf\n",(*now).shipping_fee);
	}printf("5.包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
	printf("%d\n",(*now).package_status);
	
	if(if_id==1){
		printf("6.包裹序列号: \n");
		printf("%s\n",(*now).package_id);
	}
	
	return;
}

void find_package_r(struct package_r **lstfind,struct package_r **find,char* p,struct package_r* head){
	
	struct package_r *lst,*now;
	lst=head;
	
	for(; ;){
		now=(*lst).next;
		if(now==NULL){
			(*lstfind)=NULL;
			(*find)=NULL;
			break;
		}if(strcmp((*now).package_id,p)==0){
			(*lstfind)=lst;
			(*find)=now;
			break;
		}lst=now;
	}
	
	return;
}

struct package_r* load_package_r(){
	
	FILE *file = fopen("packagelist.txt", "r");
	if (!file) {
		perror("No package file found");
		return NULL;
	}
	
	struct package_r *head,*lst;
	head=(struct package_r*)malloc(sizeof(struct package_r));
	(*head).next=NULL;
	lst=head;
	
	while(!feof(file)){
		struct package_r* now=(struct package_r*)malloc(sizeof(struct package_r));
		fscanf(file,"%s%lf%d%d",(*now).phone_number,&(*now).volume,&(*now).package_type,&(*now).ifCollection);
		if((*now).ifCollection==1){
			fscanf(file,"%lf",&(*now).shipping_fee);
		}fscanf(file,"%d",&(*now).package_status);
		fscanf(file,"%s",(*now).package_id);
		
		(*lst).next=now;
		lst=now;
		(*lst).next=NULL;
	}
	
	fclose(file);
	
	return head;
}

void save_package_r(struct package_r* head){
	FILE *file = fopen("packagelist.txt", "w");
	if (!file) {
		perror("Failed to open file for saving packages");
		return;
	}
	
	struct package_r *lst,*now;
	lst=head;
	
	for(; ;){
		now=(*lst).next;
		if(now==NULL){
			break;
		}fprintf(file,"%s %lf %d %d",(*now).phone_number,(*now).volume,(*now).package_type,(*now).ifCollection);
		if((*now).ifCollection==1){
			fprintf(file," %lf",(*now).shipping_fee);
		}fprintf(file," %d",(*now).package_status);
		fprintf(file," %s",(*now).package_id);
		
		if((*now).next!=NULL){
			fprintf(file,"\n");
		}
		
		lst=now;
	}
	
	fclose(file);
}

void package_r_original_start(){
	
	package_r *head=load_package_r();
	
	int op;
	
	for(; ;){
		
		printf("\n========== 收件包裹管理系统 ==========\n");
		printf("1. 添加收件包裹\n");
		printf("2. 查询收件包裹\n");
		printf("3. 保存收件包裹\n");
		printf("4. 删除指定包裹\n");
		printf("0. 返回主菜单\n");
		printf("=====================================\n");	
		printf("请输入您的选择: ");
		
		scanf("%d",&op);
		
		if(op==0){
			save_package_r(head);
			break;
		}
		
		if(op==1){
			
			struct package_r* now=(struct package_r*)malloc(sizeof(struct package_r));
			printf("1.请输入包裹所属用户电话: \n");
			scanf("%s",(*now).phone_number);
			printf("2.请输入包裹体积 (立方厘米): \n");
			scanf("%lf",&(*now).volume);
			printf("3.请输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
			scanf("%d",&(*now).package_type);
			printf("4.是否需要到付 (0-不需要, 1-需要): \n");
			scanf("%d",&(*now).ifCollection);
			if((*now).ifCollection){
				printf("请输入运输费用: \n");
				scanf("%lf",&(*now).shipping_fee);
			}printf("5.请输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
			scanf("%d", &(*now).package_status);
			
			char jdg[22];
			for(; ;){
				
				printf("\n包裹信息展示如下: \n\n");
				show_package_r(now,0);
				printf("\n请仔细核对包裹信息录入是否正确 Y/N\n");
				for(int i=0; ;){
					scanf("%c",&jdg[i]);
					if(i==0&&jdg[i]=='\n'){
						continue;
					}if(jdg[i]=='\n'&&i<=20){
						jdg[i]='\0';
						break;
					}if(i==20){
						for(; ;){
							scanf("%c",&jdg[i]);
							if(jdg[i]=='\n'){
								jdg[i]='\0';
								break;
							}
						}break;
					}i++;
				}
				
				if(strlen(jdg)==1&&(*jdg)=='Y'){
					break;
				}else{
					if(strlen(jdg)==1&&(*jdg)=='N'){
						printf("请输入希望更改项的序号 (输入\"0\"以放弃本次修改):\n");	
						int lsl;
						scanf("%d",&lsl);
						if(lsl==1){
							printf("请重新输入包裹所属用户电话: \n");
							scanf("%s",(*now).phone_number);
						}if(lsl==2){
							printf("请重新输入包裹体积 (立方厘米): \n");
							scanf("%lf",&(*now).volume);
						}if(lsl==3){
							printf("请重新输入包裹类型 (1-文件, 2-生鲜, 3-易碎品, 4-家电, 5-危险品): \n");
							scanf("%d",&(*now).package_type);
						}if(lsl==4){
							printf("请重新输入是否需要到付 (0-不需要, 1-需要): \n");
							scanf("%d",&(*now).ifCollection);
							if((*now).ifCollection){
								printf("请重新输入运输费用: \n");
								scanf("%lf",&(*now).shipping_fee);
							}
						}if(lsl==5){
							printf("请重新输入包裹状态 (1-正常, 2-损坏, 3-违禁品): \n");
							scanf("%d", &(*now).package_status);
						}
					}else{
						printf("输入\"Y\"以表示确认, \"N\"以表示需要修改包裹信息. \n");
					}
				}
			}
			
			(*now).next=(*head).next;
			(*head).next=now;
			
			get_package_r_id(now);
			
			printf("收件包裹添加成功！\n");
			printf("已自动为您分配包裹序列号：\n%s\n",(*now).package_id);
			save_package_r(head);
		}
		
		if(op==2){
			
			printf("请输入希望查询的包裹序列号: \n");
			char p[22];
			for(int i=0; ;){
				scanf("%c",&p[i]);
				if(i==0&&p[i]=='\n'){
					continue;
				}if(p[i]=='\n'&&i<=20){
					p[i]='\0';
					break;
				}if(i==20){
					for(; ;){
						scanf("%c",&p[i]);
						if(p[i]=='\n'){
							p[i]='\0';
							break;
						}
					}break;
				}i++;
			}
			
			struct package_r *lstfin,*fin;
			find_package_r(&lstfin,&fin,p,head);
			
			if(fin==NULL){
				printf("未查询到相应包裹, 请检查序列号拼写是否正确. \n");
				continue;
			}printf("\n查询到包裹信息如下: \n\n");
			
			show_package_r(fin,1);
			
		}
		
		if(op==3){
			save_package_r(head);
			printf("收件包裹保存成功！\n");
		}
		
		if(op==4){
			
			printf("请输入希望查询的包裹序列号: \n");
			char p[22];
			for(int i=0; ;){
				scanf("%c",&p[i]);
				if(i==0&&p[i]=='\n'){
					continue;
				}if(p[i]=='\n'&&i<=20){
					p[i]='\0';
					break;
				}if(i==20){
					for(; ;){
						scanf("%c",&p[i]);
						if(p[i]=='\n'){
							p[i]='\0';
							break;
						}
					}break;
				}i++;
			}
			
			struct package_r *lstfin,*fin;
			find_package_r(&lstfin,&fin,p,head);
			
			if(fin==NULL){
				printf("未查询到相应包裹, 请检查序列号拼写是否正确. \n");
				continue;
			}
			
			printf("请慎重选择, 确认删除此包裹吗? Y/N\n");
			
			for(; ;){
				
				for(int i=0; ;){
					scanf("%c",&p[i]);
					if(i==0&&p[i]=='\n'){
						continue;
					}if(p[i]=='\n'&&i<=20){
						p[i]='\0';
						break;
					}if(i==20){
						for(; ;){
							scanf("%c",&p[i]);
							if(p[i]=='\n'){
								p[i]='\0';
								break;
							}
						}break;
					}i++;
				}
				
				if(strlen(p)==1&&(*p)=='Y'){
					return_package_r_id(fin);
					(*lstfin).next=(*fin).next;
					free(fin);
					printf("包裹已删除. \n");
					save_package_r(head);
					break;
				}else{
					if(strlen(p)==1&&(*p)=='N'){
						printf("操作已取消. \n");
						break;
					}else{
						printf("输入\"Y\"以表示确认删除此包裹, \"N\"以取消删除操作. \n");
					}
				}
			}
			
		}
	}
	
}
