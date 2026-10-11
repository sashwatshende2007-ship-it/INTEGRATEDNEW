#include "models/Bed.h"
#include <stdexcept>
#include <iomanip>

Bed::Bed()
    : bedId(0), wardNumber(0), type("General"), dailyRate(500.0),
      isOccupied(false), occupiedByPatientId(-1) {}

Bed::Bed(int bedId, int wardNumber, const std::string& type, double dailyRate)
    : bedId(bedId), wardNumber(wardNumber), type(type), dailyRate(dailyRate),
      isOccupied(false), occupiedByPatientId(-1) {
    if (dailyRate < 0.0) {
        throw std::invalid_argument("Daily rate cannot be negative");
    }
}

void Bed::setDailyRate(double rate) {
    if (rate < 0.0) {
        throw std::invalid_argument("Daily rate cannot be negative");
    }
    dailyRate = rate;
}

bool Bed::assignPatient(int patientNumericId) {
    if (isOccupied) return false;
    isOccupied = true;
    occupiedByPatientId = patientNumericId;
    return true;
}

bool Bed::release() {
    if (!isOccupied) return false;
    isOccupied = false;
    occupiedByPatientId = -1;
    return true;
}

void Bed::displayDetails() const {
    std::cout << "Bed #" << std::setw(3) << bedId 
              << " | Ward: " << wardNumber 
              << " | Type: " << std::setw(12) << type 
              << " | Rate/Day: Rs. " << std::fixed << std::setprecision(2) << dailyRate
              << " | Status: " << (isOccupied ? ("OCCUPIED (P" + std::to_string(occupiedByPatientId) + ")") : "AVAILABLE")
              << "\n";
}
