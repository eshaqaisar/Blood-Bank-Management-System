
#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H
#include <QString>//for handling blood group strings and other text data
#include <QList>//for returning lists of compatible blood groups in the compatibility checker functions



class CompatibilityChecker {
public:
    static bool canDonateTo(QString donorBlood, QString patientBlood);
    static bool isCompatible(const QString& donorGroup, const QString& recipientGroup);
    static QStringList getCompatibleDonors(const QString& recipientGroup);
    static QStringList getCompatibleRecipients(const QString& donorGroup);
};

#endif