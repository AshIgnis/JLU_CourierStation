#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "receivedPackage.h"
#include "structure.h"

void free_package_r(struct package_r* head){
	struct package_r *lst,*now;
	lst=head;
	for(; ;){
		now=(*lst).next;
		free(lst);
		if(now==NULL){
			break;
		}lst=now;
	}return;
}

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

void return_package_r_id(struct package_r *x){
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
	
	printf("1.°ü¹üËùÊôÓÃ»§µç»°: \n");
	printf("%s\n",(*now).phone_number);
	printf("2.°ü¹üÌå»ý (Á¢·½ÀåÃ×): \n");
	printf("%lf\n",(*now).volume);
	printf("3.°ü¹üÀàÐÍ (1-ÎÄ¼þ, 2-ÉúÏÊ, 3-Ò×ËéÆ·, 4-¼Òµç, 5-Î£ÏÕÆ·): \n");
	printf("%d\n",(*now).package_type);
	printf("4.ÊÇ·ñÐèÒªµ½¸¶ (0-²»ÐèÒª, 1-ÐèÒª): \n");
	printf("%d\n",(*now).ifCollection);
	if((*now).ifCollection){
		printf("ÔËÊä·ÑÓÃ: \n");
		printf("%lf\n",(*now).shipping_fee);
	}printf("5.°ü¹ü×´Ì¬ (1-Õý³£, 2-Ëð»µ, 3-Î¥½ûÆ·): \n");
	printf("%d\n",(*now).package_status);
	
	if(if_id==1){
		printf("6.°ü¹üÐòÁÐºÅ: \n");
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
	
	FILE *file = fopen("received_packages.txt", "r");
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
		fscanf(file,"%lf",&(*now).shipping_fee);
		fscanf(file,"%d",&(*now).package_status);
		fscanf(file,"%s",(*now).package_id);
		
		(*lst).next=now;
		lst=now;
		(*lst).next=NULL;
	}
	
	fclose(file);
	
	return head;
}

void save_package_r(struct package_r* head){
	FILE *file = fopen("received_packages.txt", "w");
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
		}else{
			fprintf(file," 0");
		}fprintf(file," %d",(*now).package_status);
		fprintf(file," %s",(*now).package_id);
		
		if((*now).package_id[0]=='S'){
			break;
		}fprintf(file,"\n");
		
		lst=now;
	}
	
	fclose(file);
}

