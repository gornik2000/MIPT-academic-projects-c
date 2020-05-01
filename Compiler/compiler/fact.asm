.intel_syntax noprefix

		.globl _start

.text
func0:
push rbp
mov rbp, rsp
sub rsp, 0
push [rbp + (16)]
mov rax, 1
push rax
pop rbx
pop rax
cmp rax, rbx
jne if0_end
mov rax, 1
push rax
pop rax
mov [rbp + (24)], rax
leave
ret
if0_end:

push [rbp + (16)]
sub rsp, 8
push [rbp + (16)]
mov rax, 1
push rax
pop rbx
pop rax
sub rax, rbx
push rax
call func0
add rsp, 8
pop rbx
pop rax
xor rdx, rdx
imul rbx
push rax
pop rax
mov [rbp + (24)], rax
leave
ret

mov rax, 0
push rax
pop rax
mov [rbp + (24)], rax
leave
ret

_start:
push rbp
mov rbp, rsp
sub rsp, 0
sub rsp, 8
sub rsp, 8
call in
call func0
add rsp, 8
call out
leave
mov rax, 0x3c
xor rdi, rdi
syscall
