#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "warehouse_explosion_warning.h"

void warning(){
    const char *filePath = "../output/receivedPackage.txt";
    const char *tempFilePath = "../output/temp.txt";
    FILE *file = fopen(filePath, "r");
    FILE *tempFile = fopen(tempFilePath, "w");

    if (!file) {
        perror("无法打开 receivedPackage.txt");
        return 0;
    }
    if (!tempFile) {
        perror("无法创建临时文件");
        fclose(file);
        return 0;
    }

    // 读取文件内容并统计行数
    char line[105];
    int lineCount = 0;
    while (fgets(line, sizeof(line), file)) {
        lineCount++;
    }

    // 计算需要保留的行数
    int linesToKeep = lineCount / 2;
    // 重置文件指针并写入需要保留的行
    rewind(file);
    int currentLine = 0;
    while (fgets(line, sizeof(line), file)) {
        currentLine++;
        if (currentLine <= linesToKeep || currentLine == lineCount) {
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    // 替换原文件
    if (remove(filePath) != 0) {
        perror("无法删除原文件");
        return 0;
    }
    if (rename(tempFilePath, filePath) != 0) {
        perror("无法重命名临时文件");
        return 0;
    }

    printf("已成功删除行号较大的那一半行。\n");
    return 0;
}