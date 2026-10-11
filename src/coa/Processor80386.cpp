#include "coa/Processor80386.h"
#include "utilities/Utils.h"
#include <bitset>
#include <sstream>

Processor80386::Processor80386() {
    reset();
}

void Processor80386::reset() {
    EAX = EBX = ECX = EDX = 0;
    ESI = EDI = EBP = ESP = 0x00100000; // Base stack pointer
    EIP = 0x00007C00;
    EFLAGS = 0x00000002; // Bit 1 is always 1 in x86
    CS = 0x0008; // Kernel code segment selector
    DS = SS = ES = FS = GS = 0x0010; // Kernel data segment selector
    CR0 = 0x80000001; // PE (Protected Mode) bit 0 + PG (Paging) bit 31 set
    CR2 = 0;
    CR3 = 0x00020000; // Page Directory Base
    CF = PF = AF = ZF = SF = OF = false;
    IF = true;
    DF = false;
    cycleCount = 0;
    pipelineQueue.clear();

    // Setup simulated GDT entries
    GDT.clear();
    GDT.push_back({0x00000000, 0x00000000, 0x00, 0x00, "Null Descriptor (Selector 0x00)"});
    GDT.push_back({0x00000000, 0xFFFFFFFF, 0x9A, 0xCF, "Kernel Code Segment (Selector 0x08, DPL=0, 4GB)"});
    GDT.push_back({0x00000000, 0xFFFFFFFF, 0x92, 0xCF, "Kernel Data Segment (Selector 0x10, DPL=0, 4GB)"});
    GDT.push_back({0x00000000, 0xFFFFFFFF, 0xFA, 0xCF, "User Code Segment   (Selector 0x1B, DPL=3, 4GB)"});
    GDT.push_back({0x00000000, 0xFFFFFFFF, 0xF2, 0xCF, "User Data Segment   (Selector 0x23, DPL=3, 4GB)"});
}

void Processor80386::updateEFLAGS() {
    EFLAGS = 0x00000002;
    if (CF) EFLAGS |= (1 << 0);
    if (PF) EFLAGS |= (1 << 2);
    if (AF) EFLAGS |= (1 << 4);
    if (ZF) EFLAGS |= (1 << 6);
    if (SF) EFLAGS |= (1 << 7);
    if (IF) EFLAGS |= (1 << 9);
    if (DF) EFLAGS |= (1 << 10);
    if (OF) EFLAGS |= (1 << 11);
}

void Processor80386::syncFlagsFromEFLAGS() {
    CF = (EFLAGS & (1 << 0)) != 0;
    PF = (EFLAGS & (1 << 2)) != 0;
    AF = (EFLAGS & (1 << 4)) != 0;
    ZF = (EFLAGS & (1 << 6)) != 0;
    SF = (EFLAGS & (1 << 7)) != 0;
    IF = (EFLAGS & (1 << 9)) != 0;
    DF = (EFLAGS & (1 << 10)) != 0;
    OF = (EFLAGS & (1 << 11)) != 0;
}

// ----------------------------------------------------------------------------
// COA CO1: Architecture, Registers, Protected Mode, Paging
// ----------------------------------------------------------------------------
void Processor80386::explainArchitecture() const {
    Utils::printSubHeader("INTEL 80386 MICROPROCESSOR ARCHITECTURE OVERVIEW (COA CO1)");
    std::cout << "The Intel 80386 is a milestone 32-bit CISC microprocessor featuring:\n"
              << "  - 32-bit General Purpose Registers (EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP)\n"
              << "  - 32-bit Data Bus (4 GB physical memory addressability)\n"
              << "  - 32-bit Address Bus (32-bit linear address space with two-level Paging)\n"
              << "  - Segmented Memory Model with Descriptor Tables (GDT, LDT, IDT)\n"
              << "  - 4 Privilege Levels (Ring 0 Kernel to Ring 3 User application)\n"
              << "  - On-chip Memory Management Unit (MMU) supporting Segmentation and Paging\n"
              << "  - 5-stage conceptual pipelining (Fetch, Decode, Execute, Memory, Writeback)\n\n";
    displayRegisterState();
}

