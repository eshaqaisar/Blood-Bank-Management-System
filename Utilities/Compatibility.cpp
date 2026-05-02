#include "Compatibility.h"//header file for the CompatibilityChecker class, which provides methods for checking blood group compatibility between donors and recipients, as well as getting lists of compatible donors and recipients based on blood type
#include <QString>//for using QString in the method signatures and implementations
bool CompatibilityChecker::canDonateTo(QString donorBlood, QString patientBlood) {
	if (donorBlood == "O-") 
	{
		return true;
	}
	if (donorBlood == "O+")
	{
		return (patientBlood == "O+" || patientBlood == "A+" ||
			patientBlood == "B+" || patientBlood == "AB+");
	}
	if (donorBlood == "A-")
	{
		return (patientBlood == "A-" || patientBlood == "A+" ||
			patientBlood == "AB+" || patientBlood == "AB-");
	}
	if (donorBlood == "A+")
	{
		return (patientBlood == "AB+" || patientBlood == "A+");
	}
	if (donorBlood == "B-")
	{
		return (patientBlood == "B-" || patientBlood == "AB-" ||
			patientBlood == "B+" || patientBlood == "AB+");
	}
	if (donorBlood == "B+")
	{
		return (patientBlood == "B+" || patientBlood == "AB+");
	}
	if (donorBlood == "AB+")
	{
		return (patientBlood == "AB+");
	}
	if (donorBlood == "AB-") 
	{
		return (patientBlood == "AB-" || patientBlood == "AB+");
	}
	return false;
}
//the isCompatible method is a simple wrapper around canDonateTo, providing a more intuitive method name for checking compatibility between a donor and recipient blood type. It returns true if the donor can donate to the recipient based on their blood types, and false otherwise.
bool CompatibilityChecker::isCompatible(const QString& d, const QString& r) {
    return canDonateTo(d, r);
}
//the getCompatibleDonors method returns a list of blood types that can donate to the specified recipient blood type. It iterates through all possible donor blood types and checks if each one can donate to the recipient using the canDonateTo method. If a donor blood type is compatible, it is added to the result list, which is returned at the end.
QStringList CompatibilityChecker::getCompatibleDonors(const QString& recipient) {
    QStringList result;
    for (const QString& donor : {"O-","O+","A-","A+","B-","B+","AB-","AB+"}) {
        if (canDonateTo(donor, recipient)) result.append(donor);
    }
    return result;
}
//the getCompatibleRecipients method returns a list of blood types that can receive blood from the specified donor blood type. It iterates through all possible recipient blood types and checks if the donor can donate to each recipient using the canDonateTo method. If the donor is compatible with a recipient blood type, it is added to the result list, which is returned at the end.
QStringList CompatibilityChecker::getCompatibleRecipients(const QString& donor) {
    QStringList result;
    for (const QString& r : {"O-","O+","A-","A+","B-","B+","AB-","AB+"}) {
        if (canDonateTo(donor, r)) 
			result.append(r);
    }
    return result;
}