from bcc import BPF
import os

bpf_source = """
BPF_HASH(cache, u64, u64);

int trace_start_time(struct pt_regs *ctx) {
  u64 pid = bpf_get_current_pid_tgid();
  u64 start_time_ns = bpf_ktime_get_ns();
  cache.update(&pid, &start_time_ns);
  return 0;
}
"""

bpf_source += """
int print_duration(struct pt_regs *ctx) {
  u64 pid = bpf_get_current_pid_tgid();
  u64 *p = cache.lookup(&pid);
  if (!p) {
    return 0;
  }

  u64 start_time_ns = *p;

  u64 duration_ns = 0; // bpf_ktime_get_ns() - start_time_ns;
  bpf_trace_printk("Function call duration: %d", duration_ns);
  return 0;
}
"""

file_path = os.path.realpath(__file__)
file_path = os.path.dirname(file_path)
binary_file = file_path + "/../4.uprobes/hello-bpf-go.out"
print(binary_file)
bpf = BPF(text=bpf_source)
bpf.attach_uprobe(name=binary_file, sym="main.main",
                  fn_name="trace_start_time")
bpf.attach_uretprobe(name=binary_file, sym="main.main",
                     fn_name="print_duration")
bpf.trace_print()
