; TP2 Proyecto Integrador de Lenguaje Ensamblador.
; author B93986 Luis Alfonso Jiménez.
; author B95346 Jesús Alonso Moreno Montero.
; author B95092 Víctor Jesús Mora Abarca.

bits 64

section .text
    global _quickMatrixMul
    global _WinMain@16

_WinMain@16:

_quickMatrixMul:

    xor         rax, rax            ; clear rax.
    xor         rbx, rbx            ; clear rbx.

    mov         r11, [rsp+40]       ; get m from the stack.
    mov         r12, [rsp+48]       ; get p from the stack.

    mov         rax, 0              ; counter for the first matrix.
    mov         r10, 0              ; counter for the third matrix.
    mov         r15, 0              ; counter for the rows.

    
    ._loop:    
    xor         r14, r14            ; counter for the columns.
    xor         rbx, rbx            ; counter for the second matrix.

    ._loop_m2:
    xorps       xmm0, xmm0          ; clear the output register.
    push        rax                 ; save rax.
    xor         r13, r13            ; chunk counter.

    ._loop_load:
    
    xorps       xmm1, xmm1          ; clear the xmm1 register.
    movups      xmm1, [rcx+rax*4]   ; move xmm1 the next row chunk.
    xorps       xmm2, xmm2          ; clear the xmm2 register.
    movaps      xmm2, [rdx+rbx*4]   ; move xmm2 the next transposed column chunk.

    call        _dot_product
    
    add         r13, 4              ; add 4 to the chunk counter.
    add         rbx, 4              ; add 4 to get the next column.
    add         rax, 4              ; add 4 to get the next row.
    cmp         r13, r11            ; stop condition is 4*rows.
    jl         ._loop_load

    movss       [r8+r10*4], xmm0    ; this saves the value on the third matrix using r10 as the writer pointer.
    inc         r10                 ; increment the writer pointer.
    inc         r14                 ; increment the column counter.
    pop         rax                 ; restore rax.
    cmp         r14, r12            ; compare the current column with the number of output columns.
    jl         ._loop_m2

    add         rax, r11            ; do rax+m
    inc         r15                 ; increment the counter for rows.
    cmp         r15, r9             ; compare the row counter with the expected rows.
    jl         ._loop               ; if less, repeat.

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
    haddps      xmm0, xmm1          ; add first half xmm1 horizontally in xmm0, stores in upper xmm0.
    movhlps     xmm0, xmm0          ; move upper xmm0 to lower xmm0.
    haddps      xmm0, xmm0          ; add second half xmm0 horizontally. 

    addss       xmm0, xmm4          ; add dot product result with previous result for return.

    movups      xmm1, xmm3          ; restore xmm1.
    ret         