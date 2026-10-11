#include "models/Appointment.h"
#include <stdexcept>
#include <iomanip>

Appointment::Appointment()
    : appointmentId(0), patientId(0), doctorId(0), priority(3), status(SCHEDULED) {}

Appointment::Appointment(int apptId, int pId, int dId, const std::string& dt,
                         const std::string& reason, int priority)
    : appointmentId(apptId), patientId(pId), doctorId(dId), dateTime(dt),
      reason(reason), priority(priority), status(SCHEDULED) {
    if (priority < 1 || priority > 3) {
        throw std::invalid_argument("Priority must be 1 (Emergency), 2 (Urgent), or 3 (Normal)");
    }
}

void Appointment::setPriority(int p) {
    if (p < 1 || p > 3) {
        throw std::invalid_argument("Priority must be 1 (Emergency), 2 (Urgent), or 3 (Normal)");
    }
    priority = p;
}

std::string Appointment::getStatusString() const {
    switch (status) {
        case SCHEDULED: return "SCHEDULED";
        case COMPLETED: return "COMPLETED";
        case CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

std::string Appointment::getPriorityString() const {
    switch (priority) {
        case 1: return "1 - EMERGENCY";
        case 2: return "2 - URGENT";
        case 3: return "3 - NORMAL";
        default: return "NORMAL";
    }
}

void Appointment::displayDetails() const {
    std::cout << "Appt #" << appointmentId 
              << " | Patient ID: P" << patientId 
              << " | Doctor ID: D" << doctorId
              << " | Date/Time: " << dateTime 
              << " | Priority: " << getPriorityString()
              << " | Status: " << getStatusString() 
              << " | Reason: " << reason << "\n";
}
