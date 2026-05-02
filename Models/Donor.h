#ifndef DONOR_H
#define DONOR_H

#include "Person.h"//header file for the Person base class that Donor inherits from
#include <QDate>//for handling dates, such as the last donation date and calculating eligibility based on the 56-day gap between donations
#include <QStringList>//for storing the donor's donation history as a list of date strings, which can be displayed in the donor profile and used for tracking past donations



class Donor : public Person {  // "public Person" = Donor inherits from Person
private:
    //donor-specific private attributes (not in person)
    QString    bloodGroup;         //e.g. "A+", "O-", "AB+"
    double     weight;             //in kilograms (must be > 50 to donate)
    QDate      lastDonationDate;   //to enforce 56-day gap between donations
    QStringList donationHistory;   //list of past donation date strings

public:
    //constructor: takes all Person fields + Donor-specific fields
    Donor(const QString& name, int age, const QString& contact, const QString& city,
        const QString& bloodGroup, double weight,
        const QDate& lastDonationDate = QDate()); //default = null date = first-time donor

	//getters for Person fields are inherited from Person class (getName(), getAge(), getContact(), getCity())
    QString     getBloodGroup()       const;
    double      getWeight()           const;
    QDate       getLastDonationDate() const;
    QStringList getDonationHistory()  const;

	//setters for Donor-specific fields (no setters for Person fields to maintain immutability of name, age, contact, city)
    void setBloodGroup(const QString& bg);
    void setWeight(double w);
    void setLastDonationDate(const QDate& date);
    void addToDonationHistory(const QString& entry); // adds one entry to history list

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
    QString toFileString() const override;

    //static factory method: deserialize from CSV (file reading)
    //creates a Donor object from one line read from donors.txt
    static Donor fromFileString(const QString& line);
};

#endif // DONOR_H
