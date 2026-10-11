; ==============================================================================
; SMART HOSPITAL MANAGEMENT SYSTEM - ARITHMETIC DEMO (NASM 80386 ASSEMBLY)
; COA CO2 & CO3: Basic & Complex Computer Arithmetic Operations
; Demonstrates ADD, SUB, MUL (64-bit product), DIV, XOR, and ALU Flag behavior
; ==============================================================================

section .data
    op1         dd 2500
    op2         dd 1500
    res_add     dd 0
    res_sub     dd 0
    res_mul_lo  dd 0
    res_mul_hi  dd 0
    res_div_q   dd 0
    res_div_r   dd 0

section .text
    global _start

_start:
    ; 1. ADDITION: EAX = op1 + op2
    mov eax, [op1]                   ; EAX = 2500
    add eax, [op2]                   ; EAX = 4000
    mov [res_add], eax               ; Flags updated: ZF=0, CF=0, SF=0

    ; 2. SUBTRACTION: EAX = op1 - op2
    mov eax, [op1]                   ; EAX = 2500
    sub eax, [op2]                   ; EAX = 1000
    mov [res_sub], eax               ; Flags updated: ZF=0, CF=0, SF=0

    ; 3. MULTIPLICATION: 64-bit product in EDX:EAX = EAX * op2
    mov eax, [op1]                   ; EAX = 2500
    mul dword [op2]                  ; EDX:EAX = 2500 * 1500 = 3,750,000
    mov [res_mul_lo], eax            ; Lower 32 bits = 3,750,000
    mov [res_mul_hi], edx            ; Upper 32 bits = 0 (CF=0, OF=0)

    ; 4. DIVISION: EAX = Quotient, EDX = Remainder
    mov eax, [op1]                   ; EAX = 2500 (Dividend)
    xor edx, edx                     ; EDX = 0 (Clear high 32 bits)
    div dword [op2]                  ; Divide EDX:EAX by 1500
    mov [res_div_q], eax             ; Quotient = 1
    mov [res_div_r], edx             ; Remainder = 1000

    ; 5. EXIT
    mov eax, 1                       ; sys_exit
    xor ebx, ebx                     ; return 0
    int 0x80
