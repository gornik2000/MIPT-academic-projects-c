;==============================================================================
;
;  Calculates sum of pushed int line up to the first found 0
;
;  exit:  sum in eax
;  extra: prints raw size
;
;==============================================================================

section .code
global _my_sum

extern printf

_my_sum:
    push ebx
    push ecx

    push ebp 
    mov ebp, esp

    add ebp, 4 * 4	

    xor ecx, ecx

    mov eax, [ebp]
    cmp eax, 0
    je _my_sum_end


_my_sum_circle:
    add ebp, 4
    mov ebx, [ebp]
    inc ecx

    cmp ebx, 0    
    je _my_sum_end
    
    add eax, ebx

    jmp _my_sum_circle

_my_sum_end:

    push eax

    push ecx
    push dword msg
    call printf
    add esp, 4 * 2

    pop eax

    pop ebp    

    pop ecx
    pop ebx

    ret

msg : db 10, " # _my_sum got <%d> numbers", 0

;------------------------------------------------------------------------------
;                      © Gorbachev Nikita, April 2019                       
;------------------------------------------------------------------------------

