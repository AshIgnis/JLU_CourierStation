#include <stdio.h>
#include <math.h>
// 定义一个全局数组用于存储每天的包裹数量
int count_day[400] = {0};// 预计存储365个数据
int count_week[60] = {0};//预计存储53个数据
int count_month[15] = {0};//预计存储12个数据
// 函数声明
void load_data_from_file();
void set_count_week(int real_time);
void set_count_month(int real_time);
int day_to_week(int day);
int day_to_month(int day);
void save_data_to_file_day();
void save_data_to_file_week();
void save_data_to_file_month();
int* analysis();
void set_real_time(int value);
int* get_array();
size_t get_array_length();
void load();

// 暴露的接口
#ifdef __cplusplus
extern "C" {
#endif

    // 获取数组指针
    int* get_array(int real_time,int choice_son4) {
		int* p = NULL;
		switch (choice_son4)
		{
		case 1:
			p = analysis(real_time,count_day);
			save_data_to_file_day(real_time);
			return p;
			break;
		case 2:
			int week = day_to_week(real_time) - 1;
			p = analysis(week,count_week);
			save_data_to_file_week(week);
			return p;
			break;
		case 3:
			int month = day_to_month(real_time) - 1;
			p = analysis(month,count_month);
			save_data_to_file_month(month);
			return p;
			break;
		default:
			return p;
			break;
		}
    }

    // 获取数组长度
    size_t get_array_length(int real_time,int choice_son4) {
		switch (choice_son4)
		{
		case 1:
        	return real_time + 1; // 返回数组长度
			break;
		case 2:
			return day_to_week(real_time) + 1;
			break;
		case 3:
			return day_to_month(real_time) + 1;
			break;
		default:
			return 0;
			break;
		}
    }

    // 加载数据
    void load(int real_time,int choice_son4) {
		switch (choice_son4)
		{
		case 1:
			load_data_from_file(real_time);
			break;
		case 2:
			load_data_from_file(real_time);
			set_count_week(real_time);
			break;
		case 3:
			load_data_from_file(real_time);
			set_count_month(real_time);
			break;
		default:
			break;
		}
    }

#ifdef __cplusplus
}
#endif

// 从文件 "huise.txt" 中读取数据并初始化 count_day 数组
void load_data_from_file(int real_time) {
    FILE* file = fopen("huise.txt", "r");
    if (file == NULL) {
        printf("无法打开文件 huise.txt\n");
        return;
    }
    for (int i = 0; i <= real_time && fscanf(file, "%d", &count_day[i]) == 1; ++i);
    fclose(file);
}

// 天数转化周数函数
int day_to_week(int day) {
	return (day + 6) / 7; // 向上取整
}

// 天数转化月份函数,假设统计时以30天为一个月
int day_to_month(int day) {
	return (day + 29) / 30; // 向上取整
}

// 开辟count_week数组
void set_count_week(int real_time){
	// 清零 count_week 数组
    for (int i = 0; i < 60; ++i) {
        count_week[i] = 0;
    }
	int real_week = day_to_week(real_time) - 1;// 实际满七天的周数,测试时可以通过终端改动real_day进而通过函数变动
	for (int i = 0; i < real_week; ++i) {
		for (int j = 1; j <= 7 ; ++j) {
			count_week[i+1] += count_day[i * 7 + j];
		}
	}
}

void set_count_month(int real_time){
	// 清零 count_month 数组
    for (int i = 0; i < 15; ++i) {
        count_month[i] = 0;
    }
	int real_month = day_to_month(real_time) - 1;// 实际满30天的月数,测试时可以通过终端改动real_day进而通过函数变动
	for (int i = 0; i < real_month; ++i) {
		for (int j = 1; j <= 30 ; ++j) {
			count_month[i+1] += count_day[i * 30 + j];
		}
	}
}

// 自定义四舍五入函数
int custom_round(double num) {
	return (int)(num + 0.5);
}

// 保存数据到文件,三种
void save_data_to_file_day(int real_time) {
    // 动态生成文件名
    char filename[50];
    sprintf(filename, "huise_real_time_day_%d.txt", real_time);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法保存文件 %s\n", filename);
        return;
    }
    for (int i = 0; i <= real_time + 1; i++) {
        fprintf(file, "%d\n", count_day[i]);
    }
    fclose(file);
    // printf("数据已保存到文件: %s\n", filename);
}

