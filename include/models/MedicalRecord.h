#ifndef MEDICAL_RECORD_H
#define MEDICAL_RECORD_H

#include <string>
#include <iostream>

// ============================================================================
// PSOOP CO1 / DS CO2: MedicalRecord Class (Doubly Linked List Node Element)
// Demonstrates Medical History Chain per Patient
// ============================================================================
class MedicalRecord {
public:
    int recordId;
    int patientId;
    int doctorId;
    std::string diagnosis;
    std::string treatment;
    std::string doctorNotes;
    std::string timestamp;

    // Pointer for linked list chaining (DS CO2)
    MedicalRecord* next;

    MedicalRecord();
    MedicalRecord(int rId, int pId, int dId, const std::string& diag,
                  const std::string& treat, const std::string& notes,
                  const std::string& time);

    void displayDetails() const;

    // Stream operator for Linked List display
    friend std::ostream& operator<<(std::ostream& os, const MedicalRecord& rec) {
        os << "[Rec #" << rec.recordId << ": " << rec.diagnosis << " (" << rec.timestamp << ")]";
        return os;
    }
};

#endif // MEDICAL_RECORD_H
