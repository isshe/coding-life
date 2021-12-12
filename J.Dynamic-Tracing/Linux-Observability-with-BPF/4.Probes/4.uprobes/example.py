from bcc import BPF
import os

file_path = os.path.realpath(__file__)
file_path = os.path.dirname(file_path)

bpf_source = """
int trace_main(struct pt_regs *ctx) {
  u64 pid = bpf_get_current_pid_tgid();
  bpf_trace_printk("New hello-bpf process running with PID: %d", pid);
  return 0;
}
"""

bpf = BPF(text=bpf_source)
print("file_path = ", file_path)
bpf.attach_uprobe(name="./hello-bpf-go.out",
                  sym="main.main", fn_name="trace_main")
bpf.trace_print()
# me = os.getpid()
# while 1:
#     try:
#         (task, pid, cpu, flags, ts, msg) = bpf.trace_fields()
#     except ValueError:
#         continue
#     if pid == me or msg == "":
#         continue
#     print("%-18.9f %-16s %-6d %s" % (ts, task, pid, msg))
