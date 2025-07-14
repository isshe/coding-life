#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <signal.h>
#include <string.h>

static volatile size_t max_stack_usage = 0;
static void* stack_start = NULL;

void segfault_handler(int sig) {
    char msg[200];
    int len = snprintf(msg, sizeof(msg),
        "栈溢出！最大栈使用量：%.2f MB (%zu bytes)\n",
        max_stack_usage / (1024.0 * 1024.0), max_stack_usage);
    write(STDOUT_FILENO, msg, len);
    _exit(1);
}

void setup_signal_handler() {
    stack_t ss;
    ss.ss_sp = malloc(SIGSTKSZ);
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;
    sigaltstack(&ss, NULL);

    struct sigaction sa;
    sa.sa_handler = segfault_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;
    sigaction(SIGSEGV, &sa, NULL);
}

void recursive_function(int depth) {
    char buffer[1024];  // 每次递归 1KB
    void* current_stack = &buffer;

    // 计算栈使用量
    if (stack_start == NULL) {
        stack_start = current_stack;
    }

    size_t stack_usage = (char*)stack_start - (char*)current_stack;
    if (stack_usage > max_stack_usage) {
        max_stack_usage = stack_usage;
    }

    // 每 1000 次递归打印一次
    if (depth % 1000 == 0) {
        printf("递归深度：%d, 栈使用：%.2f MB\n",
               depth, stack_usage / (1024.0 * 1024.0));
        fflush(stdout);
    }

    buffer[0] = (char)(depth & 0xFF);  // 防止优化
    recursive_function(depth + 1);
}

int main() {
    struct rlimit rl;

    // 显示当前限制
    getrlimit(RLIMIT_STACK, &rl);
    printf("软限制：%ld KB (%.2f MB)\n",
           rl.rlim_cur / 1024, rl.rlim_cur / (1024.0 * 1024.0));
    printf("硬限制：%ld KB (%.2f MB)\n",
           rl.rlim_max / 1024, rl.rlim_max / (1024.0 * 1024.0));

    setup_signal_handler();

    printf("开始栈扩展测试...\n");
    recursive_function(1);

    return 0;
}
