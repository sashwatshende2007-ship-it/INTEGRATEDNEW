#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>

#include "models/Hospital.h"
#include "coa/COAIntegration.h"
#include "graphics/HospitalGraphics.h"
#include "utilities/Utils.h"

// ============================================================================
// SMART HOSPITAL MANAGEMENT SYSTEM - MAIN APPLICATION ENTRY POINT
// Institution : MIT Academy of Engineering
// Courses     : PSOOP (2310261L), DS/PL (2310214L), COA (2310221L), CG (2310218L)
// Team Members: Sashwat Shende, Smit Redij, Vighnesh Chityal, Vedant Thakare
// ============================================================================

void printBanner() {
    std::cout << "\n";
    std::cout << "======================================================================\n";
    std::cout << "                 SMART HOSPITAL MANAGEMENT SYSTEM                     \n";
    std::cout << "                   MIT ACADEMY OF ENGINEERING                         \n";
    std::cout << "======================================================================\n";
    std::cout << " Integrated Course Outcomes Demonstration:                            \n";
    std::cout << "  - PSOOP (2310261L): OOP Domain Models, Inheritance, Polymorphism    \n";
    std::cout << "  - DS/PL (2310214L): FixedArray, LinkedList, Stack, Queues, Hash     \n";
    std::cout << "  - COA   (2310221L): 80386 Processor Simulation, Assembly, Pipeline   \n";
    std::cout << "  - CG    (2310218L): OpenGL Dashboard, Top-Down View, Emergency Sim   \n";
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << " Team Members:                                                        \n";
    std::cout << "  - Sashwat Shende (PRN: 202501100091)                                \n";
    std::cout << "  - Smit Redij    (PRN: 202501100085)                                \n";
    std::cout << "  - Vighnesh Chityal (PRN: 202501100084)                             \n";
    std::cout << "  - Vedant Thakare (PRN: 202501100092)                                \n";
    std::cout << "======================================================================\n";
}

void printMainMenu() {
    std::cout << "\n=========================== MAIN MENU ============================\n";
    std::cout << "  1.  Patient Management     (Registration, Search, Condition Update)\n";
    std::cout << "  2.  Doctor Management      (Roster, Departments, Availability)    \n";
    std::cout << "  3.  Appointment System     (FIFO Queue Scheduling - DS CO3)        \n";
    std::cout << "  4.  Bed & Ward Management  (FixedArray Ward Beds - DS CO1)         \n";
    std::cout << "  5.  Emergency Triage       (Min-Heap Priority Queue - DS CO3)      \n";
    std::cout << "  6.  Pharmacy & Dispensing  (Inventory, Stock, Reorder Alerts)      \n";
    std::cout << "  7.  Medical Records        (Doubly Linked List Chain - DS CO2)     \n";
    std::cout << "  8.  Billing & Invoices     (Itemized Billing, Payment, Export)     \n";
    std::cout << "  9.  COA 80386 Simulation   (Architecture, ASM, Arithmetic, Pipe)   \n";
    std::cout << "  10. Hospital Visualization (OpenGL Architectural Top-Down View)    \n";
    std::cout << "  11. Emergency Simulation   (OpenGL Real-time Cart Movement)        \n";
    std::cout << "  12. Hospital System Status (Comprehensive Statistics & Audit Stack)\n";
    std::cout << "  13. Exit Application                                               \n";
    std::cout << "==================================================================\n";
    std::cout << "Enter your choice [1 - 13]: ";
}

