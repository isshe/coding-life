#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("用法：%s <内存大小 (字节)>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n <= 0) {
        printf("内存大小必须为正整数!\n");
        return 1;
    }

    printf("进程 PID: %d\n", getpid());
    printf("正在申请 %d 个整数的内存 (%ld 字节)...\n", n, n * sizeof(int));

    // 申请指定大小的内存
    int* arr = (int*)malloc(n * sizeof(int));

    if (arr == NULL) {
        printf("内存申请失败!\n");
        return 1;
    }

    printf("内存申请成功，开始初始化...\n");

    // 对内存进行访问，以触发缺页中断真正分配物理内存
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    printf("内存初始化完成，进入休眠状态 (2 秒)...\n");
    fflush(stdout);  // 确保输出被刷新

    // 让程序休眠 2 秒方便观察
    sleep(2);

    printf("休眠结束，释放内存...\n");
    // 释放内存
    free(arr);

    return 0;
}
