#include <linux/version.h>
#include <linux/ptrace.h>
#include <uapi/linux/bpf.h>
#include "bpf_helpers.h"

#define SEC(NAME) __attribute__((section(NAME), used))

// static int (*bpf_trace_printk)(const char *fmt, int fmt_size,
//                                ...) = (void *)BPF_FUNC_trace_printk;

#define IPV4_FAMILY 1
struct ip_key
{
    union
    {
        __u32 v4_addr;
        __u8 v6_addr[16];
    };

    __u8 family;
};

struct bpf_map_def SEC("maps") counters = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(struct ip_key),
    .value_size = sizeof(uint64_t),
    .max_entries = 100,
    .map_flags = BPF_F_NO_PREALLOC,
};

uint64_t update_counter(uint32_t ipv4)
{

    uint64_t value;
    struct ip_key key = {};

    key.v4_addr = ipv4;
    key.family = IPV4_FAMILY;

    bpf_map_lookup_elem(counters, &key, &value);

    (*value) += 1;
};

SEC("tracepoint/syscalls/sys_enter_execve")
int bpf_prog(void *ctx)
{
    char msg[] = "Hello, BPF World!";
    // bpf_trace_printk(msg, sizeof(msg));
    return 0;
}

char _license[] SEC("license") = "GPL";