// ----------------------------------------------------------------------------
// Submenu Handlers
// ----------------------------------------------------------------------------
void handlePatientManagement(Hospital* hosp) {
    Utils::printHeader("PATIENT MANAGEMENT MODULE (PSOOP CO1, CO3)");
    std::cout << "1. Register New Patient\n";
    std::cout << "2. View All Registered Patients\n";
    std::cout << "3. Search Patient by ID\n";
    std::cout << "4. Update Patient Condition\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }
    std::cin.ignore();

    if (ch == 1) {
        std::string name, gender, contact, address, bg, condition;
        int age;
        char emgChar;
        std::cout << "Enter Patient Name: "; std::getline(std::cin, name);
        std::cout << "Enter Gender (Male/Female/Other): "; std::getline(std::cin, gender);
        std::cout << "Enter Contact Number: "; std::getline(std::cin, contact);
        std::cout << "Enter Address: "; std::getline(std::cin, address);
        std::cout << "Enter Age: "; std::cin >> age; std::cin.ignore();
        std::cout << "Enter Blood Group (A+, A-, B+, B-, AB+, AB-, O+, O-): "; std::getline(std::cin, bg);
        std::cout << "Enter Medical Condition: "; std::getline(std::cin, condition);
        std::cout << "Is this an Emergency case? (y/n): "; std::cin >> emgChar;

        bool emg = (emgChar == 'y' || emgChar == 'Y');
        bool ok = hosp->registerPatient(name, gender, contact, address, age, bg, condition, emg);
        if (ok) {
            std::cout << "\n>>> Patient registered successfully! <<< \n";
            if (emg) std::cout << ">>> Emergency patient automatically placed in Min-Heap Priority Queue! <<<\n";
        } else {
            std::cout << "\nFailed to register patient due to validation error.\n";
        }
    } else if (ch == 2) {
        hosp->listAllPatients();
    } else if (ch == 3) {
        std::string pid;
        std::cout << "Enter Patient ID (e.g. P101): ";
        std::cin >> pid;
        Patient* p = hosp->findPatient(pid);
        if (p) p->displayDetails();
        else std::cout << "Patient not found!\n";
    } else if (ch == 4) {
        std::string pid, cond;
        std::cout << "Enter Patient ID (e.g. P101): "; std::cin >> pid; std::cin.ignore();
        std::cout << "Enter Updated Medical Condition: "; std::getline(std::cin, cond);
        if (hosp->updatePatientCondition(pid, cond)) {
            std::cout << "Patient condition updated successfully!\n";
        } else {
            std::cout << "Patient not found or invalid condition.\n";
        }
    }
}

void handleDoctorManagement(Hospital* hosp) {
    Utils::printHeader("DOCTOR MANAGEMENT MODULE (PSOOP CO1)");
    std::cout << "1. List All Doctors\n";
    std::cout << "2. Filter Doctors by Department\n";
    std::cout << "3. Register New Doctor\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }
    std::cin.ignore();

    if (ch == 1) {
        hosp->listAllDoctors();
    } else if (ch == 2) {
        std::string dept;
        std::cout << "Enter Department Name (Cardiology/Neurology/Orthopedics/Emergency): ";
        std::getline(std::cin, dept);
        hosp->listDoctorsByDepartment(dept);
    } else if (ch == 3) {
        std::string name, gender, contact, address, spec, dept, qual;
        int age, exp, maxP;
        double fee;
        std::cout << "Doctor Name: "; std::getline(std::cin, name);
        std::cout << "Gender: "; std::getline(std::cin, gender);
        std::cout << "Contact: "; std::getline(std::cin, contact);
        std::cout << "Address: "; std::getline(std::cin, address);
        std::cout << "Age: "; std::cin >> age; std::cin.ignore();
        std::cout << "Specialization: "; std::getline(std::cin, spec);
        std::cout << "Department: "; std::getline(std::cin, dept);
        std::cout << "Qualification: "; std::getline(std::cin, qual);
        std::cout << "Experience (Years): "; std::cin >> exp;
        std::cout << "Consultation Fee (Rs): "; std::cin >> fee;
        std::cout << "Max Patients Capacity: "; std::cin >> maxP;

        if (hosp->addDoctor(name, gender, contact, address, age, spec, dept, qual, exp, fee, maxP)) {
            std::cout << "Doctor registered successfully!\n";
        } else {
            std::cout << "Error registering doctor.\n";
        }
    }
}

void handleAppointmentSystem(Hospital* hosp) {
    Utils::printHeader("APPOINTMENT QUEUE MANAGEMENT (DS/PL CO3 FIFO QUEUE)");
    std::cout << "1. Schedule New Appointment\n";
    std::cout << "2. View All Appointments\n";
    std::cout << "3. Display Appointment FIFO Queue\n";
    std::cout << "4. Process Next Scheduled Appointment in Queue\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }

    if (ch == 1) {
        int pId, dId, priority;
        std::string dt, reason;
        std::cout << "Enter Patient Numeric ID (e.g., 101): "; std::cin >> pId;
        std::cout << "Enter Doctor Numeric ID (e.g., 201): "; std::cin >> dId; std::cin.ignore();
        std::cout << "Enter Date/Time (e.g., 2026-10-15 10:30 AM): "; std::getline(std::cin, dt);
        std::cout << "Enter Reason: "; std::getline(std::cin, reason);
        std::cout << "Priority (1=Emergency, 2=Urgent, 3=Normal): "; std::cin >> priority;

        if (hosp->scheduleAppointment(pId, dId, dt, reason, priority)) {
            std::cout << "Appointment successfully scheduled and enqueued in FIFO queue!\n";
        } else {
            std::cout << "Could not schedule appointment.\n";
        }
    } else if (ch == 2) {
        hosp->listAllAppointments();
    } else if (ch == 3) {
        hosp->displayAppointmentQueue();
    } else if (ch == 4) {
        int processedId;
        if (hosp->processNextAppointment(processedId)) {
            std::cout << "Processed Appointment #" << processedId << " from queue!\n";
        } else {
            std::cout << "Appointment queue is currently empty.\n";
        }
    }
}

