#include <linux/bpf.h>

#define SEC(NAME) __attribute__((section(NAME), used))

SEC("tracepoint/syscalls/sys_enter_execve")

int bpf_prog(void *ctx) {
    char msg[] = "Hello, BPF World";

    // tracing log in /sys/kernel/debug/tracing/trace_pipe.
    bpf_trace_printk(msg, sizeof(msg));

    return 0;
}

char _license[] SEC("license") = "GPL";
