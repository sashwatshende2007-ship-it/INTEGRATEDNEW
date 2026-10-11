; ==============================================================================
; SMART HOSPITAL MANAGEMENT SYSTEM - BILLING CALCULATION (NASM 80386 ASSEMBLY)
; COA CO2: Develop assembly language programs using 80386 instruction set
; Standard Example: Consultation (500) + Meds (750) + Room (2000) + Test (400) = 3650
; ==============================================================================

section .data
    ; Hospital Itemized Charges (32-bit double words)
    consultation_charge dd 500       ; Consultation fee in EAX
    medicine_charge     dd 750       ; Pharmacy medicine charge
    room_charge         dd 2000      ; Ward / Bed daily charge
    test_charge         dd 400       ; Diagnostic lab / radiology charge
    
    ; Output variable to store calculated grand total
    total_bill          dd 0

section .text
    global _start

_start:
    ; Step 1: Load doctor consultation charge into Accumulator (EAX)
    mov eax, [consultation_charge]   ; EAX = 500

    ; Step 2: Load pharmacy medicine charge into Base Register (EBX) and Add
    mov ebx, [medicine_charge]       ; EBX = 750
    add eax, ebx                     ; EAX = 500 + 750 = 1250 (CF=0, ZF=0)

    ; Step 3: Load bed/room charge into EBX and Add
    mov ebx, [room_charge]           ; EBX = 2000
    add eax, ebx                     ; EAX = 1250 + 2000 = 3250

    ; Step 4: Load laboratory diagnostic test charge into EBX and Add
    mov ebx, [test_charge]           ; EBX = 400
    add eax, ebx                     ; EAX = 3250 + 400 = 3650 (Final Subtotal)

    ; Step 5: Store final computed total from EAX into memory destination
    mov [total_bill], eax            ; Memory[total_bill] = 3650

    ; Step 6: Graceful Program Exit (Linux sys_exit or DOS INT 21h equivalent)
    mov ebx, eax                     ; Return final bill amount as exit code
    mov eax, 1                       ; sys_exit syscall number (1)
    int 0x80                         ; Invoke kernel interrupt
