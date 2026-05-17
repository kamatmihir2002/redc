#include <asm/unistd.h>

.text

.equ PROT_READ,      1
.equ PROT_WRITE,     2

.equ MAP_PRIVATE,    2
.equ MAP_ANONYMOUS, 32

.global abi_sysv_amd64_free
.global abi_sysv_amd64_malloc

abi_sysv_amd64_malloc:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    
    # save rbx
    movq %rbx, -8(%rbp)
    
    # rdi = size
    
    # add 8 to size and save to stack
    addq $8, %rdi
    movq %rdi, -16(%rbp)

    movq $__NR_mmap, %rax #syscall num
    movq $0, %rdi
    movq -16(%rbp), %rsi
    movq $(PROT_READ + PROT_WRITE), %rdx
    movq $(MAP_PRIVATE + MAP_ANONYMOUS), %r10
    movq $-1, %r8
    movq $0, %r9
    syscall
    
    testq %rax, %rax
    js _mmap_failure

    # load size
    movq -16(%rbp), %rbx
    
    # mov size to location in rax (on success)
    movq %rbx, (%rax)

    # return ptr after size
    addq $8, %rax
    jmp _mmap_success
_mmap_failure:
    movq $0, %rax
_mmap_success:
    movq -8(%rbp), %rbx
    movq %rbp, %rsp
    popq %rbp
    ret
    

abi_sysv_amd64_free:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
 
    # rdi = ptr given to user (actual ptr + 8)
 
    movq $__NR_munmap, %rax
    subq $8, %rdi
    movq (%rdi), %rsi
    syscall

    xorq %rax, %rax
    movq %rbp, %rsp
    popq %rbp
    ret
    