void save_data_to_file_week(int real_time) {
    // 动态生成文件名
    char filename[50];
    sprintf(filename, "huise_real_time_week_%d.txt", real_time);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法保存文件 %s\n", filename);
        return;
    }
    for (int i = 0; i <= real_time + 1; i++) {
        fprintf(file, "%d\n", count_week[i]);
    }
    fclose(file);
    // printf("数据已保存到文件: %s\n", filename);
}

void save_data_to_file_month(int real_time) {
    // 动态生成文件名
    char filename[50];
    sprintf(filename, "huise_real_time_month_%d.txt", real_time);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法保存文件 %s\n", filename);
        return;
    }
    for (int i = 0; i <= real_time + 1; i++) {
        fprintf(file, "%d\n", count_month[i]);
    }
    fclose(file);
    // printf("数据已保存到文件: %s\n", filename);
}

// 自定义数据分析函数
int* analysis(int real_time,int count[]){
    // 读取每天的包裹数量
	double x0[400] = { 0 }; // 原始序列，real_time不超过365天
    for (int i = 0; i <= real_time; i++)
    {
        x0[i] = count[i]; // count数组存储日包裹数
    }
	//定义累加数组x1，累加数组大小为real_time
	double x1[400] = { 1 };
	//x1累加数组的第一个数就是x0原始数组的第一个数
	x1[0] = x0[0];
	//x1累加数组除去第一个数的后面数
	for (int i = 1;i <= real_time;i++)
	{
		x1[i] = x0[i] + x1[i - 1];    //x1的第i个数就是x0的第i个数和第i-1个数之和
	}
	//创建矩阵B，大小为real_time*2
	double B[400][2];
	//对矩阵B进行赋值
	for (int i = 0; i < real_time; ++i) {
		B[i][0] = -(x1[i] + x1[i + 1]) / 2.0;
		B[i][1] = 1.0;
	}
	//创建转置矩阵Bt，大小为2*real_time
	double Bt[2][400];
	//转置矩阵Bt值为矩阵B的转置
	for (int i = 0; i < real_time; ++i) {
		Bt[0][i] = B[i][0];
		Bt[1][i] = B[i][1];
	}
	//创建矩阵t，作为Bt*B的结果
	double t[2][2];
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < 2;j++) {
			double sum=0.0;
			for (int k = 0;k < real_time;k++) {
				sum+= Bt[i][k] * B[k][j];
			}
			t[i][j] = sum;
		}
	}
	//创建矩阵t1，作为矩阵t的逆矩阵
	double t1[2][2];
	double det = t[0][0] * t[1][1] - t[0][1] * t[1][0];
	if (det > 0.0) {
		t1[0][0] = t[1][1] / det;
		t1[0][1] = -t[1][0] / det;
		t1[1][0] = -t[0][1] / det;
		t1[1][1] = t[0][0] / det;
	}
	else {
		printf("矩阵不可逆");
	}
	//创建矩阵t2，作为t1*Bt的结果
	double t2[2][400];
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < real_time;j++) {
			double sum = 0;
			for (int k = 0;k < 2;k++) {
				sum += t1[i][k] * Bt[k][j];
			}
			t2[i][j] = sum;
		}
	}
	//创建矩阵Y
	double Y[400][1];
	for (int i = 0; i < real_time; ++i) {
		Y[i][0] = x0[i + 1];
	}
	//创建矩阵t3，作为t2*Y的结果
	double t3 [2][1] ;
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < 1;j++) {
			double sum = 0;
			for (int k = 0;k < real_time;k++) {
				sum += t2[i][k] * Y[k][j];
			}
			t3[i][j] = sum;
		}
	}
	//求a
	double a = t3[0][0];
	// printf("a=%.2f", a);
	// printf("\n");
	//求b
	double b = t3[1][0];
	// printf("b=%.2f", b);
	// printf("\n");
	//累加序列第k个预测值
	double m = (x0[0] - b / a) * exp(-a * (real_time)) + b / a;
	//累加序列第k+1个预测值
	double n = (x0[0] - b / a) * exp(-a * (real_time + 1)) + b / a;
	//原始序列第k+1个预测值
	double p = n-m;
	// printf("The estimated number of packages is:%d", custom_round(p));
	// printf("\n");
	count[real_time + 1] = custom_round(p);
	return count;
}