void handleBedManagement(Hospital* hosp) {
    Utils::printHeader("BED & WARD MANAGEMENT (DS/PL CO1 FIXED ARRAY)");
    std::cout << "1. View All Hospital Beds\n";
    std::cout << "2. View Available Vacant Beds\n";
    std::cout << "3. View Ward 1 Beds (DS CO1 FixedArray)\n";
    std::cout << "4. Assign Bed to Patient\n";
    std::cout << "5. Discharge Patient from Bed\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }

    if (ch == 1) {
        hosp->listAllBeds();
    } else if (ch == 2) {
        hosp->listAvailableBeds();
    } else if (ch == 3) {
        hosp->displayWard1BedArray();
    } else if (ch == 4) {
        int bedId, pId;
        std::cout << "Enter Bed ID: "; std::cin >> bedId;
        std::cout << "Enter Patient Numeric ID (e.g., 101): "; std::cin >> pId;
        if (hosp->assignBedToPatient(bedId, pId)) {
            std::cout << "Bed #" << bedId << " successfully assigned to Patient P" << pId << "!\n";
        }
    } else if (ch == 5) {
        int bedId;
        std::cout << "Enter Bed ID to discharge: "; std::cin >> bedId;
        if (hosp->dischargePatientFromBed(bedId)) {
            std::cout << "Bed #" << bedId << " released and marked available!\n";
        } else {
            std::cout << "Bed is already vacant or does not exist.\n";
        }
    }
}

void handleEmergencyTriage(Hospital* hosp) {
    Utils::printHeader("EMERGENCY MANAGEMENT (DS/PL CO3 MIN-HEAP PRIORITY QUEUE)");
    std::cout << "1. Display Emergency Priority Queue\n";
    std::cout << "2. Add Critical Patient to Emergency Queue\n";
    std::cout << "3. Dequeue Highest Priority Emergency Case for Immediate Surgery/ICU\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }

    if (ch == 1) {
        hosp->displayEmergencyQueue();
    } else if (ch == 2) {
        int pId, priority;
        std::cout << "Enter Patient Numeric ID (e.g. 104): "; std::cin >> pId;
        std::cout << "Enter Triage Priority (1=Immediate Life-Threatening, 2=Urgent, 3=Serious): ";
        std::cin >> priority;
        hosp->addEmergencyPatient(pId, priority);
        std::cout << "Patient P" << pId << " added to Min-Heap Emergency Queue!\n";
    } else if (ch == 3) {
        int pId, priority;
        if (hosp->processNextEmergency(pId, priority)) {
            std::cout << "\n>>> TRIAGE ALERT: Calling Patient P" << pId 
                      << " (Priority " << priority << ") into Emergency Operation Room! <<<\n";
        } else {
            std::cout << "No pending emergency cases in queue.\n";
        }
    }
}

void handlePharmacy(Hospital* hosp) {
    Utils::printHeader("PHARMACY & DISPENSING MODULE (PSOOP CO1, CO3)");
    std::cout << "1. View All Medicines in Stock\n";
    std::cout << "2. Check Low-Stock Reorder Alerts\n";
    std::cout << "3. Dispense Medicine to Patient\n";
    std::cout << "4. Restock Medicine\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }

    if (ch == 1) {
        hosp->listAllMedicines();
    } else if (ch == 2) {
        hosp->listLowStockMedicines();
    } else if (ch == 3) {
        std::string mid;
        int qty;
        std::cout << "Enter Medicine ID (e.g. M001): "; std::cin >> mid;
        std::cout << "Enter Quantity to dispense: "; std::cin >> qty;
        if (hosp->dispenseMedicine(mid, qty)) {
            std::cout << "Dispensed " << qty << " units successfully!\n";
        }
    } else if (ch == 4) {
        std::string mid;
        int qty;
        std::cout << "Enter Medicine ID (e.g. M001): "; std::cin >> mid;
        std::cout << "Enter Quantity to add: "; std::cin >> qty;
        if (hosp->restockMedicine(mid, qty)) {
            std::cout << "Restocked " << qty << " units successfully!\n";
        }
    }
}

