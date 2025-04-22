#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void warning() {
    const char *receivedFilePath = "../output/received_packages.txt";
    const char *tempFilePath = "../output/temp.txt";
    const char *idBoxFilePath = "../output/id_box.txt";

    FILE *receivedFile = fopen(receivedFilePath, "r");
    FILE *tempFile = fopen(tempFilePath, "w");
    FILE *idBoxFile = fopen(idBoxFilePath, "a");

    if (!receivedFile) {
        perror("无法打开 received_packages.txt");
        return;
    }
    if (!tempFile) {
        perror("无法创建临时文件");
        fclose(receivedFile);
        return;
    }
    if (!idBoxFile) {
        perror("无法打开 id_box.txt");
        fclose(receivedFile);
        fclose(tempFile);
        return;
    }

    // 读取文件内容并统计行数
    char line[256];
    int lineCount = 0;
    while (fgets(line, sizeof(line), receivedFile)) {
        lineCount++;
    }

    // 计算需要保留的行数
    int linesToKeep = lineCount / 2;

    // 重置文件指针并写入需要保留的行
    rewind(receivedFile);
    int currentLine = 0;
    while (fgets(line, sizeof(line), receivedFile)) {
        currentLine++;
        if (currentLine <= linesToKeep || currentLine == lineCount) {
            // 保留的行写入临时文件
            fputs(line, tempFile);
        } else {
            // 删除的行提取包裹序列号并写入 id_box.txt
            char packageID[9] = {0};
            sscanf(line, "%*s %*s %*s %*s %*s %*s %8s %*s", packageID); // 提取第7个字段
            int tmp = 0;
            tmp = packageID[1] - '0';
            tmp *= 10;
            tmp += packageID[2] - '0';
            tmp -= 1;
            tmp *= 10;
            tmp += packageID[4] - '0';
            tmp *= 10;
            tmp += packageID[5] - '0';
            tmp *= 10;
            tmp += packageID[6] - '0';
            tmp *= 10;
            tmp += packageID[7] - '0';
            fprintf(idBoxFile, " %d", tmp);
        }
    }

    fclose(receivedFile);
    fclose(tempFile);
    fclose(idBoxFile);

    // 替换原文件
    if (remove(receivedFilePath) != 0) {
        perror("无法删除原文件");
        return;
    }
    if (rename(tempFilePath, receivedFilePath) != 0) {
        perror("无法重命名临时文件");
        return;
    }

    printf("已成功删除最早的一半库存，并将包裹序列号归还到 id_box.txt。\n");
}


void load_and_initialize_huise() {
    const char *receivedFilePath = "received_packages.txt";
    const char *huiseFilePath = "huise.txt";

    // 打开 received_packages.txt 文件
    FILE *receivedFile = fopen(receivedFilePath, "r");
    if (!receivedFile) {
        perror("无法打开 received_packages.txt");
        return;
    }

    // 打开 huise.txt 文件以写入模式
    FILE *huiseFile = fopen(huiseFilePath, "w");
    if (!huiseFile) {
        perror("无法创建 huise.txt");
        fclose(receivedFile);
        return;
    }

    // 初始化 countbegin 数组
    int countbegin[367] = {0}; // 用于统计到达时间，假设最大到达时间为 366 天

    char line[256];
    int lineCount = 0;

    // 统计行数
    while (fgets(line, sizeof(line), receivedFile)) {
        lineCount++;
    }

    // 重置文件指针
    rewind(receivedFile);

    int currentLine = 0;
    while (fgets(line, sizeof(line), receivedFile)) {
        currentLine++;

        // 跳过最后一行
        if (currentLine == lineCount) {
            break;
        }

        // 解析第八个字段（到达时间）
        int arrivalDay = 0;
        sscanf(line, "%*s %*s %*s %*s %*s %*s %*s %d", &arrivalDay);

        // 更新 countbegin 数组
        if (arrivalDay >= 1 && arrivalDay <= 366) {
            countbegin[arrivalDay]++;
        }
    }

    fclose(receivedFile);

    // 将 countbegin 数组写入 huise.txt
    for (int i = 0; i <= 366; i++) {
        fprintf(huiseFile, "%d\n", countbegin[i]);
    }

    fclose(huiseFile);

    printf("已根据 received_packages.txt 初始化 huise.txt。\n");
}