void Processor80386::explainRegisters() const {
    Utils::printSubHeader("80386 REGISTER SET DETAILS (COA CO1)");
    std::cout << "  EAX: Accumulator (Billing subtotal and final invoice accumulator)\n"
              << "  EBX: Base Register (Secondary charge operand, memory pointers)\n"
              << "  ECX: Count Register (Loop iterations, shift/rotate counts)\n"
              << "  EDX: Data Register (High 32 bits for 64-bit MUL/DIV products)\n"
              << "  ESI: Source Index (String operations and table pointers)\n"
              << "  EDI: Destination Index (String destination and array writes)\n"
              << "  EBP: Base Pointer (Stack frame base for function arguments)\n"
              << "  ESP: Stack Pointer (Current top of the runtime stack)\n"
              << "  EIP: Instruction Pointer (Points to the next opcode to fetch)\n"
              << "  EFLAGS: Status & Control Flags (CF, PF, AF, ZF, SF, OF, IF, DF)\n\n";
}

void Processor80386::explainProtectedMode() const {
    Utils::printSubHeader("80386 PROTECTED MODE ARCHITECTURE (COA CO1)");
    std::cout << "Protected Mode Operation:\n"
              << "  - Control Register CR0 Bit 0 (PE - Protection Enable): " 
              << ((CR0 & 1) ? "1 (ENABLED - 32-bit Protected Mode Active)" : "0 (Real Mode)") << "\n"
              << "  - Segment Registers (CS, DS, SS, ES) act as 16-bit SELECTORS into GDT/LDT:\n"
              << "      Bits 15-3: Index into Descriptor Table\n"
              << "      Bit 2:     Table Indicator (0 = GDT, 1 = LDT)\n"
              << "      Bits 1-0:  Requested Privilege Level (RPL, 00=Ring 0, 11=Ring 3)\n"
              << "\nGlobal Descriptor Table (GDT) Layout:\n";
    for (size_t i = 0; i < GDT.size(); ++i) {
        std::cout << "  Entry [" << i << "]: " << GDT[i].description
                  << " | Base=0x" << std::hex << std::setw(8) << std::setfill('0') << GDT[i].baseAddress
                  << " | Limit=0x" << GDT[i].limit 
                  << " | Access=0x" << (int)GDT[i].accessByte << std::dec << "\n";
    }
}

void Processor80386::explainPaging() const {
    Utils::printSubHeader("80386 TWO-LEVEL PAGING MECHANISM (COA CO1)");
    std::cout << "Paging Translation Pipeline:\n"
              << "  - CR0 Bit 31 (PG - Paging Enable): " << ((CR0 & 0x80000000) ? "1 (PAGING ACTIVE)" : "0 (Disabled)") << "\n"
              << "  - CR3 Register (PDBR): 0x" << std::hex << std::setw(8) << std::setfill('0') << CR3 << std::dec
              << " (Points to physical base of Page Directory)\n"
              << "  - 32-bit Linear Address Breakdown: [Directory: 10 bits | Table: 10 bits | Offset: 12 bits]\n"
              << "      1. Bits 31-22 (10 bits) -> Index into Page Directory (1024 entries x 4 bytes = 4KB)\n"
              << "      2. Bits 21-12 (10 bits) -> Index into Page Table (1024 entries x 4 bytes = 4KB)\n"
              << "      3. Bits 11-0  (12 bits) -> Offset into 4KB Physical Page Frame\n"
              << "  - Total Addressable Virtual Memory per process: 1024 x 1024 x 4KB = 4 GB\n\n";
}