void handleMedicalRecords(Hospital* hosp) {
    Utils::printHeader("MEDICAL RECORDS HISTORY (DS/PL CO2 DOUBLY LINKED LIST)");
    std::cout << "1. View Patient Medical Record Chain\n";
    std::cout << "2. Add New Clinical Encounter Record\n";
    std::cout << "3. Display Global Doubly Linked List Structure\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }

    if (ch == 1) {
        int pId;
        std::cout << "Enter Patient Numeric ID (e.g. 101): "; std::cin >> pId;
        hosp->displayMedicalRecordsForPatient(pId);
    } else if (ch == 2) {
        int pId, dId;
        std::string diag, treat, notes;
        std::cout << "Patient Numeric ID: "; std::cin >> pId;
        std::cout << "Doctor Numeric ID: "; std::cin >> dId; std::cin.ignore();
        std::cout << "Diagnosis: "; std::getline(std::cin, diag);
        std::cout << "Treatment: "; std::getline(std::cin, treat);
        std::cout << "Clinical Notes: "; std::getline(std::cin, notes);
        if (hosp->addMedicalRecord(pId, dId, diag, treat, notes)) {
            std::cout << "Record appended to Patient's Doubly Linked List chain!\n";
        }
    } else if (ch == 3) {
        hosp->displayCompleteMedicalRecordChain();
    }
}

void handleBilling(Hospital* hosp, COAIntegration* coa) {
    Utils::printHeader("BILLING MODULE (PSOOP CO1 & COA BRIDGE)");
    std::cout << "1. View All Bills\n";
    std::cout << "2. Generate New Hospital Bill\n";
    std::cout << "3. Process Payment for Bill\n";
    std::cout << "4. Export Bill to NASM 80386 Assembly (.asm file)\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }

    if (ch == 1) {
        hosp->listAllBills();
    } else if (ch == 2) {
        int pId;
        double cons, med, room, test, tax, disc;
        std::cout << "Patient Numeric ID: "; std::cin >> pId;
        std::cout << "Doctor Consultation Charge (Rs): "; std::cin >> cons;
        std::cout << "Pharmacy Medicine Charge (Rs): "; std::cin >> med;
        std::cout << "Room/Bed Daily Charge (Rs): "; std::cin >> room;
        std::cout << "Lab/Diagnostic Test Charge (Rs): "; std::cin >> test;
        std::cout << "GST Tax Percentage (Default 18): "; std::cin >> tax;
        std::cout << "Discount Percentage (0 for none): "; std::cin >> disc;

        int bId = hosp->generateBill(pId, cons, med, room, test, tax, disc);
        std::cout << "\nBill #" << bId << " successfully generated!\n";
        Billing* b = hosp->findBill(bId);
        if (b) b->displayBill();
    } else if (ch == 3) {
        int bId;
        std::string method;
        std::cout << "Enter Bill ID: "; std::cin >> bId;
        std::cout << "Payment Method (Cash / Card / UPI): "; std::cin >> method;
        if (hosp->processBillPayment(bId, method)) {
            std::cout << "Payment successfully processed! Receipt marked PAID.\n";
        } else {
            std::cout << "Bill not found or already paid.\n";
        }
    } else if (ch == 4) {
        int bId;
        std::cout << "Enter Bill ID to export to NASM: "; std::cin >> bId;
        Billing* b = hosp->findBill(bId);
        if (b) {
            std::string path = "asm/exported_bill_" + std::to_string(bId) + ".asm";
            coa->exportBillingForAssembly(*b, path);
        } else {
            std::cout << "Bill not found!\n";
        }
    }
}

void handleCOAMenu(COAIntegration* coa, Hospital* hosp) {
    Utils::printHeader("80386 PROCESSOR SIMULATION MODULE (COA CO1 - CO4)");
    std::cout << "1. Run Complete COA Demonstration (CO1 - CO4 Standard Example: Rs. 3650)\n";
    std::cout << "2. COA CO1: 80386 Architecture, 32-bit Registers, Protected Mode, Paging\n";
    std::cout << "3. COA CO2: NASM/MASM Assembly Programs & Instruction Analysis\n";
    std::cout << "4. COA CO3: Computer Arithmetic, Ripple-Carry Bit Adder, ALU Flags\n";
    std::cout << "5. COA CO4: 5-Stage Instruction Pipeline Simulation & Control Signals\n";
    std::cout << "6. Process Active Patient Bill through 80386 Hardware Registers\n";
    std::cout << "Enter choice: ";
    int ch;
    if (!(std::cin >> ch)) { std::cin.clear(); std::cin.ignore(1000, '\n'); return; }

    switch (ch) {
        case 1: coa->runCompleteCOADemo(); break;
        case 2: coa->demonstrateCOA_CO1_Architecture(); break;
        case 3: coa->demonstrateCOA_CO2_Assembly(); break;
        case 4: coa->demonstrateCOA_CO3_Arithmetic(); break;
        case 5: coa->demonstrateCOA_CO4_PipelineControl(); break;
        case 6: {
            int bId;
            std::cout << "Enter Bill ID (e.g. 401): "; std::cin >> bId;
            Billing* b = hosp->findBill(bId);
            if (b) {
                coa->processHospitalBill(b->getChargesForCOA());
            } else {
                std::cout << "Bill not found! Running standard demo charges instead:\n";
                BillingCharges bc = {500, 750, 2000, 400, 3650};
                coa->processHospitalBill(bc);
            }
            break;
        }
        default: std::cout << "Invalid selection.\n"; break;
    }
}

