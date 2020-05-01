jmp main
func0:
pushr r0
push 1
jne if0_end
push 1
ret
if0_end:

pushr r0
pushr r0
pushr r0
push 1
sub
popr r0
call func0
popr r15
popr r0
pushr r15
mul
ret

push 0
ret

main:
pushr r0
in
popr r0
call func0
popr r15
popr r0
pushr r15
out
end
