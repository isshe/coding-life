#!/usr/bin/python3
from bcc import BPF, PerfType, PerfSWConfig
import sys
import errno
from time import sleep
import signal


def signal_ignore():
    print("---isshe---: signal_ignore--")
    pass


bpf_source = """
#include <uapi/linux/ptrace.h>
#include <uapi/linux/bpf_perf_event.h>
#include <linux/sched.h>

struct trace_t {
  int stack_id;
};

BPF_HASH(cache, struct trace_t);
BPF_STACK_TRACE(traces, 10000);
"""

bpf_source += """
int collect_stack_traces(struct bpf_perf_event_data *ctx) {
  u32 pid = bpf_get_current_pid_tgid() >> 32;
  if (pid != PROGRAM_PID)
    return 0;

  struct trace_t trace = {
    .stack_id = traces.get_stackid(&ctx->regs, BPF_F_USER_STACK)
  };

  cache.increment(trace);
  return 0;
}
"""
print(sys.argv[1])
program_pid = sys.argv[1]
bpf_source = bpf_source.replace('PROGRAM_PID', program_pid)

print(bpf_source)
bpf = BPF(text=bpf_source)
bpf.attach_perf_event(ev_type=PerfType.SOFTWARE,
                      ev_config=PerfSWConfig.CPU_CLOCK,
                      fn_name='collect_stack_traces',
                      sample_period=0,
                      sample_freq=99)

try:
    sleep(99999999)
except KeyboardInterrupt:
    # signal_ignore = True  # signal.SIG_IGN
    signal.signal(signal.SIGINT, signal_ignore)

cache = bpf.get_table("cache")
traces = bpf.get_table("traces")
for trace, acc in sorted(cache.items(), key=lambda cache: cache[1].value):
    line = []
    if trace.stack_id < 0 and trace.stack_id == -errno.EFAULT:
        line = ['Unknown stack']
    else:
        stack_trace = list(traces.walk(trace.stack_id))
        for stack_address in reversed(stack_trace):
            print("----isshe---: stack_trace = ",
                  bpf.sym(stack_address, int(program_pid)))
            line.extend(bpf.sym(stack_address, int(program_pid)))

    print("----isshe---: line = ", line)

    frame = b";".join(line).decode('utf-8', 'replace')
    print("%s %d" % (frame, acc.value))
