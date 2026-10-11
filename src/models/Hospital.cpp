#include "models/Hospital.h"
#include "utilities/Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

Hospital* Hospital::instance = nullptr;

Hospital* Hospital::getInstance() {
    if (!instance) {
        instance = new Hospital();
    }
    return instance;
}

Hospital::Hospital()
    : operationStack(50), emergencyQueue(), patientHashTable(31),
      nextPatientNumericId(101), nextDoctorNumericId(201),
      nextAppointmentId(301), nextBillId(401), nextRecordId(501) {
    initializeSampleData();
}

void Hospital::pushOperation(int opCode) {
    operationStack.push(opCode);
}

int Hospital::popOperation() {
    if (operationStack.isEmpty()) return -1;
    return operationStack.pop();
}

void Hospital::displayOperationHistory() const {
    Utils::printSubHeader("HOSPITAL OPERATION HISTORY (LIFO STACK - DS CO3)");
    operationStack.display();
}

void Hospital::initializeSampleData() {
    // 1. Doctors
    addDoctor("Sarah Jenkins", "Female", "+91-9876543210", "Pune", 42,
              "Cardiologist", "Cardiology", "MD, DM Cardiology", 15, 800.0, 12);
    addDoctor("Rajesh Sharma", "Male", "+91-9876543211", "Mumbai", 48,
              "Neurologist", "Neurology", "MS, MCh Neuro", 18, 1000.0, 10);
    addDoctor("Anita Desai", "Female", "+91-9876543212", "Pune", 36,
              "General Physician", "General Medicine", "MBBS, MD", 10, 500.0, 20);
    addDoctor("Vikram Patil", "Male", "+91-9876543213", "Nashik", 45,
              "Orthopedic Surgeon", "Orthopedics", "MS Ortho", 16, 750.0, 8);
    addDoctor("Pooja Nair", "Female", "+91-9876543214", "Pune", 39,
              "Emergency Specialist", "Emergency / ICU", "MD Emergency Medicine", 12, 600.0, 15);

    // 2. Beds (Wards 1, 2, 3)
    // Ward 1: General (slots 1 to 10) - Connected to FixedArray
    for (int i = 1; i <= 10; ++i) {
        addBed(1, "General", 500.0);
    }
    // Ward 2: Semi-Private & Private
    for (int i = 11; i <= 15; ++i) {
        addBed(2, "Semi-Private", 1200.0);
    }
    for (int i = 16; i <= 18; ++i) {
        addBed(2, "Private", 2500.0);
    }
    // Ward 3: ICU & Emergency
    for (int i = 19; i <= 22; ++i) {
        addBed(3, "ICU", 4000.0);
    }
    for (int i = 23; i <= 25; ++i) {
        addBed(3, "Emergency", 3500.0);
    }

    // 3. Pharmacy Stock
    addMedicine("M001", "Paracetamol", "Acetaminophen", "Antipyretic", "Cipla", 25.0, 200, 30, "2027-12-31", "B901");
    addMedicine("M002", "Amoxicillin", "Amoxicillin Clavulanate", "Antibiotic", "Sun Pharma", 120.0, 80, 20, "2027-06-30", "B902");
    addMedicine("M003", "Atorvastatin", "Atorvastatin Calcium", "Cardiac", "Lupin", 150.0, 50, 15, "2028-03-31", "B903");
    addMedicine("M004", "Ibuprofen", "Ibuprofen 400mg", "Analgesic", "Abbott", 45.0, 120, 25, "2027-09-30", "B904");
    addMedicine("M005", "Azithromycin", "Azithromycin 500mg", "Antibiotic", "Zydus", 180.0, 40, 15, "2027-11-30", "B905");
    addMedicine("M006", "Metformin", "Metformin HCl 500mg", "Antidiabetic", "USV", 35.0, 150, 30, "2028-05-31", "B906");
    addMedicine("M007", "Pantoprazole", "Pantoprazole 40mg", "Antacid", "Alkem", 65.0, 90, 20, "2027-10-31", "B907");
    addMedicine("M008", "Epinephrine", "Adrenaline 1mg/ml", "Emergency", "Neon", 95.0, 15, 10, "2026-12-31", "B908");

    // 4. Patients
    registerPatient("Rohan Kulkarni", "Male", "+91-9123456780", "Kothrud, Pune", 34,
                    "B+", "Acute Chest Pain - Suspected Angina", true);
    registerPatient("Anjali Mehta", "Female", "+91-9123456781", "Viman Nagar, Pune", 28,
                    "O+", "Seasonal Viral Fever", false);
    registerPatient("Suresh Gupta", "Male", "+91-9123456782", "Hadapsar, Pune", 62,
                    "A+", "Type 2 Diabetes with Hypertension", false);
    registerPatient("Kavita Joshi", "Female", "+91-9123456783", "Aundh, Pune", 45,
                    "AB+", "Compound Fracture Right Tibia", true);

    // Assign beds to some initial patients
    assignBedToPatient(19, 101); // Rohan in ICU (Emergency)
    assignBedToPatient(1, 102);  // Anjali in Ward 1 General
    assignBedToPatient(20, 104); // Kavita in ICU

    // Assign doctors
    Doctor* d1 = findDoctorById(201);
    if (d1) d1->assignPatient(101);
    Doctor* d3 = findDoctorById(203);
    if (d3) d3->assignPatient(102);

    // Add Medical Records (DS CO2 Linked List)
    addMedicalRecord(101, 201, "Acute Angina Pectoris", "Nitroglycerin drip, ECG, Oxygen", "Admitted to ICU Bed #19");
    addMedicalRecord(101, 201, "Stable Post Nitroglycerin", "Started Atorvastatin 40mg", "Vitals stable, continue ICU monitoring");
    addMedicalRecord(102, 203, "Acute Viral Pharyngitis", "Paracetamol 650mg, Oral hydration", "Rest advised for 3 days");

    // Sample Bill for Patient 101 (Standard COA Billing numbers)
    // Consultation = 500, Medicine = 750, Room = 2000, Test = 400 => Total = 3650
    generateBill(101, 500.0, 750.0, 2000.0, 400.0, 0.0, 0.0);
}

