#include "models/Patient.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>

bool Patient::isValidBloodGroup(const std::string& bg) {
    static const std::vector<std::string> validGroups = {
        "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
    };
    return std::find(validGroups.begin(), validGroups.end(), bg) != validGroups.end();
}

Patient::Patient()
    : Person(), assignedBedId(-1), assignedDoctorId(-1), isEmergency(false) {}

Patient::Patient(const std::string& id, const std::string& name, const std::string& gender,
                 const std::string& contact, const std::string& address, int age,
                 const std::string& bloodGroup, const std::string& medicalCondition,
                 const std::string& admissionDate, bool isEmergency)
    : Person(id, name, gender, contact, address, age),
      assignedBedId(-1), assignedDoctorId(-1), isEmergency(isEmergency) {
    setBloodGroup(bloodGroup);
    setMedicalCondition(medicalCondition);
    setAdmissionDate(admissionDate);
}

void Patient::setBloodGroup(const std::string& bg) {
    if (!isValidBloodGroup(bg)) {
        throw std::invalid_argument("Invalid blood group: " + bg + " (Must be A+, A-, B+, B-, AB+, AB-, O+, O-)");
    }
    bloodGroup = bg;
}

void Patient::setMedicalCondition(const std::string& cond) {
    if (cond.empty()) {
        throw std::invalid_argument("Medical condition cannot be empty");
    }
    medicalCondition = cond;
}

void Patient::setAdmissionDate(const std::string& date) {
    if (date.empty()) {
        throw std::invalid_argument("Admission date cannot be empty");
    }
    admissionDate = date;
}

void Patient::addTreatment(const std::string& treatment) {
    if (!treatment.empty()) {
        treatmentHistory.push_back(treatment);
    }
}

void Patient::addPrescribedMedicine(const std::string& medicine) {
    if (!medicine.empty()) {
        prescribedMedicines.push_back(medicine);
    }
}

void Patient::displayDetails() const {
    std::cout << "--------------------------------------------------\n";
    std::cout << " [Patient Profile] ID: " << id << " | Name: " << name << "\n";
    std::cout << " Age: " << age << " | Gender: " << gender << " | Blood Group: " << bloodGroup << "\n";
    std::cout << " Contact: " << contactNumber << " | Address: " << address << "\n";
    std::cout << " Condition: " << medicalCondition << " | Admitted: " << admissionDate << "\n";
    std::cout << " Priority Status: " << (isEmergency ? "CRITICAL EMERGENCY" : "Normal") << "\n";
    std::cout << " Assigned Bed ID: " << (assignedBedId != -1 ? std::to_string(assignedBedId) : "None")
              << " | Doctor ID: " << (assignedDoctorId != -1 ? std::to_string(assignedDoctorId) : "None") << "\n";
    if (!prescribedMedicines.empty()) {
        std::cout << " Prescribed Meds: ";
        for (size_t i = 0; i < prescribedMedicines.size(); ++i) {
            std::cout << prescribedMedicines[i] << (i + 1 < prescribedMedicines.size() ? ", " : "");
        }
        std::cout << "\n";
    }
    std::cout << "--------------------------------------------------\n";
}
