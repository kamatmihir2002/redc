#include <asm/unistd.h>

.text
.global abi_sysv_amd64_exit

abi_sysv_amd64_exit:
    movq $__NR_exit, %rax
    syscall  
