#include <stdio.h>

void print_stack_info() {
    register void* rbp asm("rbp");
    register void* rsp asm("rsp");

    printf("当前 rbp 寄存器值：%p\n", rbp);
    printf("当前 rsp 寄存器值：%p\n", rsp);
    printf("rbp 指向的内存内容 (上一个 rbp): %p\n", *(void**)rbp);
}

void func_c() {
    printf("=== func_c ===\n");
    print_stack_info();
}

void func_b() {
    printf("=== func_b ===\n");
    print_stack_info();
    func_c();
}

void func_a() {
    printf("=== func_a ===\n");
    print_stack_info();
    func_b();
}

int main() {
    printf("=== main ===\n");
    print_stack_info();
    func_a();
}
