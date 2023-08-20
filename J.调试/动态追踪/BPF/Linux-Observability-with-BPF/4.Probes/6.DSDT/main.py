#!/usr/bin/python3
from bcc import BPF, USDT

# \\n is important
bpf_source = \
    """
#include <uapi/linux/ptrace.h>
int trace_binary_exec(struct pt_regs *ctx) {
  u64 pid = bpf_get_current_pid_tgid();

  bpf_trace_printk("New hello process running with PID: %d\\n", pid);
  return 0;
}
"""

usdt = USDT(path="./main.out")
usdt.enable_probe(probe="probe-main", fn_name="trace_binary_exec")
bpf = BPF(text=bpf_source, usdt_contexts=[usdt])
bpf.trace_print()
