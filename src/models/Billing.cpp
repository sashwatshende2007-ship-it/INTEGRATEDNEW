#include "models/Billing.h"
#include <iomanip>
#include <stdexcept>

void Billing::calculateTotals() {
    subtotal = consultationCharge + medicineCharge + roomCharge + testCharge;
    double taxAmount = (subtotal * taxPercent) / 100.0;
    double discountAmount = (subtotal * discountPercent) / 100.0;
    totalAmount = subtotal + taxAmount - discountAmount;
    if (totalAmount < 0.0) totalAmount = 0.0;
}

Billing::Billing()
    : billId(0), patientId(0), consultationCharge(0.0), medicineCharge(0.0),
      roomCharge(0.0), testCharge(0.0), subtotal(0.0), taxPercent(18.0),
      discountPercent(0.0), totalAmount(0.0), status(PENDING),
      paymentMethod("Unpaid"), billingDate("") {}

Billing::Billing(int bId, int pId, double consultation, double medicine,
                 double room, double test, const std::string& date,
                 double tax, double discount)
    : billId(bId), patientId(pId), consultationCharge(consultation),
      medicineCharge(medicine), roomCharge(room), testCharge(test),
      taxPercent(tax), discountPercent(discount), status(PENDING),
      paymentMethod("Pending"), billingDate(date) {
    if (consultation < 0 || medicine < 0 || room < 0 || test < 0) {
        throw std::invalid_argument("Bill charges cannot be negative");
    }
    calculateTotals();
}

void Billing::setConsultationCharge(double c) {
    if (c < 0) throw std::invalid_argument("Charge cannot be negative");
    consultationCharge = c;
    calculateTotals();
}

void Billing::setMedicineCharge(double m) {
    if (m < 0) throw std::invalid_argument("Charge cannot be negative");
    medicineCharge = m;
    calculateTotals();
}

void Billing::setRoomCharge(double r) {
    if (r < 0) throw std::invalid_argument("Charge cannot be negative");
    roomCharge = r;
    calculateTotals();
}

void Billing::setTestCharge(double t) {
    if (t < 0) throw std::invalid_argument("Charge cannot be negative");
    testCharge = t;
    calculateTotals();
}

void Billing::setDiscountPercent(double d) {
    if (d < 0.0 || d > 100.0) throw std::invalid_argument("Discount must be between 0 and 100");
    discountPercent = d;
    calculateTotals();
}

bool Billing::processPayment(const std::string& method) {
    if (status == PAID) return false;
    paymentMethod = method;
    status = PAID;
    return true;
}

BillingCharges Billing::getChargesForCOA() const {
    BillingCharges c;
    c.consultation = static_cast<uint32_t>(consultationCharge);
    c.medicine = static_cast<uint32_t>(medicineCharge);
    c.room = static_cast<uint32_t>(roomCharge);
    c.test = static_cast<uint32_t>(testCharge);
    c.total = c.consultation + c.medicine + c.room + c.test;
    return c;
}

std::string Billing::getPaymentStatusString() const {
    switch (status) {
        case PENDING: return "PENDING";
        case PAID: return "PAID";
        case REFUNDED: return "REFUNDED";
        default: return "UNKNOWN";
    }
}

void Billing::displayBill() const {
    std::cout << "\n=======================================================\n";
    std::cout << "                 HOSPITAL INVOICE                      \n";
    std::cout << "=======================================================\n";
    std::cout << " Invoice ID: B" << billId << " | Patient ID: P" << patientId 
              << " | Date: " << billingDate << "\n";
    std::cout << " Status: " << getPaymentStatusString() 
              << " | Payment Method: " << paymentMethod << "\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << "  Itemized Charges:                                    \n";
    std::cout << "  - Doctor Consultation Fee : Rs. " << std::fixed << std::setprecision(2) << consultationCharge << "\n";
    std::cout << "  - Pharmacy / Medicine Fee : Rs. " << medicineCharge << "\n";
    std::cout << "  - Room / Bed Charges      : Rs. " << roomCharge << "\n";
    std::cout << "  - Diagnostic & Lab Tests  : Rs. " << testCharge << "\n";
    std::cout << "-------------------------------------------------------\n";
    std::cout << "  Subtotal (Base Charges)   : Rs. " << subtotal << "\n";
    std::cout << "  GST Tax (" << taxPercent << "%)              : Rs. " << (subtotal * taxPercent / 100.0) << "\n";
    if (discountPercent > 0.0) {
        std::cout << "  Discount (" << discountPercent << "%)         : -Rs. " << (subtotal * discountPercent / 100.0) << "\n";
    }
    std::cout << "-------------------------------------------------------\n";
    std::cout << "  GRAND TOTAL               : Rs. " << totalAmount << "\n";
    std::cout << "=======================================================\n";
}
