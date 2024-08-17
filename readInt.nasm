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
atoi_convert:    
    movzx ecx, byte [esi]
    movzx ebx, buffer_size
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
    dec ebx
    cmp ebx, 0
    je done_convert
    jmp atoi_convert
done_convert:
    push eax
    ret


;printInt : 

printInt:
    ;get memory address from stack
    