#include "models/Medicine.h"
#include <stdexcept>
#include <iomanip>

Medicine::Medicine()
    : price(0.0), quantity(0), reorderLevel(10) {}

Medicine::Medicine(const std::string& id, const std::string& name, const std::string& genericName,
                   const std::string& category, const std::string& manufacturer,
                   double price, int quantity, int reorderLevel,
                   const std::string& expiryDate, const std::string& batchNumber)
    : id(id), name(name), genericName(genericName), category(category),
      manufacturer(manufacturer), price(price), quantity(quantity),
      reorderLevel(reorderLevel), expiryDate(expiryDate), batchNumber(batchNumber) {
    if (price < 0.0) throw std::invalid_argument("Price cannot be negative");
    if (quantity < 0) throw std::invalid_argument("Quantity cannot be negative");
    if (reorderLevel < 0) throw std::invalid_argument("Reorder level cannot be negative");
}

void Medicine::setPrice(double p) {
    if (p < 0.0) throw std::invalid_argument("Price cannot be negative");
    price = p;
}

void Medicine::setReorderLevel(int rLevel) {
    if (rLevel < 0) throw std::invalid_argument("Reorder level cannot be negative");
    reorderLevel = rLevel;
}

bool Medicine::dispense(int qty) {
    if (qty <= 0) return false;
    if (quantity < qty) return false;
    quantity -= qty;
    return true;
}

void Medicine::restock(int qty) {
    if (qty > 0) {
        quantity += qty;
    }
}

void Medicine::displayDetails() const {
    std::cout << "Med ID: " << std::setw(6) << id 
              << " | " << std::setw(16) << name 
              << " | Cat: " << std::setw(12) << category 
              << " | Price: Rs. " << std::fixed << std::setprecision(2) << std::setw(6) << price 
              << " | Stock: " << std::setw(4) << quantity 
              << " | Status: " << (isLowStock() ? "[REORDER ALERT]" : "OK") 
              << " | Exp: " << expiryDate << "\n";
}
