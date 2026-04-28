#include "Donor.h"

Donor::Donor(QString n, int a, QString c, QString bg, double w)
    : Person(n, a, c, bg), weight(w) {
    isEligible = checkEligibility();
}

bool Donor::checkEligibility() const {
    return (age >= 18 && weight >= 50.0);
}