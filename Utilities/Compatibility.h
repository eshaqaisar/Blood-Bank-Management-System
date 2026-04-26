
#ifndef COMPATIBILITYCHECKER_H
#define COMPATIBILITYCHECKER_H
#include <QString>


class CompatibilityChecker {
public:
    static bool canDonateTo(QString donorBlood, QString patientBlood);
};

#endif