// ----------------------------------------------------------------------------
// Patient Operations
// ----------------------------------------------------------------------------
bool Hospital::registerPatient(const std::string& name, const std::string& gender,
                               const std::string& contact, const std::string& address, int age,
                               const std::string& bloodGroup, const std::string& condition,
                               bool isEmergency) {
    try {
        std::string pIdStr = "P" + std::to_string(nextPatientNumericId);
        std::string dateStr = Utils::getCurrentTimestamp().substr(0, 10);

        Patient patient(pIdStr, name, gender, contact, address, age,
                        bloodGroup, condition, dateStr, isEmergency);

        patients[pIdStr] = patient;
        patientIdList.push_back(pIdStr);

        // Add to fast O(1) Hash Table (DS CO5)
        patientHashTable.addPatient(pIdStr, name, age, condition, -1, isEmergency);

        // If emergency, enqueue into Min-Heap Priority Queue (DS CO3)
        if (isEmergency) {
            emergencyQueue.enqueue(nextPatientNumericId, 1); // 1 = highest emergency
        }

        pushOperation(100 + nextPatientNumericId); // Audit record in Stack (DS CO3)
        nextPatientNumericId++;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error registering patient: " << e.what() << "\n";
        return false;
    }
}

Patient* Hospital::findPatient(const std::string& patientId) {
    auto it = patients.find(patientId);
    if (it != patients.end()) {
        return &(it->second);
    }
    return nullptr;
}

bool Hospital::updatePatientCondition(const std::string& patientId, const std::string& newCondition) {
    Patient* p = findPatient(patientId);
    if (!p) return false;
    try {
        p->setMedicalCondition(newCondition);
        return true;
    } catch (...) {
        return false;
    }
}

