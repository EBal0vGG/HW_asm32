global f1, f2, f3
global f1d, f2d, f3d

section .data
one     dq 1.0
two     dq 2.0
three   dq 3.0
half    dq 0.5
six     dq 6.0
log2e   dq 1.4426950408889634 ; log2(e)

section .text

; f1(x) = 3 / ((x - 1)^2 + 1)
f1:
    push ebp
    mov ebp, esp

    fld qword [ebp + 8]        ; ST(0) = x
    fld1                       ; ST(0) = 1
    fsub                       ; ST(0) = x - 1
    fld st0                    ; ST(0) = x - 1, ST(1) = x - 1
    fmul                       ; ST(0) = (x - 1)^2
    fld1                       ; ST(0) = 1
    fadd                       ; ST(0) = (x - 1)^2 + 1
    fld qword [three]          ; ST(0) = 3
    fxch                       ; swap: ST(0) = denom, ST(1) = 3
    fdiv                       ; ST(0) = 3 / denom

    pop ebp
    ret

; f2(x) = sqrt(x + 0.5)
f2:
    push ebp
    mov ebp, esp

    fld qword [ebp + 8]        ; x
    fld qword [half]
    fadd                       ; x + 0.5
    fsqrt                      ; sqrt(x + 0.5)

    pop ebp
    ret

; f3(x) = exp(-x)
f3:
    push ebp
    mov ebp, esp

    fld qword [ebp + 8]        ; x
    fchs                       ; -x
    fld qword [log2e]          ; log2(e)
    fmulp                      ; -x * log2(e)
    fld st0
    frndint
    fsub st1, st0
    fxch
    f2xm1
    fld1
    fadd
    fscale
    fstp st1

    pop ebp
    ret

; df1(x) = -6(x - 1) / ((x - 1)^2 + 1)^2
f1d:
    push ebp
    mov ebp, esp

    ; ST(0) = x
    fld qword [ebp + 8]
    ; ST(0) = x - 1
    fld qword [one]
    fsub
    ; ST(0) = x - 1
    fld st0              ; ST(0) = x - 1, ST(1) = x - 1
    fmul st0, st0        ; ST(0) = (x - 1)^2, ST(1) = x - 1
    fld qword [one]
    fadd                 ; ST(0) = (x - 1)^2 + 1, ST(1) = x - 1
    fld st0
    fmul                 ; ST(0) = ((x - 1)^2 + 1)^2, ST(1) = x - 1
    fxch st1             ; ST(0) = x - 1, ST(1) = denom
    fmul qword [six]     ; ST(0) = 6(x - 1), ST(1) = denom
    fchs                 ; ST(0) = -6(x - 1), ST(1) = denom
    fdiv st0, st1        ; ST(0) = -6(x - 1) / denom
    fstp st1             ; очистка

    pop ebp
    ret

; df2(x) = 1 / (2 * sqrt(x + 0.5))
f2d:
    push ebp
    mov ebp, esp

    fld qword [ebp + 8]        ; x
    fld qword [half]           ; 0.5
    fadd                       ; x + 0.5
    fsqrt                      ; sqrt(x + 0.5)
    fld qword [two]            ; 2
    fmul                       ; 2 * sqrt(x + 0.5)
    fld1                       ; 1
    fxch                       ; ST(0) = denom, ST(1) = 1
    fdiv                       ; ST(0) = 1 / (2 * sqrt(...))

    pop ebp
    ret

; df3(x) = -e^(-x)
f3d:
    push ebp
    mov ebp, esp

    fld qword [ebp + 8]
    fchs
    fld qword [log2e]
    fmulp
    fld st0
    frndint
    fsub st1, st0
    fxch
    f2xm1
    fld1
    fadd
    fscale
    fstp st1
    fchs                       ; negate for -e^(-x)

    pop ebp
    ret
