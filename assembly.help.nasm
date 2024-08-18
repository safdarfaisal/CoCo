section .data
    prompt1 db "Enter first number: ", 0
    prompt2 db "Enter second number: ", 0
    output1 db "Result (c3b4): ", 0
    output2 db "Result (c2d3): ", 0
    newline db 0xA, 0

ection .bss
    b3c4_foo resd 1
    b3c4_bar resd 1
    c3b4 resd 1
    c3b5 resd 1
    c3 resd 1
    b2c3 resd 1s
    c3d2 resd 1
    c2b3 resd 1
    c2d3 resd 1

section .text
    global _start

_start:
    ; Print prompt for first number
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt1
    mov edx, 18
    int 0x80

    ; Read first number from input
    call read_int
    mov [c3b4], eax

    ; Print prompt for second number
    mov eax, 4
    mov ebx, 1
    mov ecx, prompt2
    mov edx, 19
    int 0x80

    ; Read second number from input
    call read_int
    mov [c3b5], eax

    ; Assign values to record fields
    mov dword [b3c4_foo], 47
    mov dword [b3c4_bar], 78

    ; Perform addition for c2b3
    mov eax, [b3c4_foo]
    add eax, [b3c4_bar]
    mov [c2b3], eax

    ; Call function _foo1
    mov eax, [c3b4]       ; c4d3
    mov ebx, [c3b5]       ; c3b2
    call _foo1

    ; Store results in c2d3 and c3d2
    mov [c2d3], eax
    mov [c3d2], ebx

    ; Print c3b4 (input number)
    mov eax, [c3b4]
    call print_int

    ; Print newline
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    ; Print c2d3 (result)
    mov eax, [c2d3]
    call print_int

    ; Exit
    mov eax, 1
    xor ebx, ebx
    int 0x80

; Function _foo1
; Input: eax (c4d3), ebx (c3b2)
; Output: eax (b2c3), ebx (c3d2)
_foo1:
    mov ecx, 4        ; c3 = 4
    sub eax, ecx      ; c4d3 - c3
    add eax, ebx      ; (c4d3 - c3) + c3b2
    mov edx, ecx
    imul edx, ecx     ; c3 * c3
    mov ebx, edx      ; c3d2 = c3 * c3
    ret
; TODO: implement functionality for records

