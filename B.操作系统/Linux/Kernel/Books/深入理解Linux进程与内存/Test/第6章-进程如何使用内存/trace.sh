#!/bin/bash

sudo bpftrace -e '
    kprobe:handle_mm_fault {
        printf("=== Page Fault ===\n");
        printf("PID: %d, COMM: %s\n", pid, comm);
        printf("Kernel stack:\n");
        print(kstack);
        printf("User stack:\n");
        print(ustack);
        printf("\n");
    }
'
