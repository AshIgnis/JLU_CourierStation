#include <stdio.h>
#include <math.h>
// 定义一个全局数组用于存储每天的包裹数量
int count[366] = {0}; // 假设最多存储365天的数据
// 函数声明
void load_data_from_file();
void save_data_to_file();
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
    int* get_array(int real_time) {
        return analysis(real_time);
    }

    // 获取数组长度
    size_t get_array_length(int real_time) {
        return real_time + 1; // 返回数组长度
    }

    // 加载数据
    void load(int real_time) {
        load_data_from_file(real_time);
    }

#ifdef __cplusplus
}
#endif

// 从文件 "huise.txt" 中读取数据并初始化 count 数组
// 在分析函数中调用文件加载函数
void load_data_from_file(int real_time) {
    FILE* file = fopen("huise.txt", "r");
    if (file == NULL) {
        printf("无法打开文件 huise.txt\n");
        return;
    }
    for (int i = 0; i <= real_time && fscanf(file, "%d", &count[i]) == 1; ++i);
    fclose(file);
}

// 自定义四舍五入函数
int custom_round(double num) {
	return (int)(num + 0.5);
}

// 保存数据到文件
void save_data_to_file(int real_time) {
    // 动态生成文件名，例如 "huise_real_time_7.txt"
    char filename[50];
    sprintf(filename, "huise_real_time_%d.txt", real_time);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法保存文件 %s\n", filename);
        return;
    }
    for (int i = 0; i <= real_time + 1; i++) {
        fprintf(file, "%d\n", count[i]);
    }
    fclose(file);
    // printf("数据已保存到文件: %s\n", filename);
}

// 自定义数据分析函数
int* analysis(int real_time){
	// 第一个数据分析预测函数(灰色预测)，适用于real_time较小(不超过15天)情况
    // 读取每天的包裹数量
	double x0[366] = { 0 }; // 原始序列，real_time不超过365天
    for (int i = 0; i <= real_time; i++)
    {
        x0[i] = count[i]; // count数组存储日包裹数
    }
	//定义累加数组x1，累加数组大小为real_time
	double x1[366] = { 1 };
	//x1累加数组的第一个数就是x0原始数组的第一个数
	x1[0] = x0[0];
	//x1累加数组除去第一个数的后面数
	for (int i = 1;i <= real_time;i++)
	{
		x1[i] = x0[i] + x1[i - 1];    //x1的第i个数就是x0的第i个数和第i-1个数之和
	}
	//创建矩阵B，大小为real_time*2
	double B[real_time][2];
	//对矩阵B进行赋值
	for (int i = 0; i < real_time; ++i) {
		B[i][0] = -(x1[i] + x1[i + 1]) / 2.0;
		B[i][1] = 1.0;
	}
	//创建转置矩阵Bt，大小为2*real_time
	double Bt[2][real_time];
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
	double t2[2][real_time];
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
	double Y[real_time][1];
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
	save_data_to_file(real_time); // 新增保存操作
	return count;
}