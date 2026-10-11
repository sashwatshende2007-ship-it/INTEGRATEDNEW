#ifndef DOCTOR_H
#define DOCTOR_H

#include "models/Person.h"
#include <vector>

// ============================================================================
// PSOOP CO1: Doctor Class (Inherits from Person)
// Demonstrates Association with Patients and Capacity Management
// ============================================================================
class Doctor : public Person {
private:
    std::string specialization;
    std::string department;
    std::string qualification;
    int experienceYears;
    double consultationFee;
    int maxPatients;
    bool isAvailable;
    std::vector<int> assignedPatientIds;

public:
    Doctor();
    Doctor(const std::string& id, const std::string& name, const std::string& gender,
           const std::string& contact, const std::string& address, int age,
           const std::string& specialization, const std::string& department,
           const std::string& qualification, int experienceYears,
           double consultationFee, int maxPatients = 10);

    // Polymorphism
    void displayDetails() const override;
    std::string getRole() const override { return "Doctor"; }

    // Getters
    std::string getSpecialization() const { return specialization; }
    std::string getDepartment() const { return department; }
    std::string getQualification() const { return qualification; }
    int getExperienceYears() const { return experienceYears; }
    double getConsultationFee() const { return consultationFee; }
    int getMaxPatients() const { return maxPatients; }
    int getCurrentPatientCount() const { return static_cast<int>(assignedPatientIds.size()); }
    bool getIsAvailable() const { return isAvailable && (getCurrentPatientCount() < maxPatients); }
    const std::vector<int>& getAssignedPatientIds() const { return assignedPatientIds; }

    // Setters
    void setSpecialization(const std::string& spec);
    void setDepartment(const std::string& dept);
    void setQualification(const std::string& qual);
    void setExperienceYears(int years);
    void setConsultationFee(double fee);
    void setMaxPatients(int maxP);
    void setIsAvailable(bool avail) { isAvailable = avail; }

    // Operations
    bool canAcceptMorePatients() const;
    bool assignPatient(int patientNumericId);
    bool dischargePatient(int patientNumericId);
};

#endif // DOCTOR_H
