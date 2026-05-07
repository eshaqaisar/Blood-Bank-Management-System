#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"//for the base Person class that Patient inherits from
#include <string>//std::string replaces QString for all string member variables and method signatures

class Patient : public Person {
private:
    std::string requiredBloodGroup;
    int unitsRequired;
    std::string hospitalName;
    std::string requestStatus;

public:
    Patient(const std::string& name, int age, const std::string& contact, const std::string& city,
        const std::string& requiredBloodGroup, int unitsRequired,
        const std::string& hospitalName, const std::string& requestStatus = "Pending");

    //getters
    std::string getRequiredBloodGroup() const;
    int getUnitsRequired()      const;
    std::string getHospitalName()       const;
    std::string getRequestStatus()      const;

    //setters
    void setRequiredBloodGroup(const std::string& bg);
    void setUnitsRequired(int units);
    void setHospitalName(const std::string& hospital);
    void setRequestStatus(const std::string& status);

    //overrides
    void display() const override;
    std::string toFileString() const override;

    static Patient fromFileString(const std::string& line);
};

#endif