from bcc import BPF

bpf_source = """
int trace_bpf_prog_load(void *ctx) {
  char comm[16];
  bpf_get_current_comm(&comm, sizeof(comm));

  bpf_trace_printk("%s is loading a BPF program", comm);
  return 0;
}
"""

bpf = BPF(text=bpf_source)
# bpf trace event have been removed in kernel version 4.18
# (trace/event/bpf.h not found after this version)
# https://git.kernel.org/pub/scm/linux/kernel/git/bpf/bpf-next.git/commit/?id=4d220ed0f8140c478ab7b0a14d96821da639b646
bpf.attach_tracepoint(tp="bpf:bpf_prog_load", fn_name="trace_bpf_prog_load")
bpf.trace_print()
