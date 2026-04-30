#ifndef DONOR_H
#define DONOR_H

#include "Person.h"
#include <QDate>
#include <QStringList>



class Donor : public Person {  // "public Person" = Donor inherits from Person
private:
    // Donor-specific private attributes (not in Person)
    QString    bloodGroup;         // e.g. "A+", "O-", "AB+"
    double     weight;             // in kilograms (must be > 50 to donate)
    QDate      lastDonationDate;   // to enforce 56-day gap between donations
    QStringList donationHistory;   // list of past donation date strings

public:
    // Constructor: takes all Person fields + Donor-specific fields
    Donor(const QString& name, int age, const QString& contact, const QString& city,
        const QString& bloodGroup, double weight,
        const QDate& lastDonationDate = QDate()); // default = null date = first-time donor

    // ---- Getters for Donor-specific data ----
    QString     getBloodGroup()       const;
    double      getWeight()           const;
    QDate       getLastDonationDate() const;
    QStringList getDonationHistory()  const;

    // ---- Setters for Donor-specific data ----
    void setBloodGroup(const QString& bg);
    void setWeight(double w);
    void setLastDonationDate(const QDate& date);
    void addToDonationHistory(const QString& entry); // adds one entry to history list

    // ---- Business Logic ----
    // Returns true if donor meets ALL eligibility criteria:
    //   1. Age > 18
    //   2. Weight > 50 kg
    //   3. At least 56 days since last donation (or no previous donation)
    bool isEligible() const;

    // ---- POLYMORPHISM: Overrides Person's pure virtual display() ----
    // This is what gets called when you do: Person* p = new Donor(); p->display();
    void display() const override;

    // ---- File Handling: Serialize to CSV ----
    // Format: name,age,contact,city,bloodGroup,weight,lastDonationDate
    QString toFileString() const override;

    // ---- Static Factory Method: Deserialize from CSV (file reading) ----
    // Creates a Donor object from one line read from donors.txt
    static Donor fromFileString(const QString& line);
};

#endif // DONOR_H
