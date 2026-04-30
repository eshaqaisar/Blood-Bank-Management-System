#include "Donor.h"
#include <QDebug>


Donor::Donor(const QString& name, int age, const QString& contact, const QString& city,
    const QString& bloodGroup, double weight, const QDate& lastDonationDate)
    : Person(name, age, contact, city),   // Step 1: Initialize base class (Person)
    bloodGroup(bloodGroup),             // Step 2: Initialize Donor-specific fields
    weight(weight),
    lastDonationDate(lastDonationDate)
{
    // Nothing else needed — init list handled everything
}

// ---- Getters ----
QString     Donor::getBloodGroup()       const { return bloodGroup; }
double      Donor::getWeight()           const { return weight; }
QDate       Donor::getLastDonationDate() const { return lastDonationDate; }
QStringList Donor::getDonationHistory()  const { return donationHistory; }

// ---- Setters ----
void Donor::setBloodGroup(const QString& bg) { bloodGroup = bg; }
void Donor::setWeight(double w) { weight = w; }
void Donor::setLastDonationDate(const QDate& date) { lastDonationDate = date; }
void Donor::addToDonationHistory(const QString& e) { donationHistory.append(e); }

// ---- Eligibility Check ----
// This is the business rule logic. All three conditions must be true.
bool Donor::isEligible() const {
    // Rule 1: Must be an adult (at least 18 years old)
    if (getAge() < 18) return false;

    // Rule 2: Must be heavy enough to donate safely
    if (weight < 50.0) return false;

    // Rule 3: Must wait 56 days between donations to recover
    // If lastDonationDate is invalid (null), this is their first donation — allowed.
    if (lastDonationDate.isValid()) {
        int daysSince = lastDonationDate.daysTo(QDate::currentDate());
        if (daysSince < 56) return false;  // Too soon since last donation
    }

    return true;  // All rules passed — donor is eligible
}

// ---- POLYMORPHISM: Overrides Person::display() ----
// When called through a Person* pointer that actually points to a Donor,
// THIS function runs, not Person's version.
void Donor::display() const {
    qDebug() << "===== DONOR PROFILE =====";
    qDebug() << "Name      :" << getName();
    qDebug() << "Age       :" << getAge();
    qDebug() << "Contact   :" << getContact();
    qDebug() << "City      :" << getCity();
    qDebug() << "Blood Grp :" << bloodGroup;
    qDebug() << "Weight    :" << weight << "kg";
    qDebug() << "Eligible  :" << (isEligible() ? "YES" : "NO");
    qDebug() << "Last Donated:" << lastDonationDate.toString("dd-MM-yyyy");
}

// ---- File Serialization ----
// Calls base class toFileString() and appends Donor-specific fields
QString Donor::toFileString() const {
    // Result: "name,age,contact,city,bloodGroup,weight,yyyy-MM-dd"
    return Person::toFileString() + "," +
        bloodGroup + "," +
        QString::number(weight) + "," +
        lastDonationDate.toString("yyyy-MM-dd");
}

// ---- Static Factory: Reconstruct Donor from one line of donors.txt ----
Donor Donor::fromFileString(const QString& line) {
    QStringList p = line.split(",");
    // Expected format: name(0),age(1),contact(2),city(3),bloodGroup(4),weight(5),date(6)
    if (p.size() < 7) {
        // Return a default/invalid donor if the line is malformed
        return Donor("UNKNOWN", 0, "N/A", "N/A", "A+", 0.0);
    }
    return Donor(
        p[0],                                   // name
        p[1].toInt(),                           // age
        p[2],                                   // contact
        p[3],                                   // city
        p[4],                                   // bloodGroup
        p[5].toDouble(),                        // weight
        QDate::fromString(p[6], "yyyy-MM-dd")  // lastDonationDate
    );
}
