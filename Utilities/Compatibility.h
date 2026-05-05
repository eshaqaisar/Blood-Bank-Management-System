#ifndef COMPATIBILITY_H
#define COMPATIBILITY_H

#include <string>//std::string replaces QString for blood group strings

// CompatibleList replaces QStringList for returning lists of blood groups.
// Uses a fixed-size plain array (max 8 blood groups) to avoid Qt list types.
struct CompatibleList {
    std::string groups[8]; //max 8 standard blood groups
    int         count;     //number of entries actually filled

    CompatibleList() : count(0) {}

    void add(const std::string& g) {
        if (count < 8) groups[count++] = g;
    }
};

class CompatibilityChecker {
public:
    static bool           canDonateTo(const std::string& donorBlood, const std::string& patientBlood);
    static bool           isCompatible(const std::string& donorGroup, const std::string& recipientGroup);
    static CompatibleList getCompatibleDonors(const std::string& recipientGroup);
    static CompatibleList getCompatibleRecipients(const std::string& donorGroup);
};

#endif