; ==============================================================================
; SMART HOSPITAL MANAGEMENT SYSTEM - PIPELINE DEMONSTRATION (NASM 80386 ASSEMBLY)
; COA CO4: 5-Stage Instruction Pipelining, Hazards, and Control Signals
; Instructions advance: FETCH -> DECODE -> EXECUTE -> MEMORY -> WRITEBACK
; ==============================================================================

section .data
    charge1     dd 500               ; Doctor Consultation
    charge2     dd 750               ; Pharmacy Medicine
    charge3     dd 2000              ; Ward Bed
    charge4     dd 400               ; Diagnostic Test
    grand_total dd 0

section .text
    global _start

_start:
    ; Instruction 1: Load consultation charge into EAX
    ; Clock Cycles: F:1, D:2, E:3, M:4, W:5
    mov eax, [charge1]               ; EAX = 500

    ; Instruction 2: Load pharmacy charge into EBX
    ; Clock Cycles: F:2, D:3, E:4, M:5, W:6
    mov ebx, [charge2]               ; EBX = 750

    ; Instruction 3: Add EBX into EAX
    ; RAW Hazard detected on EBX! (Operand forwarding passes EBX from Cycle 4 to 5)
    ; Clock Cycles: F:3, D:4, E:5, M:6, W:7
    add eax, ebx                     ; EAX = 1250

    ; Instruction 4: Load bed charge into EBX
    ; Clock Cycles: F:4, D:5, E:6, M:7, W:8
    mov ebx, [charge3]               ; EBX = 2000

    ; Instruction 5: Add EBX into EAX
    ; Clock Cycles: F:5, D:6, E:7, M:8, W:9
    add eax, ebx                     ; EAX = 3250

    ; Instruction 6: Load diagnostic lab charge into EBX
    ; Clock Cycles: F:6, D:7, E:8, M:9, W:10
    mov ebx, [charge4]               ; EBX = 400

    ; Instruction 7: Add EBX into EAX
    ; Clock Cycles: F:7, D:8, E:9, M:10, W:11
    add eax, ebx                     ; EAX = 3650 (Final Bill computed in 11 cycles!)

    ; Store result and exit
    mov [grand_total], eax
    mov ebx, eax
    mov eax, 1
    int 0x80
