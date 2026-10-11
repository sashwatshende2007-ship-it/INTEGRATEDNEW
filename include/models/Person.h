#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

// ============================================================================
// PSOOP CO1: Person Abstract Base Class
// Demonstrates Abstraction & Encapsulation
// ============================================================================
class Person {
protected:
    std::string id;
    std::string name;
    std::string gender;
    std::string contactNumber;
    std::string address;
    int age;

public:
    Person();
    Person(const std::string& id, const std::string& name, const std::string& gender,
           const std::string& contact, const std::string& address, int age);
    virtual ~Person() = default;

    // Pure virtual methods enforcing polymorphism
    virtual void displayDetails() const = 0;
    virtual std::string getRole() const = 0;

    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getGender() const { return gender; }
    std::string getContactNumber() const { return contactNumber; }
    std::string getAddress() const { return address; }
    int getAge() const { return age; }

    // Setters with validation
    void setId(const std::string& newId);
    void setName(const std::string& newName);
    void setGender(const std::string& newGender);
    void setContactNumber(const std::string& newContact);
    void setAddress(const std::string& newAddress);
    void setAge(int newAge);
};

#endif // PERSON_H
