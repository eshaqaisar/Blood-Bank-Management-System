
#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H
#include <QString>


class CompatibilityChecker {
public:
    static bool canDonateTo(QString donorBlood, QString patientBlood);
    static bool isCompatible(const QString& donorGroup, const QString& recipientGroup);
    static QStringList getCompatibleDonors(const QString& recipientGroup);
    static QStringList getCompatibleRecipients(const QString& donorGroup);
};

#endif