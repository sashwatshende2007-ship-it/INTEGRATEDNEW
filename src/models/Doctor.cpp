#include "models/Doctor.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

Doctor::Doctor()
    : Person(), experienceYears(0), consultationFee(0.0), maxPatients(10), isAvailable(true) {}

Doctor::Doctor(const std::string& id, const std::string& name, const std::string& gender,
               const std::string& contact, const std::string& address, int age,
               const std::string& specialization, const std::string& department,
               const std::string& qualification, int experienceYears,
               double consultationFee, int maxPatients)
    : Person(id, name, gender, contact, address, age),
      specialization(specialization), department(department), qualification(qualification),
      experienceYears(experienceYears), consultationFee(consultationFee),
      maxPatients(maxPatients), isAvailable(true) {
    if (consultationFee < 0.0) {
        throw std::invalid_argument("Consultation fee cannot be negative");
    }
    if (experienceYears < 0) {
        throw std::invalid_argument("Experience years cannot be negative");
    }
}

void Doctor::setSpecialization(const std::string& spec) {
    specialization = spec;
}

void Doctor::setDepartment(const std::string& dept) {
    department = dept;
}

void Doctor::setQualification(const std::string& qual) {
    qualification = qual;
}

void Doctor::setExperienceYears(int years) {
    if (years < 0) throw std::invalid_argument("Experience years cannot be negative");
    experienceYears = years;
}

void Doctor::setConsultationFee(double fee) {
    if (fee < 0.0) throw std::invalid_argument("Consultation fee cannot be negative");
    consultationFee = fee;
}

void Doctor::setMaxPatients(int maxP) {
    if (maxP <= 0) throw std::invalid_argument("Max patients must be positive");
    maxPatients = maxP;
}

bool Doctor::canAcceptMorePatients() const {
    return isAvailable && (getCurrentPatientCount() < maxPatients);
}

bool Doctor::assignPatient(int patientNumericId) {
    if (!canAcceptMorePatients()) return false;
    auto it = std::find(assignedPatientIds.begin(), assignedPatientIds.end(), patientNumericId);
    if (it == assignedPatientIds.end()) {
        assignedPatientIds.push_back(patientNumericId);
        return true;
    }
    return false; // Already assigned
}

bool Doctor::dischargePatient(int patientNumericId) {
    auto it = std::find(assignedPatientIds.begin(), assignedPatientIds.end(), patientNumericId);
    if (it != assignedPatientIds.end()) {
        assignedPatientIds.erase(it);
        return true;
    }
    return false;
}

void Doctor::displayDetails() const {
    std::cout << "--------------------------------------------------\n";
    std::cout << " [Doctor Profile] ID: " << id << " | Dr. " << name << "\n";
    std::cout << " Department: " << department << " | Specialization: " << specialization << "\n";
    std::cout << " Qualification: " << qualification << " | Experience: " << experienceYears << " years\n";
    std::cout << " Consultation Fee: Rs. " << consultationFee << "\n";
    std::cout << " Patient Load: " << getCurrentPatientCount() << "/" << maxPatients 
              << " | Available: " << (canAcceptMorePatients() ? "YES" : "NO / FULL") << "\n";
    std::cout << " Contact: " << contactNumber << " | Age: " << age << "\n";
    std::cout << "--------------------------------------------------\n";
}
