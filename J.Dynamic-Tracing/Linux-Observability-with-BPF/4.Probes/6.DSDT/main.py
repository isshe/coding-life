#!/usr/bin/python3
from bcc import BPF, USDT

bpf_source = """
#include <uapi/linux/ptrace.h>
int trace_binary_exec(struct pt_regs *ctx) {
  u64 pid = bpf_get_current_pid_tgid();
  u64 ts=0;
  bpf_usdt_readarg(1, ctx, &ts);
  bpf_trace_printk("PROBE-HIT (PID: %d), TS: %lu\\n", pid, ts);
  return 0;
}
"""

usdt = USDT(path="./main.out")
usdt.enable_probe(probe="probe-main", fn_name="trace_binary_exec")
bpf = BPF(text=bpf_source, usdt_contexts=[usdt])
bpf.trace_print()
