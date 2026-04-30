// BloodBag.h

#ifndef BLOODBAG_H
#define BLOODBAG_H

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

class BloodBag {
private:
    std::string bloodType;
    std::string expiryDate;
    float volume;

public:
    BloodBag(std::string bType, std::string expDate, float vol) 
        : bloodType(bType), expiryDate(expDate), volume(vol) {}

    std::string getBloodType() const { return bloodType; }
    std::string getExpiryDate() const { return expiryDate; }
    float getVolume() const { return volume; }

    bool isExpired() const {
        std::tm expDate = {}; 
        strptime(expiryDate.c_str(), "%Y-%m-%d", &expDate);
        std::time_t expiryTime = std::mktime(&expDate);
        return std::difftime(std::time(0), expiryTime) > 0;
    }

    void saveToFile(const std::string &filename) const {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << bloodType << ',' << expiryDate << ',' << volume << '\n';
            file.close();
        }
    }
};

class BloodInventory {
private:
    std::vector<BloodBag> inventory;

public:
    void addBloodBag(const BloodBag &bag) {
        inventory.push_back(bag);
    }

    void displayInventory() const {
        for (const auto &bag : inventory) {
            std::cout << "Blood Type: " << bag.getBloodType() << ", Expiry Date: " << bag.getExpiryDate() << ", Volume: " << bag.getVolume() << std::endl;
        }
    }
};

#endif // BLOODBAG_H