// ----------------------------------------------------------------------------
// Main Application Loop
// ----------------------------------------------------------------------------
int main() {
    printBanner();

    // 1. Initialize Hospital Facade Singleton
    Hospital* hospital = Hospital::getInstance();

    // 2. Initialize COA Integration Layer
    COAIntegration coaModule;

    // 3. Initialize OpenGL Visualization Window
    HospitalGraphics* graphics = HospitalGraphics::getInstance();
    graphics->setHospitalReference(hospital);

    std::cout << "\nLaunching OpenGL Graphics Window in background thread (16:9 HD: 1280x720)...\n";
    graphics->runWindow(1280, 720);
    std::cout << "OpenGL Window initialized! You can interact via Console or GUI.\n";

    bool running = true;
    while (running) {
        printMainMenu();
        int choice;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                break; // Graceful exit if input stream closes
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid numeric choice [1 - 13].\n";
            continue;
        }

        switch (choice) {
            case 1:
                graphics->setActivePanel(1);
                handlePatientManagement(hospital);
                break;
            case 2:
                graphics->setActivePanel(2);
                handleDoctorManagement(hospital);
                break;
            case 3:
                graphics->setActivePanel(3);
                handleAppointmentSystem(hospital);
                break;
            case 4:
                graphics->setActivePanel(4);
                handleBedManagement(hospital);
                break;
            case 5:
                graphics->triggerEmergencySimulation();
                handleEmergencyTriage(hospital);
                break;
            case 6:
                graphics->setActivePanel(5);
                handlePharmacy(hospital);
                break;
            case 7:
                graphics->setActivePanel(12);
                handleMedicalRecords(hospital);
                break;
            case 8:
                graphics->setActivePanel(6);
                handleBilling(hospital, &coaModule);
                break;
            case 9:
                graphics->setActivePanel(9);
                handleCOAMenu(&coaModule, hospital);
                break;
            case 10:
                std::cout << "\nSwitching OpenGL Window to Architectural Top-Down View...\n";
                if (!graphics->getIsRunning()) {
                    graphics->runWindow(1280, 720);
                }
                graphics->setMode(HospitalGraphics::HOSPITAL_VIEW_MODE);
                HospitalGraphics::explainCG_CO4_ProjectionViewing();
                break;
            case 11:
                std::cout << "\nStarting Real-time Emergency Patient Cart Animation (ER -> ICU)...\n";
                if (!graphics->getIsRunning()) {
                    graphics->runWindow(1280, 720);
                }
                graphics->triggerEmergencySimulation();
                HospitalGraphics::explainCG_CO2_Transformations();
                break;
            case 12: {
                graphics->setActivePanel(13);
                Utils::printHeader("HOSPITAL SYSTEM STATUS & AUDIT TRAIL");
                std::cout << "  Total Patients Registered : " << hospital->getTotalPatients() << "\n";
                std::cout << "  Active Duty Doctors       : " << hospital->getTotalDoctors() << "\n";
                std::cout << "  Total Bed Capacity        : " << hospital->getTotalBeds() << "\n";
                std::cout << "  Occupied Beds             : " << hospital->getOccupiedBedCount() << "\n";
                std::cout << "  Available Beds            : " << hospital->getAvailableBedCount() << "\n";
                std::cout << "  Emergency Cases in Queue  : " << hospital->getEmergencyCount() << "\n";
                std::cout << "  Appointments Pending      : " << hospital->getPendingAppointmentsCount() << "\n";
                hospital->displayOperationHistory();
                break;
            }
            case 13:
                std::cout << "\nSaving data and shutting down Smart Hospital System...\n";
                hospital->saveDataToFiles();
                graphics->closeWindow();
                running = false;
                break;
            default:
                std::cout << "Invalid choice! Please select between 1 and 13.\n";
                break;
        }
    }

    graphics->closeWindow();
    std::cout << "Application successfully terminated. Thank you!\n";
    return 0;
}
