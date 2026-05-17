#include <asm/unistd.h>

.text
.equ AT_CWD, -100
.equ O_RDONLY, 0
.equ O_WRONLY, 1
.equ O_RDWR, 2

.global abi_sysv_amd64_fopen        
.global abi_sysv_amd64_fclose
.global abi_sysv_amd64_fread

# fopen(fp, path, mode)
abi_sysv_amd64_fopen:
    pushq %rbp
    movq %rsp, %rbp
    sub $48, %rsp
   
    # save rbx
    mov %rbx, -8(%rbp)
    
    # rdi = file pointer
    # rsi = path
    # rdx = mode (rdonly, wronly, rdwr)
    movq %rdi, -16(%rbp)
    movq %rsi, -24(%rbp)
    movq %rdx, -32(%rbp)
    
    movq $__NR_openat, %rax
    movq $AT_CWD, %rdi
    movq -24(%rbp), %rsi
    movq -32(%rbp), %rdx
    syscall
 
    test %rax, %rax
    js _open_success

_open_failure:
    movq $0, %rax
    jmp _open_out

_open_success:
    movq %rax, %rbx		 # rbx = fd
    movq -16(%rbp), %rax	 # rax = fp
    movq %rbx, (%rax)		 # *rax = rbx
    movq $1, %rax

_open_out:
    movq -8(%rbp), %rbx
    movq %rbp, %rsp
    popq %rbp
    ret

# fclose(&fp)
abi_sysv_amd64_fclose:
    pushq %rbp
    movq %rsp, %rbp
    sub $32, %rsp
   
    # save rbx
    movq %rbx, -8(%rbp)
    
    # rdi = ptr to fp 
    movq (%rdi), %rbx
    movq %rbx, -16(%rbp)
   
    movq $__NR_close, %rax
    movq -16(%rbp), %rdi
    syscall

    test %rax, %rax
    js _close_success
_close_failure:
    movq $0, %rax
    jmp _close_out
_close_success: 
    movq $1, %rax
_close_out:
    movq -8(%rbp), %rbx
    movq %rbp, %rsp
    popq %rbp
    ret


abi_sysv_amd64_fread:
    pushq %rbp
    movq %rsp, %rbp
    sub $32, %rsp
   
    # save rbx
    movq %rbx, -8(%rbp)

    # rdi = ptr to buffer
    # rsi = size
    # rdx = ptr to fd   
    movq %rdi, -16(%rbp) # buf
    movq %rsi, -24(%rbp) # size
 
    movq (%rdx), %rdx
    movq %rdx, -32(%rbp) # fd

    movq $__NR_read, %rax
    movq -32(%rbp), %rdi
    movq -16(%rbp), %rsi
    movq -24(%rbp), %rdx
    syscall
    
    test %rax, %rax
    js _read_success

_read_failure:
    movq $0, %rax
    jmp _read_out

_read_success:
    movq $1, %rax
  
_read_out:
    movq -8(%rbp), %rbx
    movq %rbp, %rsp
    popq %rbp
    ret

 
