#include "models/MedicalRecord.h"
#include <iomanip>

MedicalRecord::MedicalRecord()
    : recordId(0), patientId(0), doctorId(0), next(nullptr) {}

MedicalRecord::MedicalRecord(int rId, int pId, int dId, const std::string& diag,
                             const std::string& treat, const std::string& notes,
                             const std::string& time)
    : recordId(rId), patientId(pId), doctorId(dId), diagnosis(diag),
      treatment(treat), doctorNotes(notes), timestamp(time), next(nullptr) {}

void MedicalRecord::displayDetails() const {
    std::cout << "Record #" << recordId 
              << " | Patient ID: P" << patientId 
              << " | Doctor ID: D" << doctorId
              << " | Date: " << timestamp << "\n";
    std::cout << "  Diagnosis: " << diagnosis << "\n";
    std::cout << "  Treatment: " << treatment << "\n";
    std::cout << "  Notes: " << doctorNotes << "\n";
}