void Hospital::listAllPatients() const {
    Utils::printSubHeader("REGISTERED PATIENTS LIST");
    if (patients.empty()) {
        std::cout << "  No patients currently registered.\n";
        return;
    }
    for (const auto& pair : patients) {
        pair.second.displayDetails();
    }
}

// ----------------------------------------------------------------------------
// Doctor Operations
// ----------------------------------------------------------------------------
bool Hospital::addDoctor(const std::string& name, const std::string& gender,
                         const std::string& contact, const std::string& address, int age,
                         const std::string& specialization, const std::string& department,
                         const std::string& qualification, int experienceYears,
                         double fee, int maxPatients) {
    try {
        std::string dIdStr = "D" + std::to_string(nextDoctorNumericId);
        Doctor doc(dIdStr, name, gender, contact, address, age,
                   specialization, department, qualification,
                   experienceYears, fee, maxPatients);
        doctors[dIdStr] = doc;
        nextDoctorNumericId++;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error adding doctor: " << e.what() << "\n";
        return false;
    }
}

Doctor* Hospital::findDoctor(const std::string& doctorId) {
    auto it = doctors.find(doctorId);
    if (it != doctors.end()) return &(it->second);
    return nullptr;
}

Doctor* Hospital::findDoctorById(int numericId) {
    return findDoctor("D" + std::to_string(numericId));
}

void Hospital::listAllDoctors() const {
    Utils::printSubHeader("DOCTOR ROSTER & SPECIALISTS");
    if (doctors.empty()) {
        std::cout << "  No doctors registered.\n";
        return;
    }
    for (const auto& pair : doctors) {
        pair.second.displayDetails();
    }
}

void Hospital::listDoctorsByDepartment(const std::string& dept) const {
    Utils::printSubHeader("DOCTORS IN DEPARTMENT: " + dept);
    bool found = false;
    for (const auto& pair : doctors) {
        if (pair.second.getDepartment().find(dept) != std::string::npos) {
            pair.second.displayDetails();
            found = true;
        }
    }
    if (!found) {
        std::cout << "  No doctors found in department: " << dept << "\n";
    }
}

// ----------------------------------------------------------------------------
// Appointment Operations (DS CO3 FIFO Queue)
// ----------------------------------------------------------------------------
bool Hospital::scheduleAppointment(int patientNumericId, int doctorNumericId,
                                    const std::string& dateTime, const std::string& reason,
                                    int priority) {
    std::string pIdStr = "P" + std::to_string(patientNumericId);
    std::string dIdStr = "D" + std::to_string(doctorNumericId);

    Patient* p = findPatient(pIdStr);
    Doctor* d = findDoctor(dIdStr);

    if (!p) {
        std::cerr << "Error: Patient " << pIdStr << " not found.\n";
        return false;
    }
    if (!d) {
        std::cerr << "Error: Doctor " << dIdStr << " not found.\n";
        return false;
    }
    if (!d->canAcceptMorePatients()) {
        std::cerr << "Warning: Doctor " << dIdStr << " has reached maximum patient capacity!\n";
        return false;
    }

    try {
        int apptId = nextAppointmentId++;
        Appointment appt(apptId, patientNumericId, doctorNumericId, dateTime, reason, priority);
        appointments[apptId] = appt;

        // Enqueue into FIFO Queue (DS CO3)
        appointmentQueue.enqueue(apptId);
        d->assignPatient(patientNumericId);

        if (priority == 1) {
            // Also flag as emergency in priority queue
            emergencyQueue.enqueue(patientNumericId, 1);
        }

        pushOperation(200 + apptId);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error scheduling appointment: " << e.what() << "\n";
        return false;
    }
}

Appointment* Hospital::findAppointment(int apptId) {
    auto it = appointments.find(apptId);
    if (it != appointments.end()) return &(it->second);
    return nullptr;
}

