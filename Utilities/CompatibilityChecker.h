#pragma once
#ifndef COMPATIBILITYCHECKER_H
#define COMPATIBILITYCHECKER_H
#include <string>
using namespace std;

class CompatibilityChecker {
public:
    static bool canDonateTo(string donorBlood, string patientBlood);
};

#endif