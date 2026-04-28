#pragma once
#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"

class Patient : public Person {
private:
    QString hospitalName;

public:
    Patient(QString n, int a, QString c, QString bg, QString hName);

    QString getHospitalName() const { return hospitalName; }
};

#endif