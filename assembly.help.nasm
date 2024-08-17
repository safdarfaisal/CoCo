; _foo1 input params c4d3, c3b2 output params b2c3 c3d2 pass by value

foo1: ; (NEAR or FAR PROC??)
    ; reserve memory for c3 set as global
    ; set value in c3 lvalue to 4
    ; set value in b2c3 lvalue to c4d3 + c3b2 - c3
    ; set value in c3d2 lvalue to c3 * c3
    ; push c3d2 and b2c3 to stack ret to main

readInt:
	; get memory address of variable (pushed to stack before call)
	pop eax
	

main:

    ; reserve memory for b3c4
    b3c4: db 8
    ; reserve memory for c3d2
    c3d2: db 4
    ; reserve memory for c3b4
    c3b4: db 4
    ; reserve memory for c3b5
    c3b5: db 4
    ; read input and assign to c3b4
    ; read input and assign to c3b5
    ; push c3b4 and c3b5 to stack(?) and call _foo1 to return c2d3 and c3d2
    ; write c3b4 to stdout
    ; write c2d3 to stdout

; TODO: implement functionality for records


