#pragma once
#ifndef DONOR_H
#define DONOR_H

#include "Person.h"

class Donor : public Person {
private:
    double weight;
    bool isEligible;

public:
    Donor(QString n, int a, QString c, QString bg, double w);

    
        bool checkEligibility() const;

    double getWeight() const { return weight; }
    bool getIsEligible() const { return isEligible; }
};

#endif