bool Hospital::processNextAppointment(int& processedApptId) {
    if (appointmentQueue.isEmpty()) return false;
    processedApptId = appointmentQueue.dequeue();
    Appointment* appt = findAppointment(processedApptId);
    if (appt) {
        appt->completeAppointment();
    }
    pushOperation(250 + processedApptId);
    return true;
}

void Hospital::listAllAppointments() const {
    Utils::printSubHeader("SCHEDULED APPOINTMENTS");
    if (appointments.empty()) {
        std::cout << "  No appointments scheduled.\n";
        return;
    }
    for (const auto& pair : appointments) {
        pair.second.displayDetails();
    }
}

void Hospital::displayAppointmentQueue() const {
    Utils::printSubHeader("APPOINTMENT FIFO QUEUE (DS CO3)");
    appointmentQueue.display();
}

// ----------------------------------------------------------------------------
// Bed & Ward Operations (DS CO1 FixedArray)
// ----------------------------------------------------------------------------
bool Hospital::addBed(int wardNum, const std::string& type, double dailyRate) {
    int bedId = static_cast<int>(beds.size()) + 1;
    beds[bedId] = Bed(bedId, wardNum, type, dailyRate);

    // If Ward 1, add to DS CO1 FixedArray
    if (wardNum == 1) {
        ward1BedSlots.insert(bedId);
    }
    return true;
}

Bed* Hospital::findBed(int bedId) {
    auto it = beds.find(bedId);
    if (it != beds.end()) return &(it->second);
    return nullptr;
}

bool Hospital::assignBedToPatient(int bedId, int patientNumericId) {
    Bed* b = findBed(bedId);
    std::string pIdStr = "P" + std::to_string(patientNumericId);
    Patient* p = findPatient(pIdStr);

    if (!b) {
        std::cerr << "Bed #" << bedId << " does not exist.\n";
        return false;
    }
    if (!p) {
        std::cerr << "Patient " << pIdStr << " does not exist.\n";
        return false;
    }
    if (b->getIsOccupied()) {
        std::cerr << "Bed #" << bedId << " is already occupied by Patient P" 
                  << b->getOccupiedByPatientId() << ".\n";
        return false;
    }

    b->assignPatient(patientNumericId);
    p->setAssignedBedId(bedId);
    pushOperation(300 + bedId);
    return true;
}

bool Hospital::dischargePatientFromBed(int bedId) {
    Bed* b = findBed(bedId);
    if (!b || !b->getIsOccupied()) return false;

    int pId = b->getOccupiedByPatientId();
    std::string pIdStr = "P" + std::to_string(pId);
    Patient* p = findPatient(pIdStr);
    if (p) {
        p->setAssignedBedId(-1);
    }

    b->release();
    pushOperation(350 + bedId);
    return true;
}

void Hospital::listAllBeds() const {
    Utils::printSubHeader("HOSPITAL BED INVENTORY");
    for (const auto& pair : beds) {
        pair.second.displayDetails();
    }
}

void Hospital::listAvailableBeds() const {
    Utils::printSubHeader("AVAILABLE VACANT BEDS");
    bool found = false;
    for (const auto& pair : beds) {
        if (!pair.second.getIsOccupied()) {
            pair.second.displayDetails();
            found = true;
        }
    }
    if (!found) {
        std::cout << "  No beds currently available!\n";
    }
}

void Hospital::displayWard1BedArray() const {
    Utils::printSubHeader("WARD 1 FIXED ARRAY BED ALLOCATION (DS CO1)");
    ward1BedSlots.display();
}

int Hospital::getOccupiedBedCount() const {
    int cnt = 0;
    for (const auto& pair : beds) {
        if (pair.second.getIsOccupied()) ++cnt;
    }
    return cnt;
}

int Hospital::getAvailableBedCount() const {
    return static_cast<int>(beds.size()) - getOccupiedBedCount();
}

