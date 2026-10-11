#ifndef MEDICINE_H
#define MEDICINE_H

#include <string>
#include <iostream>

// ============================================================================
// PSOOP CO1: Medicine Class
// Demonstrates Pharmacy Inventory Management and Stock Validation
// ============================================================================
class Medicine {
private:
    std::string id;
    std::string name;
    std::string genericName;
    std::string category; // Antibiotic, Analgesic, Antipyretic, Cardiac, etc.
    std::string manufacturer;
    double price;
    int quantity;
    int reorderLevel;
    std::string expiryDate;
    std::string batchNumber;

public:
    Medicine();
    Medicine(const std::string& id, const std::string& name, const std::string& genericName,
             const std::string& category, const std::string& manufacturer,
             double price, int quantity, int reorderLevel,
             const std::string& expiryDate, const std::string& batchNumber);

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getGenericName() const { return genericName; }
    std::string getCategory() const { return category; }
    std::string getManufacturer() const { return manufacturer; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getReorderLevel() const { return reorderLevel; }
    std::string getExpiryDate() const { return expiryDate; }
    std::string getBatchNumber() const { return batchNumber; }

    bool isLowStock() const { return quantity <= reorderLevel; }

    void setPrice(double p);
    void setReorderLevel(int rLevel);
    void setExpiryDate(const std::string& exp) { expiryDate = exp; }

    bool dispense(int qty);
    void restock(int qty);

    void displayDetails() const;
};

#endif // MEDICINE_H