void Processor80386::displayRegisterState() const {
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << "                  80386 REGISTER SNAPSHOT                             \n";
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << " EAX: 0x" << std::hex << std::setw(8) << std::setfill('0') << EAX 
              << " (" << std::dec << EAX << ")"
              << "\tEBX: 0x" << std::hex << std::setw(8) << std::setfill('0') << EBX 
              << " (" << std::dec << EBX << ")\n";
    std::cout << " ECX: 0x" << std::hex << std::setw(8) << std::setfill('0') << ECX 
              << " (" << std::dec << ECX << ")"
              << "\tEDX: 0x" << std::hex << std::setw(8) << std::setfill('0') << EDX 
              << " (" << std::dec << EDX << ")\n";
    std::cout << " ESI: 0x" << std::hex << std::setw(8) << std::setfill('0') << ESI
              << "\tEDI: 0x" << std::hex << std::setw(8) << std::setfill('0') << EDI << "\n";
    std::cout << " EBP: 0x" << std::hex << std::setw(8) << std::setfill('0') << EBP
              << "\tESP: 0x" << std::hex << std::setw(8) << std::setfill('0') << ESP << "\n";
    std::cout << " EIP: 0x" << std::hex << std::setw(8) << std::setfill('0') << EIP
              << "\tEFLAGS: 0x" << std::hex << std::setw(8) << std::setfill('0') << EFLAGS << std::dec << "\n";
    std::cout << " CS:  0x" << std::hex << CS << "  DS: 0x" << DS << "  SS: 0x" << SS << "  ES: 0x" << ES << std::dec << "\n";
    std::cout << " CR0: 0x" << std::hex << CR0 << " (PE=" << (CR0&1) << ", PG=" << ((CR0>>31)&1) << ")"
              << "  CR3: 0x" << CR3 << std::dec << "\n";
    displayFlags();
    std::cout << "----------------------------------------------------------------------\n";
}

void Processor80386::displayFlags() const {
    std::cout << " Flags: [CF=" << CF << "] [PF=" << PF << "] [AF=" << AF 
              << "] [ZF=" << ZF << "] [SF=" << SF << "] [OF=" << OF 
              << "] [IF=" << IF << "] [DF=" << DF << "]\n";
}

// ----------------------------------------------------------------------------
// COA CO3: Computer Arithmetic & ALU Flag Updates
// ----------------------------------------------------------------------------
void Processor80386::updateFlagsAdd(uint32_t dest, uint32_t src, uint32_t result) {
    CF = (result < dest); // Unsigned carry
    // Signed overflow: signs of operands equal, but result sign differs
    OF = ((dest ^ src) & 0x80000000) == 0 && ((dest ^ result) & 0x80000000) != 0;
    ZF = (result == 0);
    SF = (result & 0x80000000) != 0;
    // Parity of lower 8 bits (1 if even number of 1s)
    uint8_t lowByte = result & 0xFF;
    int ones = 0;
    for (int i = 0; i < 8; ++i) if (lowByte & (1 << i)) ++ones;
    PF = (ones % 2 == 0);
    // Auxiliary carry: carry from bit 3 to bit 4
    AF = ((dest & 0x0F) + (src & 0x0F)) > 0x0F;

    updateEFLAGS();
}

void Processor80386::updateFlagsSub(uint32_t dest, uint32_t src, uint32_t result) {
    CF = (dest < src); // Unsigned borrow
    OF = ((dest ^ src) & 0x80000000) != 0 && ((dest ^ result) & 0x80000000) != 0;
    ZF = (result == 0);
    SF = (result & 0x80000000) != 0;
    uint8_t lowByte = result & 0xFF;
    int ones = 0;
    for (int i = 0; i < 8; ++i) if (lowByte & (1 << i)) ++ones;
    PF = (ones % 2 == 0);
    AF = (dest & 0x0F) < (src & 0x0F);

    updateEFLAGS();
}