// ----------------------------------------------------------------------------
// Emergency Operations (DS CO3 Priority Queue)
// ----------------------------------------------------------------------------
void Hospital::addEmergencyPatient(int patientNumericId, int priorityLevel) {
    emergencyQueue.enqueue(patientNumericId, priorityLevel);
    std::string pIdStr = "P" + std::to_string(patientNumericId);
    Patient* p = findPatient(pIdStr);
    if (p) {
        p->setIsEmergency(true);
    }
    pushOperation(400 + patientNumericId);
}

bool Hospital::processNextEmergency(int& patientNumericId, int& priorityLevel) {
    if (emergencyQueue.isEmpty()) return false;
    return emergencyQueue.dequeue(patientNumericId, priorityLevel);
}

void Hospital::displayEmergencyQueue() const {
    Utils::printSubHeader("EMERGENCY MIN-HEAP PRIORITY QUEUE (DS CO3)");
    emergencyQueue.display();
}

// ----------------------------------------------------------------------------
// Pharmacy Operations
// ----------------------------------------------------------------------------
bool Hospital::addMedicine(const std::string& id, const std::string& name, const std::string& generic,
                           const std::string& category, const std::string& mfg, double price,
                           int qty, int reorder, const std::string& exp, const std::string& batch) {
    try {
        Medicine med(id, name, generic, category, mfg, price, qty, reorder, exp, batch);
        pharmacy[id] = med;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error adding medicine: " << e.what() << "\n";
        return false;
    }
}

Medicine* Hospital::findMedicine(const std::string& id) {
    auto it = pharmacy.find(id);
    if (it != pharmacy.end()) return &(it->second);
    return nullptr;
}

bool Hospital::dispenseMedicine(const std::string& id, int qty) {
    Medicine* med = findMedicine(id);
    if (!med) {
        std::cerr << "Medicine ID " << id << " not found in pharmacy.\n";
        return false;
    }
    if (!med->dispense(qty)) {
        std::cerr << "Insufficient stock for " << med->getName() << "! Available: " << med->getQuantity() << "\n";
        return false;
    }
    pushOperation(500 + qty);
    return true;
}

bool Hospital::restockMedicine(const std::string& id, int qty) {
    Medicine* med = findMedicine(id);
    if (!med) return false;
    med->restock(qty);
    return true;
}

void Hospital::listAllMedicines() const {
    Utils::printSubHeader("PHARMACY INVENTORY");
    for (const auto& pair : pharmacy) {
        pair.second.displayDetails();
    }
}

void Hospital::listLowStockMedicines() const {
    Utils::printSubHeader("PHARMACY LOW STOCK ALERTS");
    bool found = false;
    for (const auto& pair : pharmacy) {
        if (pair.second.isLowStock()) {
            pair.second.displayDetails();
            found = true;
        }
    }
    if (!found) {
        std::cout << "  All medicines are adequately stocked.\n";
    }
}

// ----------------------------------------------------------------------------
// Medical Records (DS CO2 Linked List)
// ----------------------------------------------------------------------------
bool Hospital::addMedicalRecord(int patientNumericId, int doctorNumericId,
                                const std::string& diagnosis, const std::string& treatment,
                                const std::string& notes) {
    std::string timestamp = Utils::getCurrentTimestamp();
    int recId = nextRecordId++;
    MedicalRecord rec(recId, patientNumericId, doctorNumericId, diagnosis, treatment, notes, timestamp);

    // Append to patient's record chain (DS CO2)
    medicalHistoryChain.pushBack(rec);

    std::string pIdStr = "P" + std::to_string(patientNumericId);
    Patient* p = findPatient(pIdStr);
    if (p) {
        p->addTreatment(diagnosis + " -> " + treatment);
    }

    pushOperation(600 + recId);
    return true;
}

