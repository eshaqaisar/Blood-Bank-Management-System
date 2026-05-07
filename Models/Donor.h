#ifndef DONOR_H
#define DONOR_H

#include "Person.h"//header file for the Person base class that Donor inherits from
#include <QDate>//for handling dates, such as the last donation date and calculating eligibility based on the 56-day gap between donations
#include <string>//std::string replaces QString for all string member variables and method signatures

// QStringList (donation history) replaced with a fixed-size array of std::string
// to avoid using Qt list types or STL containers beyond basics
#define MAX_DONATION_HISTORY 50

class Donor : public Person {  // "public Person" = Donor inherits from Person
private:
    //donor-specific private attributes (not in person)
    std::string bloodGroup;         //e.g. "A+", "O-", "AB+"
    double weight;             //in kilograms (must be > 50 to donate)
    QDate lastDonationDate;   //to enforce 56-day gap between donations
    //donation history stored as plain array instead of QStringList
    std::string donationHistory[MAX_DONATION_HISTORY];
    int historyCount;       //tracks how many entries are filled

public:
    //constructor: takes all Person fields + Donor-specific fields
    Donor(const std::string& name, int age, const std::string& contact, const std::string& city,
        const std::string& bloodGroup, double weight,
        const QDate& lastDonationDate = QDate()); //default = null date = first-time donor

    //getters for Person fields are inherited from Person class (getName(), getAge(), getContact(), getCity())
    std::string getBloodGroup() const;
    double getWeight() const;
    QDate getLastDonationDate() const;
    //returns history entry at index; returns "" if index out of range
    std::string getDonationHistoryEntry(int index) const;
    int getDonationHistoryCount() const;

    //setters for Donor-specific fields
    void setBloodGroup(const std::string& bg);
    void setWeight(double w);
    void setLastDonationDate(const QDate& date);
    void addToDonationHistory(const std::string& entry); //adds one entry to history array

    //business logic method to check if the donor is currently eligible to donate based on their age, weight, and last donation date. This method checks if the donor meets all eligibility criteria, including being over 18 years old, weighing more than 50 kg, and having at least 56 days since their last donation (or being a first-time donor with no previous donation date). The method returns true if the donor is eligible to donate blood, and false otherwise.
    //returns true if donor meets ALL eligibility criteria:
    //   1. Age > 18
    //   2. Weight > 50 kg
    //   3. At least 56 days since last donation (or no previous donation)
    bool isEligible() const;

    //POLYMORPHISM: overrides Person's pure virtual display()
    //this is what gets called when you do: Person* p = new Donor(); p->display();
    void display() const override;

    //file handling: serialize to CSV
    //format: name,age,contact,city,bloodGroup,weight,lastDonationDate
    std::string toFileString() const override;

    //static factory method: deserialize from CSV (file reading)
    //creates a Donor object from one line read from donors.txt
    static Donor fromFileString(const std::string& line);
};

#endif // DONOR_H