void Processor80386::demonstrateBinaryArithmetic(uint32_t a, uint32_t b) {
    Utils::printSubHeader("BIT-BY-BIT RIPPLE-CARRY ADDITION SIMULATION (COA CO3)");
    std::cout << "Simulating Hardware Full Adder for 32-bit addition:\n";
    std::cout << "  Operand A: 0x" << std::hex << std::setw(8) << std::setfill('0') << a 
              << " (" << std::dec << a << ")\n";
    std::cout << "  Operand B: 0x" << std::hex << std::setw(8) << std::setfill('0') << b 
              << " (" << std::dec << b << ")\n\n";

    bool carry = false;
    uint32_t sum = 0;

    std::cout << " Bit Step (showing lower 8 bits ripple carry demonstration):\n";
    std::cout << " Pos | BitA | BitB | Cin | SumBit | Cout\n";
    std::cout << " ----+------+------+-----+--------+-----\n";

    for (int i = 0; i < 32; ++i) {
        bool bitA = (a >> i) & 1;
        bool bitB = (b >> i) & 1;
        bool sumBit = bitA ^ bitB ^ carry;
        bool nextCarry = (bitA & bitB) | (carry & (bitA ^ bitB));

        if (i < 8) {
            std::cout << " " << std::setw(3) << i << " | "
                      << std::setw(4) << bitA << " | "
                      << std::setw(4) << bitB << " | "
                      << std::setw(3) << carry << " | "
                      << std::setw(6) << sumBit << " | "
                      << std::setw(4) << nextCarry << "\n";
        }

        if (sumBit) sum |= (1U << i);
        carry = nextCarry;
    }

    std::cout << "\n  Final 32-bit Sum Result : 0x" << std::hex << std::setw(8) << std::setfill('0') << sum
              << " (" << std::dec << sum << ")\n";
    std::cout << "  Final Carry Out (CF)   : " << carry << "\n";
}

void Processor80386::demonstrateCarryPropagation() {
    Utils::printSubHeader("CARRY PROPAGATION OVERFLOW TEST (COA CO3)");
    uint32_t maxVal = 0xFFFFFFFF;
    uint32_t one = 1;
    std::cout << "Adding 0xFFFFFFFF + 0x00000001 (Simulating ripple-carry across all 32 bits):\n";
    uint32_t res = maxVal + one;
    updateFlagsAdd(maxVal, one, res);
    std::cout << "  0xFFFFFFFF + 1 = 0x" << std::hex << std::setw(8) << std::setfill('0') << res << std::dec << "\n";
    std::cout << "  Resulting Flags -> Carry Flag (CF)=" << CF << " (Overflow past 32 bits detected!)\n";
    std::cout << "                     Zero Flag  (ZF)=" << ZF << " (Result wrapped to exactly 0)\n";
    std::cout << "                     Aux Carry  (AF)=" << AF << "\n\n";
}

// ----------------------------------------------------------------------------
// ALU Instructions
// ----------------------------------------------------------------------------
void Processor80386::MOV_reg_imm(const std::string& reg, uint32_t imm) {
    if (reg == "EAX") EAX = imm;
    else if (reg == "EBX") EBX = imm;
    else if (reg == "ECX") ECX = imm;
    else if (reg == "EDX") EDX = imm;
}

void Processor80386::ADD_reg_imm(const std::string& reg, uint32_t imm) {
    if (reg == "EAX") {
        uint32_t prev = EAX;
        EAX += imm;
        updateFlagsAdd(prev, imm, EAX);
    } else if (reg == "EBX") {
        uint32_t prev = EBX;
        EBX += imm;
        updateFlagsAdd(prev, imm, EBX);
    }
}

void Processor80386::ADD_reg_reg(const std::string& destReg, const std::string& srcReg) {
    uint32_t srcVal = (srcReg == "EBX") ? EBX : (srcReg == "ECX") ? ECX : EDX;
    if (destReg == "EAX") {
        uint32_t prev = EAX;
        EAX += srcVal;
        updateFlagsAdd(prev, srcVal, EAX);
    }
}

