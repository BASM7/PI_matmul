bits 64

section .text
    global _copy_row
    global _quickMatrixMul
    global _test
    global _WinMain@16

_WinMain@16:

_test:
    ;mov     r10, rcx
    xor     rax, rax

    add     rax, rcx
    add     rax, rdx
    add     rax, r8
    add     rax, r9
    add     rax, [rsp+40]
    add     rax, [rsp+48]

    ret     16


_quickMatrixMul:

    xor         rax, rax
    xor         rbx, rbx

    mov         r11, [rsp+40]       ; get m from the stack.
    mov         r12, [rsp+48]       ; get p from the stack.

    mov         rax, 0              ; counter for the first matrix.
    mov         r10, 0              ; counter for the third matrix.
    ._loop:
    
    mov         rbx, 0              ; counter for the second matrix.
    ._loop_m2:
    xorps       xmm1, xmm1
    movups      xmm1, [rcx+rax*4]
    xorps       xmm2, xmm2
    movaps      xmm2, [rdx+rbx*4]

    call        _dot_product
    ;movss       [r8+r10*4], xmm0   ; this saves the value on the third matrix.  

    ;inc         r10   
    add         rbx, 4              ; add 4 to get the next column.
    add         rax, 4
    cmp         rbx, r11            ; stop condition is 4*columns.
    jne         ._loop_m2

    movss       [r8+r10*4], xmm0   ; this saves the value on the third matrix.  

    ;add         rax, 4
    cmp         rax, r9           ; stop condition is 4*rows.
    ;jne         ._loop
    ret         16

_dot_product:
    ;   returns the dot product of two vectors.
    ;   expects vectors on xmm1, xmm2.
    ;   leaves the dot product on xmm0.
    xorps       xmm4, xmm4

    movups      xmm3, xmm1
    movups      xmm4, xmm0

    xorps       xmm0, xmm0

    mulps       xmm1, xmm2  
    haddps      xmm0, xmm1
    movhlps     xmm0, xmm0
    haddps      xmm0, xmm0

    addss       xmm0, xmm4

    movups      xmm1, xmm3
    ret         

_copy_row:
    mov         rax, 0
    ._loop:
    movups      xmm1, [rcx+rax*4]
    movups      [rdx+rax*4], xmm1
    add         rax, 4
    cmp         rax, 16
    jne         ._loop
    ret         16