#include "coa/AssemblyDemo.h"
#include "utilities/Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void printFileOrFallback(const std::string& path, const std::string& fallback) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::cout << file.rdbuf() << "\n";
        file.close();
    } else {
        std::cout << fallback << "\n";
    }
}

void AssemblyDemo::displayBillingAssemblyCode() {
    Utils::printSubHeader("HOSPITAL BILLING ASSEMBLY PROGRAM (asm/billing.asm - COA CO2)");
    std::string fallback =
        "section .data\n"
        "    consultation_charge dd 500\n"
        "    medicine_charge     dd 750\n"
        "    room_charge         dd 2000\n"
        "    test_charge         dd 400\n"
        "    total_bill          dd 0\n\n"
        "section .text\n"
        "    global _start\n"
        "_start:\n"
        "    mov eax, [consultation_charge]  ; EAX = 500\n"
        "    mov ebx, [medicine_charge]      ; EBX = 750\n"
        "    add eax, ebx                    ; EAX = 1250\n"
        "    mov ebx, [room_charge]          ; EBX = 2000\n"
        "    add eax, ebx                    ; EAX = 3250\n"
        "    mov ebx, [test_charge]          ; EBX = 400\n"
        "    add eax, ebx                    ; EAX = 3650\n"
        "    mov [total_bill], eax           ; total_bill = 3650\n"
        "    mov ebx, eax\n"
        "    mov eax, 1\n"
        "    int 0x80\n";
    printFileOrFallback("asm/billing.asm", fallback);
}

void AssemblyDemo::displayArithmeticAssemblyCode() {
    Utils::printSubHeader("ARITHMETIC DEMO ASSEMBLY PROGRAM (asm/arithmetic.asm - COA CO2)");
    std::string fallback =
        "section .data\n"
        "    op1 dd 2500\n"
        "    op2 dd 1500\n"
        "section .text\n"
        "    global _start\n"
        "_start:\n"
        "    mov eax, [op1]\n"
        "    add eax, [op2]        ; ADD -> 4000\n"
        "    sub eax, [op2]        ; SUB -> 2500\n"
        "    mul dword [op2]       ; MUL -> EDX:EAX product\n"
        "    div dword [op2]       ; DIV -> EAX quotient, EDX remainder\n";
    printFileOrFallback("asm/arithmetic.asm", fallback);
}

void AssemblyDemo::displayPipelineAssemblyCode() {
    Utils::printSubHeader("5-STAGE PIPELINE DEMO ASSEMBLY PROGRAM (asm/pipeline.asm - COA CO4)");
    std::string fallback =
        "section .text\n"
        "    global _start\n"
        "_start:\n"
        "    mov eax, [charge1]\n"
        "    mov ebx, [charge2]\n"
        "    add eax, ebx\n"
        "    mov ebx, [charge3]\n"
        "    add eax, ebx\n"
        "    mov ebx, [charge4]\n"
        "    add eax, ebx\n";
    printFileOrFallback("asm/pipeline.asm", fallback);
}

void AssemblyDemo::explainInstructionSetDetails() {
    Utils::printSubHeader("80386 INSTRUCTION SET ANALYSIS FOR BILLING (COA CO2)");
    std::cout << "Key Instructions Analyzed:\n"
              << "  1. MOV reg, mem / MOV reg, imm\n"
              << "     - Opcode: B8+rd (e.g., B8 00 00 00 00 for MOV EAX, imm32)\n"
              << "     - Clock Cycles: 1 cycle (Register to Register) / 2 cycles (Memory load)\n"
              << "     - Flags: None affected\n"
              << "  2. ADD EAX, EBX\n"
              << "     - Opcode: 01 D8 (ADD r/m32, r32)\n"
              << "     - Clock Cycles: 1-2 cycles\n"
              << "     - Flags Updated: CF (Carry), OF (Overflow), ZF (Zero), SF (Sign), PF, AF\n"
              << "  3. MUL dword [mem]\n"
              << "     - Opcode: F7 /4 (Unsigned Multiply EAX by r/m32)\n"
              << "     - Output: 64-bit product stored in EDX:EAX\n"
              << "     - Clock Cycles: 9-38 cycles (Hardware Multiplier)\n"
              << "     - Flags: CF and OF set if EDX != 0\n"
              << "  4. DIV dword [mem]\n"
              << "     - Opcode: F7 /6 (Unsigned Divide EDX:EAX by r/m32)\n"
              << "     - Output: EAX = Quotient, EDX = Remainder\n"
              << "     - Exceptions: Generates Interrupt 0 (#DE) on divide by zero or overflow\n"
              << "  5. INT 0x80 / INT 21h\n"
              << "     - Software Interrupt invoking kernel system service\n\n";
}

void AssemblyDemo::displayCompilationGuide() {
    Utils::printSubHeader("HOW TO ASSEMBLE AND LINK .ASM FILES (COA CO2)");
    std::cout << "1. Linux with NASM & LD (32-bit ELF):\n"
              << "   $ nasm -f elf32 asm/billing.asm -o billing.o\n"
              << "   $ ld -m elf_i386 billing.o -o billing\n"
              << "   $ ./billing; echo $?  # Echoes 3650 (or modulo 256 exit code)\n\n"
              << "2. Windows with MASM / ML:\n"
              << "   > ml /c /coff /Cp asm\\billing.asm\n"
              << "   > link /subsystem:console billing.obj\n"
              << "   > billing.exe\n\n"
              << "3. DOS with TASM (Turbo Assembler):\n"
              << "   > tasm /m3 asm\\billing.asm\n"
              << "   > tlink billing.obj\n\n";
}

void AssemblyDemo::stepThroughBillingExecution() {
    Utils::printSubHeader("STEP-BY-STEP REGISTER TRACE OF BILLING ASSEMBLY (COA CO2)");
    uint32_t eax = 0, ebx = 0;
    std::cout << "Initial: EAX = 0, EBX = 0\n";

    eax = 500;
    std::cout << "Step 1: MOV EAX, 500  => EAX = " << eax << ", EBX = " << ebx << "\n";

    ebx = 750;
    std::cout << "Step 2: MOV EBX, 750  => EAX = " << eax << ", EBX = " << ebx << "\n";

    eax += ebx;
    std::cout << "Step 3: ADD EAX, EBX  => EAX = " << eax << " (500 + 750 = 1250), EBX = " << ebx << "\n";

    ebx = 2000;
    std::cout << "Step 4: MOV EBX, 2000 => EAX = " << eax << ", EBX = " << ebx << "\n";

    eax += ebx;
    std::cout << "Step 5: ADD EAX, EBX  => EAX = " << eax << " (1250 + 2000 = 3250), EBX = " << ebx << "\n";

    ebx = 400;
    std::cout << "Step 6: MOV EBX, 400  => EAX = " << eax << ", EBX = " << ebx << "\n";

    eax += ebx;
    std::cout << "Step 7: ADD EAX, EBX  => EAX = " << eax << " (3250 + 400 = 3650), EBX = " << ebx << "\n";

    std::cout << "\nResult: Accumulator EAX holds Rs. " << eax << " (Matching hospital invoice total!)\n";
}
