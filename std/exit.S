.include "syscalls.S.pp"
.text
.global abi_sysv_amd64_exit

abi_sysv_amd64_exit:
    movq $SYS_exit, %rax
    syscall  
