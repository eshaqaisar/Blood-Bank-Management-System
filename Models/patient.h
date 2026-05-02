#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"//for the base Person class that Patient inherits from

class Patient : public Person {
private:
    QString requiredBloodGroup;
    int     unitsRequired;
    QString hospitalName;
    QString requestStatus;

public:
    Patient(const QString& name, int age, const QString& contact, const QString& city,
        const QString& requiredBloodGroup, int unitsRequired,
        const QString& hospitalName, const QString& requestStatus = "Pending");

    //getters
    QString getRequiredBloodGroup() const;
    int     getUnitsRequired() const;
    QString getHospitalName() const;
    QString getRequestStatus() const;

    //setters
    void setRequiredBloodGroup(const QString& bg);
    void setUnitsRequired(int units);
    void setHospitalName(const QString& hospital);
    void setRequestStatus(const QString& status);

    //overrides
    void display() const override;
    QString toFileString() const override;

    static Patient fromFileString(const QString& line);
};

#endif