#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void print_stack_info(const char* stage) {
    char cmd[100];
    printf("\n=== %s ===\n", stage);
    snprintf(cmd, sizeof(cmd), "cat /proc/%d/maps | grep '\\[stack\\]'", getpid());
    system((const char *)cmd);

    // 也查看总的虚拟内存使用
    snprintf(cmd, sizeof(cmd), "cat /proc/%d/status | grep VmStk", getpid());
    system((const char *)cmd);
}

void less() {
    // 使用少量栈空间
    char small_buffer[1024];
    small_buffer[0] = 'A';
    print_stack_info("使用 1KB 栈后");
}

void more() {
    // 使用更多栈空间
    char large_buffer[1000*1024];
    large_buffer[0] = 'B';
    print_stack_info("使用 1000KB 栈后");
}


int main() {
    print_stack_info("程序启动时");

    less();
    more();

    return 0;
}
