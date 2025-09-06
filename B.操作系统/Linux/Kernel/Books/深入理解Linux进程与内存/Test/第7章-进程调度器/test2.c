#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int pipes[20][3];
char buffer[10];
int running = 1;
pthread_t threads[20];  // 存储线程 ID 的数组

// 为实验建立管道
void init()
{
    int i = 20;
    while(i--)
    {
        if(pipe(pipes[i]) < 0) {
            printf("failed to create pipe");
            exit(1);
        }
        pipes[i][2] = i;
    }
}

// 关闭所有管道
void distroy()
{
    int i = 20;
    while(i--)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}

// 测试管道开销
double pipe_test()
{
    int i = 20000;
    struct timeval start, end;
    gettimeofday(&start, NULL);
    while(i--)
    {
        if(write(pipes[0][1], buffer, 10) == -1) {
            printf("failed to write to pipe 1");
            exit(1);
        }
        read(pipes[0][0], buffer, 10);
    }
    gettimeofday(&end, NULL);
    return (double)(1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec)/20000;
}

void *thread_func(void *arg)
{
    int pos = ((int *)arg)[2];
    int in = pipes[pos][0];
    int to = pipes[(pos + 1) % 20][1];
    while(running)
    {
        if(read(in, buffer, 10) <= 0) {
            printf("failed to read from pipe 2");
            // 如果读取失败或者管道关闭，退出线程
            break;
        }
        if(write(to, buffer, 10) == -1) {
            printf("failed to write to pipe 2");
            break;
        }
    }
    return NULL;
}

double thread_switch_test()
{
    int i = 20;
    struct timeval start, end;

    // 创建线程，从索引 1 开始（跳过索引 0）
    while(--i)
    {
        if(pthread_create(&threads[i], NULL, thread_func, (void *)pipes[i]) != 0) {
            printf("failed to create thread %d\n", i);
            exit(1);
        }
    }

    i = 10000;
    gettimeofday(&start, NULL);
    while(i--)
    {
        if(write(pipes[1][1], buffer, 10) == -1) {
            printf("failed to write to pipe 3");
            exit(1);
        }
        read(pipes[0][0], buffer, 10);
    }
    gettimeofday(&end, NULL);

    // 停止所有线程
    running = 0;

    // 向第一个线程发送一个消息以唤醒它，让它检查 running 标志
	if(write(pipes[1][1],buffer,10) == -1){
            printf("failed to write to pipe 4");
        exit(1);
    }
    // 等待所有线程退出
    for(int j = 1; j < 20; j++) {
        if(pthread_join(threads[j], NULL) != 0) {
            printf("failed to join thread %d\n", j);
        }
    }

    return (double)(1000000*(end.tv_sec-start.tv_sec) + end.tv_usec-start.tv_usec)/10000/20;
}

int main()
{
    init();

    // 测试管道开销
    double pipe_us = pipe_test();
    printf("管道开销：%6.6f us\n", pipe_us);

    // 测试线程上下文切换开销
    double switch_us = thread_switch_test();
    printf("线程切换开销（含管道开销）：%6.6f us\n", switch_us);

    // 每次线程切换开销
    printf("线程切换开销（不含管道开销）：%6.6f us\n", switch_us - pipe_us);

    distroy();
    exit(0);
}
