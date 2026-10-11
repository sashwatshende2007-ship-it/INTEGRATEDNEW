#include "coa/COAIntegration.h"
#include "utilities/Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>

COAIntegration::COAIntegration() {
    cpu.reset();
}

void COAIntegration::processHospitalBill(const BillingCharges& charges) {
    Utils::printHeader("80386 HARDWARE REGISTER PROCESSING OF HOSPITAL BILL");
    std::cout << "Input Hospital Charges Received:\n"
              << "  - Consultation : Rs. " << charges.consultation << "\n"
              << "  - Medicine     : Rs. " << charges.medicine << "\n"
              << "  - Room/Bed     : Rs. " << charges.room << "\n"
              << "  - Tests/Lab    : Rs. " << charges.test << "\n"
              << "--------------------------------------------------\n";

    cpu.reset();
    std::cout << "[Step 1] Loading Consultation charge into Accumulator EAX...\n";
    cpu.MOV_reg_imm("EAX", charges.consultation);
    std::cout << "         Instruction : MOV EAX, " << charges.consultation << "\n";
    std::cout << "         Register    : EAX = 0x" << std::hex << cpu.EAX << " (" << std::dec << cpu.EAX << ")\n";

    std::cout << "\n[Step 2] Loading Medicine charge into EBX & Executing ALU ADD...\n";
    cpu.MOV_reg_imm("EBX", charges.medicine);
    cpu.ADD_reg_reg("EAX", "EBX");
    std::cout << "         Instructions: MOV EBX, " << charges.medicine << "\n";
    std::cout << "                       ADD EAX, EBX\n";
    std::cout << "         Register    : EAX = 0x" << std::hex << cpu.EAX << " (" << std::dec << cpu.EAX << ")\n";
    cpu.displayFlags();

    std::cout << "\n[Step 3] Loading Room/Bed charge into EBX & Executing ALU ADD...\n";
    cpu.MOV_reg_imm("EBX", charges.room);
    cpu.ADD_reg_reg("EAX", "EBX");
    std::cout << "         Instructions: MOV EBX, " << charges.room << "\n";
    std::cout << "                       ADD EAX, EBX\n";
    std::cout << "         Register    : EAX = 0x" << std::hex << cpu.EAX << " (" << std::dec << cpu.EAX << ")\n";
    cpu.displayFlags();

    std::cout << "\n[Step 4] Loading Diagnostic Test charge into EBX & Executing ALU ADD...\n";
    cpu.MOV_reg_imm("EBX", charges.test);
    cpu.ADD_reg_reg("EAX", "EBX");
    std::cout << "         Instructions: MOV EBX, " << charges.test << "\n";
    std::cout << "                       ADD EAX, EBX\n";
    std::cout << "         Register    : EAX = 0x" << std::hex << cpu.EAX << " (" << std::dec << cpu.EAX << ")\n";
    cpu.displayFlags();

    std::cout << "\n==================================================\n";
    std::cout << " Final Computed 80386 Hardware Bill: Rs. " << cpu.EAX << "\n";
    std::cout << " Matches Hospital System Grand Total: " 
              << ((cpu.EAX == charges.total) ? "YES (VERIFIED 100% ACCURATE)" : "NO") << "\n";
    std::cout << "==================================================\n\n";

    cpu.displayRegisterState();
}

void COAIntegration::demonstrateCOA_CO1_Architecture() {
    Utils::printHeader("COA CO1: 80386 ARCHITECTURE & INSTRUCTION SET");
    cpu.explainArchitecture();
    cpu.explainRegisters();
    cpu.explainProtectedMode();
    cpu.explainPaging();
}

void COAIntegration::demonstrateCOA_CO2_Assembly() {
    Utils::printHeader("COA CO2: 80386 ASSEMBLY LANGUAGE PROGRAMS");
    AssemblyDemo::displayBillingAssemblyCode();
    AssemblyDemo::stepThroughBillingExecution();
    AssemblyDemo::explainInstructionSetDetails();
    AssemblyDemo::displayCompilationGuide();
}

void COAIntegration::demonstrateCOA_CO3_Arithmetic() {
    Utils::printHeader("COA CO3: COMPUTER ARITHMETIC & ALU ALGORITHMS");
    cpu.demonstrateBinaryArithmetic(500, 750);
    cpu.demonstrateCarryPropagation();

    std::cout << "\nSimulating 64-bit Multiplication (MUL EBX):\n";
    cpu.EAX = 50000;
    cpu.EBX = 100000;
    std::cout << "  Before MUL: EAX = " << cpu.EAX << ", EBX = " << cpu.EBX << "\n";
    cpu.MUL_reg("EBX");
    std::cout << "  After MUL : 64-bit Result in EDX:EAX\n";
    std::cout << "              EDX (High 32 bits) = 0x" << std::hex << cpu.EDX << std::dec << "\n";
    std::cout << "              EAX (Low 32 bits)  = 0x" << std::hex << cpu.EAX << " (" << std::dec << cpu.EAX << ")\n";
    cpu.displayFlags();
}

void COAIntegration::demonstrateCOA_CO4_PipelineControl() {
    Utils::printHeader("COA CO4: 5-STAGE INSTRUCTION PIPELINING & CONTROL SIGNALS");
    cpu.runCompletePipelineSimulation();
}

void COAIntegration::runCompleteCOADemo() {
    Utils::printHeader("COMPREHENSIVE COA DEMONSTRATION (CO1 TO CO4)");
    BillingCharges sampleCharges = {500, 750, 2000, 400, 3650};
    processHospitalBill(sampleCharges);
    demonstrateCOA_CO1_Architecture();
    demonstrateCOA_CO2_Assembly();
    demonstrateCOA_CO3_Arithmetic();
    demonstrateCOA_CO4_PipelineControl();
}

bool COAIntegration::exportBillingForAssembly(const Billing& bill, const std::string& filepath) {
    std::ofstream out(filepath);
    if (!out.is_open()) return false;

    out << "; ==============================================================================\n"
        << "; AUTO-GENERATED 80386 NASM ASSEMBLY FOR HOSPITAL BILL #" << bill.getBillId() << "\n"
        << "; Patient ID: P" << bill.getPatientId() << " | Date: " << bill.getBillingDate() << "\n"
        << "; ==============================================================================\n\n"
        << "section .data\n"
        << "    consultation_charge dd " << static_cast<uint32_t>(bill.getConsultationCharge()) << "\n"
        << "    medicine_charge     dd " << static_cast<uint32_t>(bill.getMedicineCharge()) << "\n"
        << "    room_charge         dd " << static_cast<uint32_t>(bill.getRoomCharge()) << "\n"
        << "    test_charge         dd " << static_cast<uint32_t>(bill.getTestCharge()) << "\n"
        << "    total_bill          dd 0\n\n"
        << "section .text\n"
        << "    global _start\n\n"
        << "_start:\n"
        << "    mov eax, [consultation_charge] ; Load consultation\n"
        << "    mov ebx, [medicine_charge]     ; Load medicine\n"
        << "    add eax, ebx                   ; Add medicine\n"
        << "    mov ebx, [room_charge]         ; Load room\n"
        << "    add eax, ebx                   ; Add room\n"
        << "    mov ebx, [test_charge]         ; Load test\n"
        << "    add eax, ebx                   ; Add test\n"
        << "    mov [total_bill], eax          ; Store calculated total\n"
        << "    mov ebx, eax                   ; Return total as exit code\n"
        << "    mov eax, 1                     ; sys_exit\n"
        << "    int 0x80\n";

    out.close();
    std::cout << "Successfully exported assembly file to: " << filepath << "\n";
    return true;
}
