#ifndef PATIENT_H
#define PATIENT_H

#include "models/Person.h"
#include <vector>

// ============================================================================
// PSOOP CO1: Patient Class (Inherits from Person)
// Demonstrates Inheritance, Polymorphism, and Robust Input Validation
// ============================================================================
class Patient : public Person {
private:
    std::string bloodGroup;
    std::string medicalCondition;
    std::string admissionDate;
    int assignedBedId;
    int assignedDoctorId;
    bool isEmergency;
    std::vector<std::string> treatmentHistory;
    std::vector<std::string> prescribedMedicines;

    static bool isValidBloodGroup(const std::string& bg);

public:
    Patient();
    Patient(const std::string& id, const std::string& name, const std::string& gender,
            const std::string& contact, const std::string& address, int age,
            const std::string& bloodGroup, const std::string& medicalCondition,
            const std::string& admissionDate, bool isEmergency = false);

    // Polymorphism
    void displayDetails() const override;
    std::string getRole() const override { return "Patient"; }

    // Getters
    std::string getBloodGroup() const { return bloodGroup; }
    std::string getMedicalCondition() const { return medicalCondition; }
    std::string getAdmissionDate() const { return admissionDate; }
    int getAssignedBedId() const { return assignedBedId; }
    int getAssignedDoctorId() const { return assignedDoctorId; }
    bool getIsEmergency() const { return isEmergency; }
    const std::vector<std::string>& getTreatmentHistory() const { return treatmentHistory; }
    const std::vector<std::string>& getPrescribedMedicines() const { return prescribedMedicines; }

    // Setters & Operations
    void setBloodGroup(const std::string& bg);
    void setMedicalCondition(const std::string& cond);
    void setAdmissionDate(const std::string& date);
    void setAssignedBedId(int bedId) { assignedBedId = bedId; }
    void setAssignedDoctorId(int docId) { assignedDoctorId = docId; }
    void setIsEmergency(bool emergency) { isEmergency = emergency; }

    void addTreatment(const std::string& treatment);
    void addPrescribedMedicine(const std::string& medicine);
};

#endif // PATIENT_H
