section .data
    buffer db 0
    max_int_size equ 8
    max_real_size equ 8
section .bss
    input resb buffer_size

section .text
    global readInt


; print_str : calls sys_write to write to std_out
;             file descriptor 1
; input : edx - length of data written onto std_out
;         ecx - memory address pointer 
; output : void
print_str:
    pop edx
    pop ecx ; How do I get the length of the message
    push eax
    push ebx
    mov eax, 4
    mov ebx, 1
    int 80H ; calls sys_write
    pop ebx
    pop eax
    ret



; print_int : calls printf 


; read_str : calls sys_read to read from console
; input : ecx - address pointed to by pointer
;         edx - size of string to be read
;         push into the stack in that order
;         push ecx push edx
; output : eax - size of string actually read
;          stack has address of memory location
read_str:
    pop edx ; size of string
    pop ecx ; address
    push eax
    push ebx
    mov eax, 3 ; sys call num for sys_read
    mov ebx, 0 ; file descriptor for std_in
    int 0x80 ; result stored in [ecx]
    pop ebx
    pop eax
    push ecx
    ret

;readInt : Reads an integer from console, stops at first non-digit
;          character (upto 8 digits)
; input : memory address of variable 
; output : void
read_int: 
    mov edx, max_int_size
    push edx
    call read_str ; address of variable 
    pop ecx
    mov esi, ecx
    xor eax, eax ; set eax and ebx to 0
    xor ebx, ebx
    mov bl, buffer_size
.atoi_convert:    
    movzx ecx, byte [esi]
    cmp ecx, '-'
    ; set ebx to 1, so to neg instruction can be triggered.
    jne pos_int_convert
    mov bh, 1
    inc esi
.pos_int_convert:
    movzx ecx, byte [esi]
    cmp ecx, 10
    je check_negate
    cmp ecx, 0x30
    jle check_negate
    cmp ecx, 0x3A
    jge check_negate
    sub ecx, '0'
    imul eax, eax, 10
    add eax, ecx
    inc esi
    dec bl
    cmp bl, 0
    je check_negate
    jmp pos_int_convert
.check_negate:
    cmp bh, 1
    je negate_mem
.return:
    push eax
    ret
.negate_mem:
    neg eax
    push eax
    ret



;printInt : 
printInt:
    ;get memory address from stack
