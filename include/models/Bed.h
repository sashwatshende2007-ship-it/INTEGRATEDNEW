#ifndef BED_H
#define BED_H

#include <string>
#include <iostream>

// ============================================================================
// PSOOP CO1: Bed Class
// Demonstrates State Management and Encapsulation
// ============================================================================
class Bed {
private:
    int bedId;
    int wardNumber;
    std::string type; // General, Semi-Private, Private, ICU, Emergency
    double dailyRate;
    bool isOccupied;
    int occupiedByPatientId;

public:
    Bed();
    Bed(int bedId, int wardNumber, const std::string& type, double dailyRate);

    int getBedId() const { return bedId; }
    int getWardNumber() const { return wardNumber; }
    std::string getType() const { return type; }
    double getDailyRate() const { return dailyRate; }
    bool getIsOccupied() const { return isOccupied; }
    int getOccupiedByPatientId() const { return occupiedByPatientId; }

    void setWardNumber(int ward) { wardNumber = ward; }
    void setType(const std::string& t) { type = t; }
    void setDailyRate(double rate);

    bool assignPatient(int patientNumericId);
    bool release();

    void displayDetails() const;
};

#endif // BED_H
