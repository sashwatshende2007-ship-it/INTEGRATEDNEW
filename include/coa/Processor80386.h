#ifndef PROCESSOR_80386_H
#define PROCESSOR_80386_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <iomanip>

// ============================================================================
// COA CO1, CO3, CO4: Intel 80386 Microprocessor Architecture Simulation
// Simulates 32-bit registers, ALU flags, arithmetic, and 5-stage pipeline
// ============================================================================
class Processor80386 {
public:
    // 32-bit General Purpose Registers (CO1)
    uint32_t EAX; // Accumulator (used for I/O, arithmetic, return values, total bill)
    uint32_t EBX; // Base register (used for memory pointers, secondary charges)
    uint32_t ECX; // Counter register (used for loops and shifts)
    uint32_t EDX; // Data register (used for I/O, high 32 bits of 64-bit MUL/DIV)
    uint32_t ESI; // Source Index
    uint32_t EDI; // Destination Index
    uint32_t EBP; // Base Pointer
    uint32_t ESP; // Stack Pointer

    // Instruction Pointer & Status Flags (CO1 & CO3)
    uint32_t EIP;
    uint32_t EFLAGS;

    // Segment Registers (16-bit)
    uint16_t CS; // Code Segment
    uint16_t DS; // Data Segment
    uint16_t SS; // Stack Segment
    uint16_t ES; // Extra Segment
    uint16_t FS; // General Segment
    uint16_t GS; // General Segment

    // Control Registers (CO1 Protected Mode & Paging)
    uint32_t CR0; // Control Register 0: Bit 0 = PE (Protected Mode), Bit 31 = PG (Paging)
    uint32_t CR2; // Page Fault Linear Address Register
    uint32_t CR3; // Page Directory Base Register (PDBR)

    // EFLAGS individual boolean flags (CO3)
    bool CF; // Carry Flag
    bool PF; // Parity Flag
    bool AF; // Auxiliary Carry Flag
    bool ZF; // Zero Flag
    bool SF; // Sign Flag
    bool OF; // Overflow Flag
    bool IF; // Interrupt Enable Flag
    bool DF; // Direction Flag

    // Segment Descriptor for GDT/LDT
    struct SegmentDescriptor {
        uint32_t baseAddress;
        uint32_t limit;
        uint8_t accessByte;
        uint8_t flags;
        std::string description;
    };
    std::vector<SegmentDescriptor> GDT; // Global Descriptor Table
    std::vector<SegmentDescriptor> IDT; // Interrupt Descriptor Table

    // 5-Stage Pipeline Structures (CO4)
    enum PipelineStage { FETCH, DECODE, EXECUTE, MEMORY_ACCESS, WRITE_BACK, COMPLETED };

    struct PipelineInstruction {
        std::string mnemonic;
        std::string operands;
        uint32_t address;
        PipelineStage currentStage;
        bool hasDataHazard;
        std::string stallReason;
    };

    std::vector<PipelineInstruction> pipelineQueue;
    int cycleCount;

public:
    Processor80386();

    void reset();
    void updateEFLAGS();
    void syncFlagsFromEFLAGS();

    // CO1: Architecture, Registers, Protected Mode, and Paging Explanations
    void explainArchitecture() const;
    void explainRegisters() const;
    void explainProtectedMode() const;
    void explainPaging() const;
    void displayRegisterState() const;
    void displayFlags() const;

    // CO3: Computer Arithmetic & ALU Operations
    void demonstrateBinaryArithmetic(uint32_t a, uint32_t b);
    void demonstrateCarryPropagation();
    void updateFlagsAdd(uint32_t dest, uint32_t src, uint32_t result);
    void updateFlagsSub(uint32_t dest, uint32_t src, uint32_t result);

    // ALU Instructions
    void MOV_reg_imm(const std::string& reg, uint32_t imm);
    void ADD_reg_imm(const std::string& reg, uint32_t imm);
    void ADD_reg_reg(const std::string& destReg, const std::string& srcReg);
    void SUB_reg_imm(const std::string& reg, uint32_t imm);
    void SUB_reg_reg(const std::string& destReg, const std::string& srcReg);
    void MUL_reg(const std::string& srcReg); // EDX:EAX = EAX * srcReg
    void DIV_reg(const std::string& srcReg); // EAX = EDX:EAX / srcReg, EDX = remainder
    void XOR_reg_reg(const std::string& reg1, const std::string& reg2);

    // CO4: 5-Stage Instruction Pipelining & Control Signal Generation
    void loadPipelineInstructions(const std::vector<std::pair<std::string, std::string>>& instrs);
    void advancePipelineCycle();
    void displayPipelineStatus() const;
    void displayControlSignalsForStage(PipelineStage stage) const;
    void runCompletePipelineSimulation();
};

#endif // PROCESSOR_80386_H
