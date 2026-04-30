// BloodBag.cpp
#include "BloodBag.h"

// Implementation of the BloodBag class with serialization, expiry checking, compatibility, and inventory management.

BloodBag::BloodBag(int id, const std::string& type, const std::string& expiryDate) : id(id), type(type), expiryDate(expiryDate) {}

void BloodBag::serialize(std::ostream& out) const {
    out << id << '\n' << type << '\n' << expiryDate << '\n';
}

bool BloodBag::isExpired(const std::string& currentDate) const {
    // Simplified check; you may want to use a date library
    return currentDate > expiryDate;
}

// Additional methods for BloodInventory management

BloodInventory::BloodInventory() {}

void BloodInventory::addBloodBag(const BloodBag& bag) {
    bags.push_back(bag);
}

// Other methods such as checking inventory, listing bags, compatibility, etc. 
