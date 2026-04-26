#include "Compatibility.h"
#include <QString>
bool CompatibilityChecker::canDonateTo(QString donorBlood, QString patientBlood) {
	if (donorBlood == "O-") {
		return true;
	}
	if (donorBlood == "O+") {
		return (patientBlood == "O+" || patientBlood == "A+" ||
			patientBlood == "B+" || patientBlood == "AB+");
	}
	if (donorBlood == "A-") {
		return (patientBlood == "A-" || patientBlood == "A+" ||
			patientBlood == "AB+" || patientBlood == "AB-");
	}
	if (donorBlood == "A+") {
		return (patientBlood == "AB+" || patientBlood == "A+");
	}
	if (donorBlood == "B-") {
		return (patientBlood == "B-" || patientBlood == "AB-" ||
			patientBlood == "B+" || patientBlood == "AB+");
	}
	if (donorBlood == "B+") {
		return (patientBlood == "B+" || patientBlood == "AB+");
	}
	if (donorBlood == "AB+") {
		return (patientBlood == "AB+");
	}
	if (donorBlood == "AB-") {
		return (patientBlood == "AB-" || patientBlood == "AB+");
	}
	return false;
}
bool CompatibilityChecker::isCompatible(const QString& d, const QString& r) {
    return canDonateTo(d, r);
}
QStringList CompatibilityChecker::getCompatibleDonors(const QString& recipient) {
    QStringList result;
    for (const QString& donor : {"O-","O+","A-","A+","B-","B+","AB-","AB+"}) {
        if (canDonateTo(donor, recipient)) result.append(donor);
    }
    return result;
}
QStringList CompatibilityChecker::getCompatibleRecipients(const QString& donor) {
    QStringList result;
    for (const QString& r : {"O-","O+","A-","A+","B-","B+","AB-","AB+"}) {
        if (canDonateTo(donor, r)) result.append(r);
    }
    return result;
}