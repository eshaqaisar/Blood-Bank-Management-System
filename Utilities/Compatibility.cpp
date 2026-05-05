#include "Compatibility.h"//header file for the CompatibilityChecker class, which provides methods for checking blood group compatibility between donors and recipients, as well as getting lists of compatible donors and recipients based on blood type
#include <string>//for std::string used in all method signatures and comparisons

// All comparisons use std::string == operator directly; no Qt types needed here.

bool CompatibilityChecker::canDonateTo(const std::string& donorBlood, const std::string& patientBlood) {
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
bool CompatibilityChecker::isCompatible(const std::string& d, const std::string& r) {
    return canDonateTo(d, r);
}

//the getCompatibleDonors method returns a CompatibleList of blood types that can donate to the specified recipient blood type. It iterates through all possible donor blood types and checks if each one can donate to the recipient using the canDonateTo method. CompatibleList replaces QStringList to avoid Qt list types.
CompatibleList CompatibilityChecker::getCompatibleDonors(const std::string& recipient) {
    CompatibleList result;
    const std::string donors[] = { "O-","O+","A-","A+","B-","B+","AB-","AB+" };
    for (int i = 0; i < 8; i++) {
        if (canDonateTo(donors[i], recipient)) result.add(donors[i]);
    }
    return result;
}

//the getCompatibleRecipients method returns a CompatibleList of blood types that can receive blood from the specified donor blood type. It iterates through all possible recipient blood types and checks if the donor can donate to each recipient using the canDonateTo method. CompatibleList replaces QStringList to avoid Qt list types.
CompatibleList CompatibilityChecker::getCompatibleRecipients(const std::string& donor) {
    CompatibleList result;
    const std::string recipients[] = { "O-","O+","A-","A+","B-","B+","AB-","AB+" };
    for (int i = 0; i < 8; i++) {
        if (canDonateTo(donor, recipients[i])) result.add(recipients[i]);
    }
    return result;
}