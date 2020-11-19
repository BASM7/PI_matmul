bits 64

section .text
    global _copy_row
    global _quickMatrixMul
    global _test
    global _WinMain@16

_WinMain@16:

; _test:
;     ;mov     r10, rcx
;     xor     rax, rax

;     add     rax, rcx
;     add     rax, rdx
;     add     rax, r8
;     add     rax, r9
;     add     rax, [rsp+40]
;     add     rax, [rsp+48]

;     ret     16


_quickMatrixMul:

    xor         rax, rax
    xor         rbx, rbx

    mov         r11, [rsp+40]       ; get m from the stack.
    mov         r12, [rsp+48]       ; get p from the stack.

    mov         rax, 0              ; counter for the first matrix.
    mov         r10, 0              ; counter for the third matrix.
    mov         r15, 0

    
    ._loop:    
    xor         r14, r14
    mov         rbx, 0              ; counter for the second matrix.

    ._loop_m2:
    xorps       xmm0, xmm0
    push        rax    
    xor         r13, r13

    ._loop_load:
    
    xorps       xmm1, xmm1
    movups      xmm1, [rcx+rax*4]
    xorps       xmm2, xmm2
    movaps      xmm2, [rdx+rbx*4]

    call        _dot_product
    
    add         r13, 4
    add         rbx, 4              ; add 4 to get the next column.
    add         rax, 4
    cmp         r13, r11             ; stop condition is 4*rows.
    jne         ._loop_load

    movss       [r8+r10*4], xmm0   ; this saves the value on the third matrix.
    inc         r10
    inc         r14
    pop         rax
    cmp         r14, r12
    jne         ._loop_m2

    add         rax, r11            ; rax+m
    inc         r15
    cmp         r15, r9
    jne         ._loop

    ret         16

_dot_product:
    ;   returns the dot product of two vectors.
    ;   expects vectors on xmm1, xmm2.
    ;   leaves the dot product on xmm0.
    xorps       xmm4, xmm4          ; clear xmm4.
    xorps       xmm3, xmm3          ; clear xmm3.
    movups      xmm4, xmm0          ; move whatever is in xmm0 to xmm4 for later.

    xorps       xmm0, xmm0          ; clear xmm0;
    movups      xmm3, xmm1          ; move xmm1 to xmm3 to restore later.

    mulps       xmm1, xmm2          ; multiply xmm1 and xmm2, store in xmm1.
    haddps      xmm0, xmm1
    movhlps     xmm0, xmm0
    haddps      xmm0, xmm0

    addss       xmm0, xmm4          ; add lower float.

    movups      xmm1, xmm3          ; restore xmm1.
    ret         


; 70.31,36.18,49.31,65.50,58.56,52.00,69.00,72.43
; 44.43,35.37,48.87,58.00,38.93,45.06,55.75,55.93
; 76.43,68.12,60.18,74.06,63.81,66.50,69.00,74.87
; 67.81,53.06,60.06,66.25,71.31,73.50,66.12,77.43
; 72.25,50.31,60.31,74.75,69.56,68.81,75.75,77.93
; 65.87,54.12,59.43,62.06,77.93,66.18,69.18,78.50
; 57.37,38.25,41.93,68.81,43.06,51.68,52.68,62.25
; 52.50,54.87,56.18,68.50,49.31,65.75,48.37,68.00