from bcc import BPF

bpf_source = """
#include <uapi/linux/ptrace.h>

int ret_sys_execve(struct pt_regs *ctx) {
  int return_value;
  char comm[16];
  bpf_get_current_comm(&comm, sizeof(comm));

  // PT_REGS_RC是一个宏，它将从这个特定上下文的BPF寄存器中读取返回值。
  return_value = PT_REGS_RC(ctx);

  bpf_trace_printk("program: %s, return: %d\\n", comm, return_value);
  return 0;
}
"""

bpf = BPF(text=bpf_source)
execve_function = bpf.get_syscall_fnname("execve")
bpf.attach_kretprobe(event=execve_function, fn_name="ret_sys_execve")
bpf.trace_print()