void Processor80386::SUB_reg_imm(const std::string& reg, uint32_t imm) {
    if (reg == "EAX") {
        uint32_t prev = EAX;
        EAX -= imm;
        updateFlagsSub(prev, imm, EAX);
    }
}

void Processor80386::SUB_reg_reg(const std::string& destReg, const std::string& srcReg) {
    uint32_t srcVal = (srcReg == "EBX") ? EBX : (srcReg == "ECX") ? ECX : EDX;
    if (destReg == "EAX") {
        uint32_t prev = EAX;
        EAX -= srcVal;
        updateFlagsSub(prev, srcVal, EAX);
    }
}

void Processor80386::MUL_reg(const std::string& srcReg) {
    uint32_t srcVal = (srcReg == "EBX") ? EBX : (srcReg == "ECX") ? ECX : EDX;
    uint64_t prod = static_cast<uint64_t>(EAX) * static_cast<uint64_t>(srcVal);
    EAX = static_cast<uint32_t>(prod & 0xFFFFFFFF);
    EDX = static_cast<uint32_t>((prod >> 32) & 0xFFFFFFFF);
    CF = OF = (EDX != 0); // If high 32 bits not zero
    updateEFLAGS();
}

void Processor80386::DIV_reg(const std::string& srcReg) {
    uint32_t divisor = (srcReg == "EBX") ? EBX : (srcReg == "ECX") ? ECX : EDX;
    if (divisor == 0) {
        std::cerr << "80386 Hardware Interrupt 0: Divide Error Exception (#DE)!\n";
        return;
    }
    uint64_t dividend = (static_cast<uint64_t>(EDX) << 32) | EAX;
    uint64_t quotient = dividend / divisor;
    uint32_t remainder = static_cast<uint32_t>(dividend % divisor);
    EAX = static_cast<uint32_t>(quotient);
    EDX = remainder;
}

void Processor80386::XOR_reg_reg(const std::string& reg1, const std::string& reg2) {
    if (reg1 == "EAX" && reg2 == "EAX") {
        EAX = 0;
        ZF = true; SF = false; CF = false; OF = false; PF = true; AF = false;
        updateEFLAGS();
    } else if (reg1 == "EDX" && reg2 == "EDX") {
        EDX = 0;
        ZF = true; SF = false; CF = false; OF = false; PF = true; AF = false;
        updateEFLAGS();
    }
}

// ----------------------------------------------------------------------------
// COA CO4: 5-Stage Instruction Pipelining & Control Signal Generation
// ----------------------------------------------------------------------------
void Processor80386::loadPipelineInstructions(const std::vector<std::pair<std::string, std::string>>& instrs) {
    pipelineQueue.clear();
    cycleCount = 0;
    uint32_t addr = 0x00401000;
    for (const auto& item : instrs) {
        pipelineQueue.push_back({item.first, item.second, addr, FETCH, false, ""});
        addr += 4;
    }
}

void Processor80386::displayControlSignalsForStage(PipelineStage stage) const {
    switch (stage) {
        case FETCH:
            std::cout << "  Control Signals [FETCH]     : PC_OUT=1, MEM_READ=1, IR_IN=1, ALU_SEL=00, REG_WRITE=0\n";
            break;
        case DECODE:
            std::cout << "  Control Signals [DECODE]    : REG_READ=1, IMM_EXTEND=1, OPCODE_DECODE=1, CTRL_ROM=ACTIVE\n";
            break;
        case EXECUTE:
            std::cout << "  Control Signals [EXECUTE]   : ALU_OP=ADD/SUB, ALU_SRC_B=REG/IMM, REG_DEST=EAX, FLAG_WRITE=1\n";
            break;
        case MEMORY_ACCESS:
            std::cout << "  Control Signals [MEMORY]    : MEM_READ=0, MEM_WRITE=0, DATA_BUS=IDLE (Register Operation)\n";
            break;
        case WRITE_BACK:
            std::cout << "  Control Signals [WRITEBACK] : REG_WRITE=1, WB_MUX=ALU_RESULT, DEST=REG_FILE\n";
            break;
        default:
            break;
    }
}

