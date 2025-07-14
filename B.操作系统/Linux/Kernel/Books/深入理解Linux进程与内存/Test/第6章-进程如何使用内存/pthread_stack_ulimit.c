#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

static volatile int max_depth = 0;

void segfault_handler(int sig) {
    // 使用异步安全的函数
    char msg[100];
    int len = snprintf(msg, sizeof(msg), "栈溢出！最大递归深度：%d\n", max_depth);
    write(STDOUT_FILENO, msg, len);
    _exit(1);
}

void setup_signal_handler() {
    // 为信号处理器分配独立的栈空间
    stack_t ss;
    ss.ss_sp = malloc(SIGSTKSZ);
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;

    if (sigaltstack(&ss, NULL) == -1) {
        perror("sigaltstack");
        exit(1);
    }

    struct sigaction sa;
    sa.sa_handler = segfault_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;  // 使用备用栈

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

void recursive_function(int depth) {
    char buffer[1024];
    max_depth = depth;
    buffer[0] = (char)(depth & 0xFF);
    recursive_function(depth + 1);
}

void* thread_function(void* arg) {
    setup_signal_handler();
    printf("线程开始测试栈溢出...\n");
    recursive_function(1);
    return NULL;
}

int main() {
    struct rlimit rl;
    getrlimit(RLIMIT_STACK, &rl);
    printf("当前栈限制：%ld KB\n", rl.rlim_cur / 1024);

    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);
    pthread_join(thread, NULL);

    return 0;
}