void Hospital::displayMedicalRecordsForPatient(int patientNumericId) const {
    Utils::printSubHeader("MEDICAL RECORD CHAIN FOR PATIENT P" + std::to_string(patientNumericId) + " (DS CO2)");
    bool found = false;
    medicalHistoryChain.forEach([patientNumericId, &found](const MedicalRecord& rec) {
        if (rec.patientId == patientNumericId) {
            rec.displayDetails();
            std::cout << "  -------------------------------------------------\n";
            found = true;
        }
    });
    if (!found) {
        std::cout << "  No medical history records found for patient P" << patientNumericId << ".\n";
    }
}

void Hospital::displayCompleteMedicalRecordChain() const {
    Utils::printSubHeader("GLOBAL MEDICAL RECORD DOUBLY LINKED LIST CHAIN (DS CO2)");
    medicalHistoryChain.display();
}

// ----------------------------------------------------------------------------
// Billing Operations
// ----------------------------------------------------------------------------
int Hospital::generateBill(int patientNumericId, double consultation, double medicine,
                           double room, double test, double tax, double discount) {
    int bId = nextBillId++;
    std::string dateStr = Utils::getCurrentTimestamp().substr(0, 10);
    Billing bill(bId, patientNumericId, consultation, medicine, room, test, dateStr, tax, discount);
    bills[bId] = bill;
    pushOperation(700 + bId);
    return bId;
}

Billing* Hospital::findBill(int billId) {
    auto it = bills.find(billId);
    if (it != bills.end()) return &(it->second);
    return nullptr;
}

bool Hospital::processBillPayment(int billId, const std::string& method) {
    Billing* b = findBill(billId);
    if (!b) return false;
    bool res = b->processPayment(method);
    if (res) pushOperation(750 + billId);
    return res;
}

void Hospital::listAllBills() const {
    Utils::printSubHeader("HOSPITAL INVOICES");
    if (bills.empty()) {
        std::cout << "  No bills generated.\n";
        return;
    }
    for (const auto& pair : bills) {
        pair.second.displayBill();
    }
}

// ----------------------------------------------------------------------------
// Data Structures Demonstration (CO1 - CO5)
// ----------------------------------------------------------------------------
void Hospital::runDataStructuresDemonstration() {
    Utils::printHeader("COMPREHENSIVE DATA STRUCTURES DEMONSTRATION (DS/PL CO1 - CO5)");

    std::cout << "\n[1] DS CO1: FixedArray Template (Fixed Ward Bed Slots)\n";
    std::cout << "  FixedArray capacity 20, stores bed IDs for Ward 1.\n";
    ward1BedSlots.display();
    std::cout << "  Bed ID 5 search: " << (ward1BedSlots.find(5) != -1 ? "FOUND" : "NOT FOUND") << "\n";

    std::cout << "\n[2] DS CO2: Doubly Linked List (Medical History Chain)\n";
    std::cout << "  Nodes represent chronological medical encounters chained with prev/next pointers.\n";
    medicalHistoryChain.display();

    std::cout << "\n[3] DS CO3: Stack (LIFO History / Audit Trail) & Queues\n";
    std::cout << "  Stack size: " << operationStack.size() << " operations recorded.\n";
    operationStack.display();

    std::cout << "\n  FIFO Queue (Appointment Scheduling):\n";
    appointmentQueue.display();

    std::cout << "\n  Min-Heap Priority Queue (Emergency Triage):\n";
    emergencyQueue.display();

    std::cout << "\n[4] DS CO4: Searching & Sorting Algorithms\n";
    std::vector<int> numbers = {45, 12, 85, 32, 89, 39, 69, 44, 42, 1, 99};
    std::cout << "  Original Unsorted Data: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << "\n";

    int linearIdx = Searching::linearSearch(numbers, 69);
    std::cout << "  Linear Search for 69: Found at index " << linearIdx << " (O(n))\n";

    std::vector<int> sortedQuick = numbers;
    Sorting::quickSort(sortedQuick);
    std::cout << "  QuickSorted Data (O(n log n)): ";
    for (int n : sortedQuick) std::cout << n << " ";
    std::cout << "\n";

    int binaryIdx = Searching::binarySearch(sortedQuick, 69);
    std::cout << "  Binary Search for 69 on sorted array: Found at index " << binaryIdx << " (O(log n))\n";

    std::vector<int> sortedMerge = numbers;
    Sorting::mergeSort(sortedMerge);
    std::cout << "  MergeSorted Data (O(n log n)): ";
    for (int n : sortedMerge) std::cout << n << " ";
    std::cout << "\n";

    std::cout << "\n[5] DS CO5: Open-Addressing Hash Table with Linear Probing (O(1) Patient Lookup)\n";
    patientHashTable.displayStats();
    PatientQuickInfo qi;
    if (patientHashTable.findPatient("P101", qi)) {
        std::cout << "  Instant O(1) Hash Lookup for 'P101': Name = " << qi.name 
                  << ", Condition = " << qi.condition << ", Emergency = " << (qi.isEmergency ? "YES" : "NO") << "\n";
    }

    Utils::printSubHeader("DEMONSTRATION COMPLETED SUCCESSFULLY");
}

