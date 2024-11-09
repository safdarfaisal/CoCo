section .data
    buffer_size equ 20
    buffer db 0
    max_int_size equ 8
    max_real_size equ 8
    debug_char db 'X'
section .bss
    input resb buffer_size

section .text
    global _start
    global read_int

_start:
    ; call debug_print
    call read_int
    call debug_print
    mov eax, 1
    xor ebx, ebx
    int 0x80 

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
    ; pop edx ; size of string
    ; pop ecx ; address
    push eax
    push ebx
    mov eax, 3 ; sys call num for sys_read
    mov ebx, 0 ; file descriptor for std_in
    int 0x80 ; result stored in [ecx]
    pop ebx
    pop eax
    ; call debug_print
    ret
debug_print:
    push eax
    push ebx
    push ecx
    push edx
    mov eax, 4
    mov ebx, 1
    mov ecx, debug_char
    mov edx, 1
    int 0x80
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret 


;readInt : Reads an integer from console, stops at first non-digit
;          character (upto 8 digits)
; input : memory address of variable 
; output : void
read_int: 
    mov edx, max_int_size
    mov ecx, input
    ; call debug_print
    ; push ecx
    ; push edx
    call read_str ; address of variable 
    ; call debug_print
    ; pop ecx
    mov esi, input
    ; call debug_print    
    xor eax, eax ; set eax and ebx to 0
    xor ebx, ebx
    ; call debug_print
    mov bl, buffer_size
    ; call debug_print
.atoi_convert:    
    ; call debug_print
    movzx ecx, byte [esi]
    ; call debug_print
    cmp ecx, '-'
    ; set ebx to 1, so to neg instruction can be triggered.
    jne .pos_int_convert
    mov bh, 1
    ; call debug_print
    inc esi
.pos_int_convert:
    movzx ecx, byte [esi]
    ; call debug_print
    cmp ecx, 10
    ; call debug_print
    je .check_negate
    cmp ecx, 0x30
    jle .check_negate
    cmp ecx, 0x3A
    jge .check_negate
    sub ecx, '0'
    imul eax, eax, 10
    add eax, ecx
    inc esi
    dec bl
    cmp bl, 0
    je .check_negate
    jmp .pos_int_convert
.check_negate:
    call debug_print
    cmp bh, 1
    je .negate_mem
.return:
    ; push eax
    call debug_print
    ret
.negate_mem:
    neg eax
    ; push eax
    call debug_print
    ret



;printInt : 

print_int:
    mov edi, 10
    mov ebx, 0
.next_digit:
    xor edx, edx
    div edi
    add dl, '0'
    push dx
    inc ebx
    test eax, eax
    jnz .next_digit

.print_loop:
    pop ax
    mov [buffer], ax
    mov eax, 4
    mov ebx, 1
    mov ecx, buffer
    mov edx, 1
    int 0x80
    dec ebx
    jnz .print_loop
    ret

call print_str