void package_r_original_start(){
	
	struct package_r *head=load_package_r();
	
	int op;
	
	for(; ;){
		
		printf("\n========== ÊÕ¼þ°ü¹ü¹ÜÀíÏµÍ³ ==========\n");
		printf("1. Ìí¼ÓÊÕ¼þ°ü¹ü\n");
		printf("2. ²éÑ¯ÊÕ¼þ°ü¹ü\n");
		printf("3. ±£´æÊÕ¼þ°ü¹ü\n");
		printf("4. É¾³ýÖ¸¶¨°ü¹ü\n");
		printf("0. ·µ»ØÖ÷²Ëµ¥\n");
		printf("=====================================\n");	
		printf("ÇëÊäÈëÄúµÄÑ¡Ôñ: ");
		
		scanf("%d",&op);
		
		if(op==0){
			save_package_r(head);
			free_package_r(head);
			break;
		}
		
		if(op==1){
			
			struct package_r* now=(struct package_r*)malloc(sizeof(struct package_r));
			printf("1.ÇëÊäÈë°ü¹üËùÊôÓÃ»§µç»°: \n");
			scanf("%s",(*now).phone_number);
			printf("2.ÇëÊäÈë°ü¹üÌå»ý (Á¢·½ÀåÃ×): \n");
			scanf("%lf",&(*now).volume);
			printf("3.ÇëÊäÈë°ü¹üÀàÐÍ (1-ÎÄ¼þ, 2-ÉúÏÊ, 3-Ò×ËéÆ·, 4-¼Òµç, 5-Î£ÏÕÆ·): \n");
			scanf("%d",&(*now).package_type);
			printf("4.ÊÇ·ñÐèÒªµ½¸¶ (0-²»ÐèÒª, 1-ÐèÒª): \n");
			scanf("%d",&(*now).ifCollection);
			if((*now).ifCollection){
				printf("ÇëÊäÈëÔËÊä·ÑÓÃ: \n");
				scanf("%lf",&(*now).shipping_fee);
			}printf("5.ÇëÊäÈë°ü¹ü×´Ì¬ (1-Õý³£, 2-Ëð»µ, 3-Î¥½ûÆ·): \n");
			scanf("%d", &(*now).package_status);
			
			char jdg[22];
			for(; ;){
				
				printf("\n°ü¹üÐÅÏ¢Õ¹Ê¾ÈçÏÂ: \n\n");
				show_package_r(now,0);
				printf("\nÇë×ÐÏ¸ºË¶Ô°ü¹üÐÅÏ¢Â¼ÈëÊÇ·ñÕýÈ· Y/N\n");
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
						printf("ÇëÊäÈëÏ£Íû¸ü¸ÄÏîµÄÐòºÅ (ÊäÈë\"0\"ÒÔ·ÅÆú±¾´ÎÐÞ¸Ä):\n");	
						int lsl;
						scanf("%d",&lsl);
						if(lsl==1){
							printf("ÇëÖØÐÂÊäÈë°ü¹üËùÊôÓÃ»§µç»°: \n");
							scanf("%s",(*now).phone_number);
						}if(lsl==2){
							printf("ÇëÖØÐÂÊäÈë°ü¹üÌå»ý (Á¢·½ÀåÃ×): \n");
							scanf("%lf",&(*now).volume);
						}if(lsl==3){
							printf("ÇëÖØÐÂÊäÈë°ü¹üÀàÐÍ (1-ÎÄ¼þ, 2-ÉúÏÊ, 3-Ò×ËéÆ·, 4-¼Òµç, 5-Î£ÏÕÆ·): \n");
							scanf("%d",&(*now).package_type);
						}if(lsl==4){
							printf("ÇëÖØÐÂÊäÈëÊÇ·ñÐèÒªµ½¸¶ (0-²»ÐèÒª, 1-ÐèÒª): \n");
							scanf("%d",&(*now).ifCollection);
							if((*now).ifCollection){
								printf("ÇëÖØÐÂÊäÈëÔËÊä·ÑÓÃ: \n");
								scanf("%lf",&(*now).shipping_fee);
							}
						}if(lsl==5){
							printf("ÇëÖØÐÂÊäÈë°ü¹ü×´Ì¬ (1-Õý³£, 2-Ëð»µ, 3-Î¥½ûÆ·): \n");
							scanf("%d", &(*now).package_status);
						}
					}else{
						printf("ÊäÈë\"Y\"ÒÔ±íÊ¾È·ÈÏ, \"N\"ÒÔ±íÊ¾ÐèÒªÐÞ¸Ä°ü¹üÐÅÏ¢. \n");
					}
				}
			}
			
			(*now).next=(*head).next;
			(*head).next=now;
			
			get_package_r_id(now);
			
			printf("ÊÕ¼þ°ü¹üÌí¼Ó³É¹¦£¡\n");
			printf("ÒÑ×Ô¶¯ÎªÄú·ÖÅä°ü¹üÐòÁÐºÅ£º\n%s\n",(*now).package_id);
			save_package_r(head);
		}
		
		if(op==2){
			
			printf("ÇëÊäÈëÏ£Íû²éÑ¯µÄ°ü¹üÐòÁÐºÅ: \n");
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
				printf("Î´²éÑ¯µ½ÏàÓ¦°ü¹ü, Çë¼ì²éÐòÁÐºÅÆ´Ð´ÊÇ·ñÕýÈ·. \n");
				continue;
			}printf("\n²éÑ¯µ½°ü¹üÐÅÏ¢ÈçÏÂ: \n\n");
			
			show_package_r(fin,1);
			
		}
		
		if(op==3){
			save_package_r(head);
			printf("ÊÕ¼þ°ü¹ü±£´æ³É¹¦£¡\n");
		}
		
		if(op==4){
			
			printf("ÇëÊäÈëÏ£Íû²éÑ¯µÄ°ü¹üÐòÁÐºÅ: \n");
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
				printf("Î´²éÑ¯µ½ÏàÓ¦°ü¹ü, Çë¼ì²éÐòÁÐºÅÆ´Ð´ÊÇ·ñÕýÈ·. \n");
				continue;
			}
			
			printf("ÇëÉ÷ÖØÑ¡Ôñ, È·ÈÏÉ¾³ý´Ë°ü¹üÂð? Y/N\n");
			
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
					printf("°ü¹üÒÑÉ¾³ý. \n");
					save_package_r(head);
					break;
				}else{
					if(strlen(p)==1&&(*p)=='N'){
						printf("²Ù×÷ÒÑÈ¡Ïû. \n");
						break;
					}else{
						printf("ÊäÈë\"Y\"ÒÔ±íÊ¾È·ÈÏÉ¾³ý´Ë°ü¹ü, \"N\"ÒÔÈ¡ÏûÉ¾³ý²Ù×÷. \n");
					}
				}
			}
			
		}
	}
	
}
