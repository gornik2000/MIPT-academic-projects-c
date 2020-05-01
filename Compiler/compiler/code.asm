.intel_syntax noprefix

		.globl _start

.text
func0:
push rbp
mov rbp, rsp
sub rsp, 8
mov rax, 1
push rax
pop [rbp + (-8)]

push [rbp + (16)]
mov rax, 1
push rax
pop rbx
pop rax
cmp rax, rbx
jbe if0_end
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
pop [rbp + (-8)]
if0_end:

push [rbp + (-8)]
pop rax
mov [rbp + (24)], rax
leave
ret

func1:
push rbp
mov rbp, rsp
sub rsp, 24
mov rax, 1
push rax
pop [rbp + (-8)]

push [rbp + (16)]
mov rax, 1
push rax
pop rbx
pop rax
cmp rax, rbx
jbe if1_end
sub rsp, 8
push [rbp + (16)]
mov rax, 1
push rax
pop rbx
pop rax
sub rax, rbx
push rax
call func1
add rsp, 8
pop [rbp + (-16)]

sub rsp, 8
push [rbp + (16)]
mov rax, 2
push rax
pop rbx
pop rax
sub rax, rbx
push rax
call func1
add rsp, 8
pop [rbp + (-24)]

push [rbp + (-16)]
push [rbp + (-24)]
pop rbx
pop rax
add rax, rbx
push rax
pop [rbp + (-8)]
if1_end:

push [rbp + (-8)]
pop rax
mov [rbp + (24)], rax
leave
ret

_start:
push rbp
mov rbp, rsp
sub rsp, 16
sub rsp, 8
call in
pop [rbp + (-8)]

loop0:
push [rbp + (-8)]
mov rax, 0
push rax
pop rbx
pop rax
cmp rax, rbx
je loop0_end
sub rsp, 8
push [rbp + (-8)]
call func0
add rsp, 8
call out

sub rsp, 8
call in
pop [rbp + (-8)]
jmp loop0
loop0_end:

sub rsp, 8
call in
pop [rbp + (-16)]

loop1:
push [rbp + (-16)]
mov rax, 0
push rax
pop rbx
pop rax
cmp rax, rbx
je loop1_end
sub rsp, 8
push [rbp + (-16)]
call func1
add rsp, 8
call out

sub rsp, 8
call in
pop [rbp + (-16)]
jmp loop1
loop1_end:

mov rax, 0
push rax
xor rdi, rdi
leave
mov rax, 0x3c
syscall
leave
mov rax, 0x3c
xor rdi, rdi
syscall