// ----------------------------------------------------------------------------
// File Persistence (Load and Save Data)
// ----------------------------------------------------------------------------
void Hospital::loadDataFromFiles() {
    // Read medicines.txt
    std::ifstream medFile("data/medicines.txt");
    if (medFile.is_open()) {
        std::string line;
        while (std::getline(medFile, line)) {
            if (line.empty() || line[0] == '#') continue;
            auto parts = Utils::split(line, ',');
            if (parts.size() >= 10) {
                addMedicine(parts[0], parts[1], parts[2], parts[3], parts[4],
                            std::stod(parts[5]), std::stoi(parts[6]), std::stoi(parts[7]),
                            parts[8], parts[9]);
            }
        }
        medFile.close();
    }

    // Read doctors.txt
    std::ifstream docFile("data/doctors.txt");
    if (docFile.is_open()) {
        std::string line;
        while (std::getline(docFile, line)) {
            if (line.empty() || line[0] == '#') continue;
            auto parts = Utils::split(line, ',');
            if (parts.size() >= 11) {
                addDoctor(parts[1], parts[2], parts[3], parts[4], std::stoi(parts[5]),
                          parts[6], parts[7], parts[8], std::stoi(parts[9]),
                          std::stod(parts[10]));
            }
        }
        docFile.close();
    }
}

void Hospital::saveDataToFiles() {
    // Save medicines
    std::ofstream medFile("data/medicines.txt");
    if (medFile.is_open()) {
        medFile << "# ID,Name,Generic,Category,Manufacturer,Price,Quantity,Reorder,Expiry,Batch\n";
        for (const auto& pair : pharmacy) {
            const Medicine& m = pair.second;
            medFile << m.getId() << "," << m.getName() << "," << m.getGenericName() << ","
                    << m.getCategory() << "," << m.getManufacturer() << ","
                    << m.getPrice() << "," << m.getQuantity() << "," << m.getReorderLevel() << ","
                    << m.getExpiryDate() << "," << m.getBatchNumber() << "\n";
        }
        medFile.close();
    }

    // Save bills
    std::ofstream billFile("data/bills.txt");
    if (billFile.is_open()) {
        billFile << "# BillID,PatientID,Consultation,Medicine,Room,Test,Total,Status,Method,Date\n";
        for (const auto& pair : bills) {
            const Billing& b = pair.second;
            billFile << b.getBillId() << "," << b.getPatientId() << ","
                     << b.getConsultationCharge() << "," << b.getMedicineCharge() << ","
                     << b.getRoomCharge() << "," << b.getTestCharge() << ","
                     << b.getTotalAmount() << "," << b.getPaymentStatusString() << ","
                     << b.getPaymentMethod() << "," << b.getBillingDate() << "\n";
        }
        billFile.close();
    }
}
