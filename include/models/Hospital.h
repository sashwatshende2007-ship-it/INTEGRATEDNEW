#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "models/Patient.h"
#include "models/Doctor.h"
#include "models/Appointment.h"
#include "models/Bed.h"
#include "models/Medicine.h"
#include "models/MedicalRecord.h"
#include "models/Billing.h"

#include "data_structures/ArrayUtils.h"
#include "data_structures/LinkedList.h"
#include "data_structures/Stack.h"
#include "data_structures/Queue.h"
#include "data_structures/HashTable.h"

// ============================================================================
// PSOOP CO2 & CO4: Hospital Facade Pattern Class
// Single unified interface coordinating all subsystems, models & data structures
// ============================================================================
class Hospital {
private:
    static Hospital* instance;

    // Entity Maps (Key-value store)
    std::map<std::string, Patient> patients;
    std::map<std::string, Doctor> doctors;
    std::map<int, Appointment> appointments;
    std::map<int, Bed> beds;
    std::map<std::string, Medicine> pharmacy;
    std::map<int, Billing> bills;

    // DS CO1: FixedArray for Ward Bed slots (20 General beds in Ward 1)
    FixedArray<int, 20> ward1BedSlots;

    // DS CO2: LinkedList of Medical Records
    LinkedList<MedicalRecord> medicalHistoryChain;

    // DS CO3: LIFO Stack for Hospital Operations History (Undo/Audit)
    Stack<int> operationStack;

    // DS CO3: FIFO Queue for Appointment Scheduling
    Queue<int> appointmentQueue;

    // DS CO3: Min-Heap PriorityQueue for Emergency Patient Triage
    PriorityQueue<int> emergencyQueue;

    // DS CO4: Vector of Patient IDs for Searching and Sorting demos
    std::vector<std::string> patientIdList;

    // DS CO5: Open-Addressing Hash Table for fast O(1) Patient Lookups
    PatientHashTable patientHashTable;

    // Auto-increment counters
    int nextPatientNumericId;
    int nextDoctorNumericId;
    int nextAppointmentId;
    int nextBillId;
    int nextRecordId;

    Hospital(); // Private constructor (Singleton)

public:
    static Hospital* getInstance();

    // Data Initialization and Persistence
    void initializeSampleData();
    void loadDataFromFiles();
    void saveDataToFiles();

    // Patient Operations (PSOOP CO1, CO3, CO4)
    bool registerPatient(const std::string& name, const std::string& gender,
                         const std::string& contact, const std::string& address, int age,
                         const std::string& bloodGroup, const std::string& condition,
                         bool isEmergency = false);
    Patient* findPatient(const std::string& patientId);
    bool updatePatientCondition(const std::string& patientId, const std::string& newCondition);
    void listAllPatients() const;

    // Doctor Operations
    bool addDoctor(const std::string& name, const std::string& gender,
                   const std::string& contact, const std::string& address, int age,
                   const std::string& specialization, const std::string& department,
                   const std::string& qualification, int experienceYears,
                   double fee, int maxPatients = 10);
    Doctor* findDoctor(const std::string& doctorId);
    Doctor* findDoctorById(int numericId);
    void listAllDoctors() const;
    void listDoctorsByDepartment(const std::string& dept) const;

    // Appointment Operations (DS CO3 Queue)
    bool scheduleAppointment(int patientNumericId, int doctorNumericId,
                             const std::string& dateTime, const std::string& reason,
                             int priority = 3);
    Appointment* findAppointment(int apptId);
    bool processNextAppointment(int& processedApptId);
    void listAllAppointments() const;
    void displayAppointmentQueue() const;

    // Bed & Ward Operations (DS CO1 FixedArray)
    bool addBed(int wardNum, const std::string& type, double dailyRate);
    Bed* findBed(int bedId);
    bool assignBedToPatient(int bedId, int patientNumericId);
    bool dischargePatientFromBed(int bedId);
    void listAllBeds() const;
    void listAvailableBeds() const;
    void displayWard1BedArray() const;

    // Emergency Operations (DS CO3 Priority Queue)
    void addEmergencyPatient(int patientNumericId, int priorityLevel);
    bool processNextEmergency(int& patientNumericId, int& priorityLevel);
    void displayEmergencyQueue() const;
    size_t getEmergencyQueueSize() const { return emergencyQueue.size(); }

    // Pharmacy Operations
    bool addMedicine(const std::string& id, const std::string& name, const std::string& generic,
                     const std::string& category, const std::string& mfg, double price,
                     int qty, int reorder, const std::string& exp, const std::string& batch);
    Medicine* findMedicine(const std::string& id);
    bool dispenseMedicine(const std::string& id, int qty);
    bool restockMedicine(const std::string& id, int qty);
    void listAllMedicines() const;
    void listLowStockMedicines() const;

    // Medical Records Operations (DS CO2 Linked List)
    bool addMedicalRecord(int patientNumericId, int doctorNumericId,
                          const std::string& diagnosis, const std::string& treatment,
                          const std::string& notes);
    void displayMedicalRecordsForPatient(int patientNumericId) const;
    void displayCompleteMedicalRecordChain() const;

    // Billing Operations
    int generateBill(int patientNumericId, double consultation, double medicine,
                     double room, double test, double tax = 18.0, double discount = 0.0);
    Billing* findBill(int billId);
    bool processBillPayment(int billId, const std::string& method);
    void listAllBills() const;

    // Stack Operations (DS CO3 LIFO Audit Trail)
    void pushOperation(int opCode);
    int popOperation();
    void displayOperationHistory() const;

    // Data Structures Demonstration (DS CO1 to CO5)
    void runDataStructuresDemonstration();

    // Statistics Getters (For Graphics Dashboard)
    size_t getTotalPatients() const { return patients.size(); }
    size_t getTotalDoctors() const { return doctors.size(); }
    size_t getTotalAppointments() const { return appointments.size(); }
    size_t getTotalBeds() const { return beds.size(); }
    int getOccupiedBedCount() const;
    int getAvailableBedCount() const;
    size_t getEmergencyCount() const { return emergencyQueue.size(); }
    size_t getPendingAppointmentsCount() const { return appointmentQueue.size(); }

    // Direct accessors for graphical dashboard panels
    const std::map<std::string, Patient>& getAllPatients() const { return patients; }
    const std::map<std::string, Doctor>& getAllDoctors() const { return doctors; }
    const std::map<int, Appointment>& getAllAppointments() const { return appointments; }
    const std::map<int, Bed>& getAllBeds() const { return beds; }
    const std::map<std::string, Medicine>& getAllMedicines() const { return pharmacy; }
    const std::map<int, Billing>& getAllBills() const { return bills; }
};

#endif // HOSPITAL_H
