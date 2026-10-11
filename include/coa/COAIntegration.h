#ifndef COA_INTEGRATION_H
#define COA_INTEGRATION_H

#include "coa/Processor80386.h"
#include "coa/AssemblyDemo.h"
#include "models/Billing.h"

// ============================================================================
// COA Integration Layer: Connects Hospital Billing to 80386 Simulation
// Demonstrates all 4 COA Course Outcomes through one cohesive billing pipeline
// ============================================================================
class COAIntegration {
private:
    Processor80386 cpu;

public:
    COAIntegration();

    // Process a hospital billing invoice through 80386 registers and ALU
    void processHospitalBill(const BillingCharges& charges);

    // Individual CO demonstrations
    void demonstrateCOA_CO1_Architecture();
    void demonstrateCOA_CO2_Assembly();
    void demonstrateCOA_CO3_Arithmetic();
    void demonstrateCOA_CO4_PipelineControl();

    // Comprehensive demonstration of all 4 COA outcomes
    void runCompleteCOADemo();

    // Export any live hospital bill into an executable NASM assembly file
    bool exportBillingForAssembly(const Billing& bill, const std::string& filepath);
};

#endif // COA_INTEGRATION_H
