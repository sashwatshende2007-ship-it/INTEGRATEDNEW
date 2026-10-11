#ifndef BILLING_H
#define BILLING_H

#include <string>
#include <iostream>

// Structure to pass charges to COA module
struct BillingCharges {
    uint32_t consultation;
    uint32_t medicine;
    uint32_t room;
    uint32_t test;
    uint32_t total;
};

// ============================================================================
// PSOOP CO1: Billing Class
// Demonstrates Itemized Invoicing, Financial Calculations, and COA Bridge
// ============================================================================
class Billing {
public:
    enum PaymentStatus { PENDING, PAID, REFUNDED };

private:
    int billId;
    int patientId;
    double consultationCharge;
    double medicineCharge;
    double roomCharge;
    double testCharge;
    double subtotal;
    double taxPercent; // 18% GST
    double discountPercent;
    double totalAmount;
    PaymentStatus status;
    std::string paymentMethod; // Cash, Card, UPI
    std::string billingDate;

    void calculateTotals();

public:
    Billing();
    Billing(int bId, int pId, double consultation, double medicine,
            double room, double test, const std::string& date,
            double tax = 18.0, double discount = 0.0);

    int getBillId() const { return billId; }
    int getPatientId() const { return patientId; }
    double getConsultationCharge() const { return consultationCharge; }
    double getMedicineCharge() const { return medicineCharge; }
    double getRoomCharge() const { return roomCharge; }
    double getTestCharge() const { return testCharge; }
    double getSubtotal() const { return subtotal; }
    double getTaxPercent() const { return taxPercent; }
    double getDiscountPercent() const { return discountPercent; }
    double getTotalAmount() const { return totalAmount; }
    PaymentStatus getStatus() const { return status; }
    std::string getPaymentStatusString() const;
    std::string getPaymentMethod() const { return paymentMethod; }
    std::string getBillingDate() const { return billingDate; }

    void setConsultationCharge(double c);
    void setMedicineCharge(double m);
    void setRoomCharge(double r);
    void setTestCharge(double t);
    void setDiscountPercent(double d);

    bool processPayment(const std::string& method);
    BillingCharges getChargesForCOA() const;

    void displayBill() const;
};

#endif // BILLING_H
