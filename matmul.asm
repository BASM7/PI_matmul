bits 64

section .text
    global _sumRow
    global _WinMain@16

_WinMain@16:

; 1º: RCX
; 2º: RDX
; 3º: R8
; 4º: R9
; >4: Stack.

_sumRow:
    movlps  xmm1, [ecx+0*4]
    movlps  xmm0, [ecx+1*4]

    addss   xmm0, xmm1
    ret     16

    ; xmml -> 128 


    ; ._loop:    
    ; addss   xmm0, xmm0
    ; dec     rdx
    ; cmp     rdx, 0
    ; jne     ._loop