jmp main
func0:
push 1
popr r1

pushr r0
push 1
jbe if0_end
pushr r0
pushr r1
pushr r0
pushr r0
push 1
sub
popr r0
push 0
popr r1
call func0
popr r15
popr r0
popr r1
pushr r15
mul
popr r1
if0_end:

pushr r1
ret

func1:
push 1
popr r1

pushr r0
push 1
jbe if1_end
pushr r3
pushr r2
pushr r1
pushr r0
pushr r0
push 1
sub
popr r0
push 0
popr r1
push 0
popr r2
push 0
popr r3
call func1
popr r15
popr r0
popr r1
popr r2
popr r3
pushr r15
popr r2

pushr r3
pushr r2
pushr r1
pushr r0
pushr r0
push 2
sub
popr r0
push 0
popr r1
push 0
popr r2
push 0
popr r3
call func1
popr r15
popr r0
popr r1
popr r2
popr r3
pushr r15
popr r3

pushr r2
pushr r3
add
popr r1
if1_end:

pushr r1
ret

main:
in
popr r0

loop0:
pushr r0
push 0
je loop0_end
pushr r1
pushr r0
pushr r0
popr r0
push 0
popr r1
call func0
popr r15
popr r0
popr r1
pushr r15
out

in
popr r0
jmp loop0
loop0_end:

in
popr r1

loop1:
pushr r1
push 0
je loop1_end
pushr r3
pushr r2
pushr r1
pushr r0
pushr r1
popr r0
push 0
popr r1
push 0
popr r2
push 0
popr r3
call func1
popr r15
popr r0
popr r1
popr r2
popr r3
pushr r15
out

in
popr r1
jmp loop1
loop1_end:

push 0
ret
end
