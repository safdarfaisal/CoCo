section .data
    buffer db 0
    buffer_size equ 8

section .bss
    input resb buffer_size

section .text
    global readInt

;readInt : Reads an integer from console, stops at first non-digit
;          character (upto 8 digits)
; input : memory address of variable 
; output : void
readInt: 
    pop ecx
    mov eax, 3
    mov ebx, 0
    ;mov ecx, input
    mov edx, buffer_size
    int 0x80
    mov esi, input
    xor eax, eax
    xor ebx, ebx
    mov bl, buffer_size
.atoi_convert:    
    movzx ecx, byte [esi]
    cmp ecx, '-'
    ; set ebx to 1, so to neg instruction can be triggered.
    jne posIntConvert
    mov bh, 1
    inc esi
.posIntConvert:
    movzx ecx, byte [esi]
    cmp ecx, 10
    je done_convert
    cmp ecx, 0x30
    jle done_convert
    cmp ecx, 0x3A
    jge done_convert
    sub ecx, '0'
    imul eax, eax, 10
    add eax, ecx
    inc esi
    dec bl
    cmp bl, 0
    je done_convert
    jmp atoi_convert
.checkNegate:
    cmp bh, 1
    je negateMem
.return:
    push eax
    ret
.negateMem:
    neg eax
    push eax
    ret



;printInt : 
printInt:
    ;get memory address from stack
