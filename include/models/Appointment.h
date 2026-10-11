#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>
#include <iostream>

// ============================================================================
// PSOOP CO1: Appointment Class
// Demonstrates Composition of Patient, Doctor, and Scheduling Priority
// ============================================================================
class Appointment {
public:
    enum Status { SCHEDULED, COMPLETED, CANCELLED };

private:
    int appointmentId;
    int patientId;
    int doctorId;
    std::string dateTime;
    std::string reason;
    int priority; // 1 = Emergency, 2 = Urgent, 3 = Normal
    Status status;

public:
    Appointment();
    Appointment(int apptId, int pId, int dId, const std::string& dt,
                const std::string& reason, int priority = 3);

    int getAppointmentId() const { return appointmentId; }
    int getPatientId() const { return patientId; }
    int getDoctorId() const { return doctorId; }
    std::string getDateTime() const { return dateTime; }
    std::string getReason() const { return reason; }
    int getPriority() const { return priority; }
    Status getStatus() const { return status; }
    std::string getStatusString() const;
    std::string getPriorityString() const;

    void setDateTime(const std::string& dt) { dateTime = dt; }
    void setReason(const std::string& r) { reason = r; }
    void setPriority(int p);
    void setStatus(Status s) { status = s; }

    void completeAppointment() { status = COMPLETED; }
    void cancelAppointment() { status = CANCELLED; }

    void displayDetails() const;
};

#endif // APPOINTMENT_H
