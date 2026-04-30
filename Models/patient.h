#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"



class Patient : public Person {
private:
    // Patient-specific private attributes
    QString requiredBloodGroup; // What blood type does the patient need?
    int     unitsRequired;      // How many units of blood are needed?
    QString hospitalName;       // Where is the patient?
    QString requestStatus;      // "Pending", "Approved", "Rejected"

public:
    // Constructor
    Patient(const QString& name, int age, const QString& contact, const QString& city,
        const QString& requiredBloodGroup, int unitsRequired,
        const QString& hospitalName, const QString& requestStatus = "Pending");

    // ---- Getters ----
    QString getRequiredBloodGroup() const;
    int     getUnitsRequired()      const;
    QString getHospitalName()       const;
    QString getRequestStatus()      const;

    // ---- Setters ----
    void setRequiredBloodGroup(const QString& bg);
    void setUnitsRequired(int units);
    void setHospitalName(const QString& hospital);
    void setRequestStatus(const QString& status);

    // ---- POLYMORPHISM: Overrides Person::display() ----
    void display() const override;

    // ---- File Handling ----
    QString toFileString() const override;
    static Patient fromFileString(const QString& line);
};

#endif // PATIENT_H
