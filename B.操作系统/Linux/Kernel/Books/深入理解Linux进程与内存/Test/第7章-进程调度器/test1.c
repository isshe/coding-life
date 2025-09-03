#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define EXEC_COUNT 10000

// 计算两个 timeval 之间的微秒差值
long long timeval_diff_us(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) * 1000000LL + (end->tv_usec - start->tv_usec);
}

int main()
{
    int x, i, fd[2], p[2];
    char send = 'h';
    char receive;
    pipe(fd);
    pipe(p);
    struct timeval start_tv, end_tv;
    struct sched_param param;
    param.sched_priority = 0;

    while ((x = fork()) == -1);

    if (x == 0) {
        // 子进程
        // sched_setscheduler(getpid(), SCHED_FIFO, &param);

        // 记录开始时间
        gettimeofday(&start_tv, NULL);
        printf("开始测试时间：%ld s, %ld us\n", start_tv.tv_sec, start_tv.tv_usec);

        for (i = 0; i < EXEC_COUNT; i++) {
            read(fd[0], &receive, 1);
            write(p[1], &send, 1);
        }

        // 记录结束时间
        gettimeofday(&end_tv, NULL);
        printf("子进程结束时间：%ld s, %ld us\n", end_tv.tv_sec, end_tv.tv_usec);

        // 计算并打印子进程的耗时信息
        long long total_us = timeval_diff_us(&start_tv, &end_tv);
        printf("子进程总耗时：%lld 微秒\n", total_us);

        exit(0);
    }
    else {
        // 父进程
        // sched_setscheduler(getpid(), SCHED_FIFO, &param);

        // 记录开始时间
        gettimeofday(&start_tv, NULL);

        for (i = 0; i < EXEC_COUNT; i++) {
            write(fd[1], &send, 1);
            read(p[0], &receive, 1);
        }

        // 记录结束时间
        gettimeofday(&end_tv, NULL);
        printf("父进程结束时间：%ld s, %ld us\n", end_tv.tv_sec, end_tv.tv_usec);

        // 等待子进程结束
        wait(NULL);

        // 计算总耗时和平均每次上下文切换耗时
        long long total_us = timeval_diff_us(&start_tv, &end_tv);
        double avg_context_switch_us = (double)total_us / (EXEC_COUNT * 2);

        printf("\n=== 上下文切换性能测试结果 ===\n");
        printf("总测试次数：%d 次循环\n", EXEC_COUNT);
        printf("总上下文切换次数：%d 次\n", EXEC_COUNT * 2);
        printf("总耗时：%lld 微秒 (%.3f 毫秒)\n", total_us, total_us / 1000.0);
        printf("平均每次上下文切换耗时：%.3f 微秒\n", avg_context_switch_us);
        printf("平均每次上下文切换耗时：%.6f 毫秒\n", avg_context_switch_us / 1000.0);
    }

    // 关闭管道
    close(fd[0]);
    close(fd[1]);
    close(p[0]);
    close(p[1]);

    return 0;
}
