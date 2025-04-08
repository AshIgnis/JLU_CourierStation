#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sendPackage.h"
#include "structure.h"

/*

1.¼Ä¼þ
2.²éÑ¯ÒÔÍù°ü¹ü
0.ÍË³ö

*/

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
	
	printf("1.ÊÕ¼þÈËÐÕÃû: \n");
	printf("%s\n",(*now).receiver_name);
	printf("2.ÊÕ¼þÈËµØÖ·: \n");
	printf("%s\n",(*now).receiver_address);	
	printf("3.°ü¹üÌå»ý (Á¢·½ÀåÃ×): ");
	printf("%.3lf\n",(*now).volume);
	printf("4.°ü¹üÀàÐÍ (1-ÎÄ¼þ, 2-ÉúÏÊ, 3-Ò×ËéÆ·, 4-¼Òµç, 5-Î£ÏÕÆ·): ");
	printf("%d\n",(*now).package_type);
	printf("5.ÊÇ·ñÐèÒªµ½¸¶ (0-²»ÐèÒª, 1-ÐèÒª): ");
	printf("%d\n",(*now).ifCollection);
	if((*now).ifCollection){
		printf("ÔËÊä·ÑÓÃ: ");
		printf("%.2lf\n",(*now).shipping_fee);
	}printf("6.°ü¹ü×´Ì¬ (1-Õý³£, 2-Ëð»µ, 3-Î¥½ûÆ·): ");
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
			printf("°ü¹ü%3d: \n",jd);
			show_package_s(now);
			printf("\n");
		}lst=now;
	}
	
	if(jd!=0){
		printf("ÄúµÄ¼Ä¼þÐÅÏ¢Õ¹Ê¾ÈçÉÏ.\n");
	}else{
		printf("Äú»¹Î´Ôø½øÐÐ¼Ä¼þ.\n");
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
			fprintf(file," %lf",(*now).shipping_fee);
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
	char nm[22];
	for(int i=0;i<13;i++){
		nm[i]=t[i];
	}nm[13]='\0';
	
	int op;
	
	for(; ;){
		
		printf("\n========== ¼Ä¼þ°ü¹ü¹ÜÀíÏµÍ³ ==========\n");
		printf("1. Ìí¼Ó¼Ä¼þ°ü¹ü\n");
		printf("2. ²éÑ¯¼Ä¼þ°ü¹ü\n");
		printf("3. ±£´æ¼Ä¼þ°ü¹ü\n");
		printf("0. ·µ»ØÖ÷²Ëµ¥\n");
		printf("=====================================\n");	
		printf("ÇëÊäÈëÄúµÄÑ¡Ôñ: ");
		
		scanf("%d",&op);
		
		if(op==0){
			
			save_package_s(head);
			free_package_s(head);
			
			break;
		}
		
		if(op==1){
			
			struct package_s* now=(struct package_s*)malloc(sizeof(struct package_s));
			for(int i=0;i<=13;i++){
				(*now).phone_number[i]=nm[i];
			}printf("1.ÇëÊäÈëÊÕ¼þÈËÐÕÃû: \n");
			scanf("%s",(*now).receiver_name);
			printf("2.ÇëÊäÈëÊÕ¼þÈËµØÖ·: \n");
			scanf("%s",(*now).receiver_address);
			printf("3.ÇëÊäÈë°ü¹üÌå»ý (Á¢·½ÀåÃ×): \n");
			scanf("%lf",&(*now).volume);
			printf("4.ÇëÊäÈë°ü¹üÀàÐÍ (1-ÎÄ¼þ, 2-ÉúÏÊ, 3-Ò×ËéÆ·, 4-¼Òµç, 5-Î£ÏÕÆ·): \n");
			scanf("%d",&(*now).package_type);
			printf("5.ÊÇ·ñÐèÒªµ½¸¶ (0-²»ÐèÒª, 1-ÐèÒª): \n");
			scanf("%d",&(*now).ifCollection);
			printf("6.ÇëÊäÈë°ü¹ü×´Ì¬ (1-Õý³£, 2-Ëð»µ, 3-Î¥½ûÆ·): \n");
			scanf("%d", &(*now).package_status);
			
			char jdg[22];
			for(; ;){
				
				printf("\n°ü¹üÐÅÏ¢Õ¹Ê¾ÈçÏÂ: \n\n");
				show_package_s(now);
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
							printf("ÇëÖØÐÂÊäÈëÊÕ¼þÈËÐÕÃû: \n");
							scanf("%s",(*now).receiver_name);
						}if(lsl==2){ 
							printf("ÇëÖØÐÂÊäÈëÊÕ¼þÈËµØÖ·: \n");
							scanf("%s",(*now).receiver_address);
						}if(lsl==3){
							printf("ÇëÖØÐÂÊäÈë°ü¹üÌå»ý (Á¢·½ÀåÃ×): \n");
							scanf("%lf",&(*now).volume);
						}if(lsl==4){
							printf("ÇëÖØÐÂÊäÈë°ü¹üÀàÐÍ (1-ÎÄ¼þ, 2-ÉúÏÊ, 3-Ò×ËéÆ·, 4-¼Òµç, 5-Î£ÏÕÆ·): \n");
							scanf("%d",&(*now).package_type);
						}if(lsl==5){
							printf("ÇëÖØÐÂÊäÈëÊÇ·ñÐèÒªµ½¸¶ (0-²»ÐèÒª, 1-ÐèÒª): \n");
							scanf("%d",&(*now).ifCollection);
						}if(lsl==6){
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
			
			printf("¼Ä¼þ°ü¹üÌí¼Ó³É¹¦£¡\n");
			save_package_s(head);
		}
		
		if(op==2){
			
			find_package_s(nm,head);
			
		}
		
		if(op==3){
			save_package_s(head);
			printf("ÊÕ¼þ°ü¹ü±£´æ³É¹¦£¡\n");
		}
	}
	
	return;
}