void Processor80386::advancePipelineCycle() {
    cycleCount++;
    std::cout << "\n>>> Pipeline Clock Cycle " << cycleCount << " <<<\n";

    // Detect RAW Data Hazard between adjacent instructions
    for (size_t i = 1; i < pipelineQueue.size(); ++i) {
        if (pipelineQueue[i].currentStage == DECODE &&
            pipelineQueue[i - 1].currentStage == EXECUTE &&
            pipelineQueue[i - 1].mnemonic == "MOV" &&
            pipelineQueue[i].mnemonic == "ADD") {
            pipelineQueue[i].hasDataHazard = true;
            pipelineQueue[i].stallReason = "RAW Data Hazard on EAX/EBX (Resolved via Operand Forwarding)";
        }
    }

    // Advance backwards so instructions don't overwrite each other in one step
    for (int i = static_cast<int>(pipelineQueue.size()) - 1; i >= 0; --i) {
        auto& inst = pipelineQueue[i];
        if (inst.currentStage == COMPLETED) continue;

        // Print stage execution
        std::string stageName;
        switch (inst.currentStage) {
            case FETCH: stageName = "FETCH     "; break;
            case DECODE: stageName = "DECODE    "; break;
            case EXECUTE: stageName = "EXECUTE   "; break;
            case MEMORY_ACCESS: stageName = "MEM_ACCESS"; break;
            case WRITE_BACK: stageName = "WRITE_BACK"; break;
            default: stageName = "DONE      "; break;
        }

        std::cout << "  [" << stageName << "] 0x" << std::hex << inst.address << std::dec << ": "
                  << std::setw(5) << inst.mnemonic << " " << std::setw(15) << inst.operands;
        if (inst.hasDataHazard) {
            std::cout << " [" << inst.stallReason << "]";
        }
        std::cout << "\n";

        // Step to next stage
        if (inst.currentStage == WRITE_BACK) {
            inst.currentStage = COMPLETED;
        } else {
            inst.currentStage = static_cast<PipelineStage>(inst.currentStage + 1);
        }
    }
}

void Processor80386::runCompletePipelineSimulation() {
    Utils::printSubHeader("5-STAGE PIPELINE EXECUTION FOR HOSPITAL BILLING (COA CO4)");
    std::vector<std::pair<std::string, std::string>> billingSequence = {
        {"MOV", "EAX, 500  ; Consultation"},
        {"MOV", "EBX, 750  ; Medicine"},
        {"ADD", "EAX, EBX  ; EAX = 1250"},
        {"MOV", "EBX, 2000 ; Room"},
        {"ADD", "EAX, EBX  ; EAX = 3250"},
        {"MOV", "EBX, 400  ; Test"},
        {"ADD", "EAX, EBX  ; EAX = 3650"}
    };

    loadPipelineInstructions(billingSequence);
    std::cout << "Pipelining 7 instructions across 5 stages:\n"
              << "  Sequential cycles required without pipelining : 7 * 5 = 35 cycles\n"
              << "  Pipelined theoretical cycles required         : (k + n - 1) = 5 + 7 - 1 = 11 cycles\n"
              << "  Speedup factor                                : ~3.18x\n";

    displayControlSignalsForStage(FETCH);
    displayControlSignalsForStage(DECODE);
    displayControlSignalsForStage(EXECUTE);
    displayControlSignalsForStage(MEMORY_ACCESS);
    displayControlSignalsForStage(WRITE_BACK);

    bool allCompleted = false;
    while (!allCompleted) {
        advancePipelineCycle();
        allCompleted = true;
        for (const auto& inst : pipelineQueue) {
            if (inst.currentStage != COMPLETED) {
                allCompleted = false;
                break;
            }
        }
    }
    std::cout << "\nAll 7 instructions completed through pipeline in " << cycleCount << " clock cycles!\n";
}
