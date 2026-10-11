#include "models/Person.h"
#include <stdexcept>

Person::Person() : age(0) {}

Person::Person(const std::string& id, const std::string& name, const std::string& gender,
               const std::string& contact, const std::string& address, int age)
    : id(id), name(name), gender(gender), contactNumber(contact), address(address), age(age) {
    if (age < 0 || age > 130) {
        throw std::invalid_argument("Age must be between 0 and 130");
    }
}

void Person::setId(const std::string& newId) {
    if (newId.empty()) throw std::invalid_argument("ID cannot be empty");
    id = newId;
}

void Person::setName(const std::string& newName) {
    if (newName.empty()) throw std::invalid_argument("Name cannot be empty");
    name = newName;
}

void Person::setGender(const std::string& newGender) {
    gender = newGender;
}

void Person::setContactNumber(const std::string& newContact) {
    contactNumber = newContact;
}

void Person::setAddress(const std::string& newAddress) {
    address = newAddress;
}

void Person::setAge(int newAge) {
    if (newAge < 0 || newAge > 130) {
        throw std::invalid_argument("Age must be between 0 and 130");
    }
    age = newAge;
}
