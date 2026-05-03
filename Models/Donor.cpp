#include "Donor.h"
#include <QDebug>

//constructor: initializes a Donor object with the provided information and calls the base class constructor to initialize the common person attributes. The initialization list is used to efficiently set the member variables, and no additional logic is needed in the constructor body since all initialization is handled in the list.
Donor::Donor(const QString& name, int age, const QString& contact, const QString& city,
    const QString& bloodGroup, double weight, const QDate& lastDonationDate)
    : Person(name, age, contact, city),  
    bloodGroup(bloodGroup),             
    weight(weight),
    lastDonationDate(lastDonationDate)
{
	//no additional logic needed in the constructor body since all initialization is handled in the initialization list
}

//getters for the Donor-specific attributes, allowing access to the donor's blood group, weight, last donation date, and donation history. These methods are const because they do not modify the state of the Donor object and simply return the current values of the member variables.
QString     Donor::getBloodGroup()       const { return bloodGroup; }
double      Donor::getWeight()           const { return weight; }
QDate       Donor::getLastDonationDate() const { return lastDonationDate; }
QStringList Donor::getDonationHistory()  const { return donationHistory; }

//setters for the Donor-specific attributes, allowing modification of the donor's blood group, weight, last donation date, and donation history. These methods allow the state of the Donor object to be updated after it has been created, such as when a donor makes a new donation and their last donation date and history need to be updated accordingly.
void Donor::setBloodGroup(const QString& bg) { bloodGroup = bg; }
void Donor::setWeight(double w) { weight = w; }
void Donor::setLastDonationDate(const QDate& date) { lastDonationDate = date; }
void Donor::addToDonationHistory(const QString& e) { donationHistory.append(e); }

//eligibility check for blood donation based on the business rules defined in the application. This method evaluates whether the donor meets all the necessary criteria to be eligible for donating blood, such as being an adult, weighing enough, and having waited the required amount of time since their last donation. The method returns true if all conditions are met, indicating that the donor is eligible to donate, and false otherwise.
//business rule logic all three conditions must be true.
bool Donor::isEligible() const {
    // Rule 1: Must be an adult (at least 18 years old)
    if (getAge() < 18) return false;

    // Rule 2: Must be heavy enough to donate safely
    if (weight < 50.0) return false;

    // Rule 3: Must wait 56 days between donations to recover
    //if lastDonationDate is invalid (null), this is their first donation — allowed.
    if (lastDonationDate.isValid()) 
    {
        int daysSince = lastDonationDate.daysTo(QDate::currentDate());
        if (daysSince < 56) return false;  // Too soon since last donation
    }

    return true;  //all rules passed , donor is eligible
}

//POLYMORPHISM: Overrides Person::display() 
//when called through a Person* pointer that actually points to a Donor,
//THIS function runs, not Person's version.
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

//file serialization
//calls base class toFileString() and appends Donor-specific fields
QString Donor::toFileString() const {
    //result: "name,age,contact,city,bloodGroup,weight,yyyy-MM-dd"
    return Person::toFileString() + "," +
        bloodGroup + "," +
        QString::number(weight) + "," +
        lastDonationDate.toString("yyyy-MM-dd");
}

//static factory: reconstruct Donor from one line of donors.txt
Donor Donor::fromFileString(const QString& line) {
    QStringList p = line.split(",");
    //expected format: name(0),age(1),contact(2),city(3),bloodGroup(4),weight(5),date(6)
    if (p.size() < 7) {
        //return a default/invalid donor if the line is malformed
        return Donor("UNKNOWN", 0, "N/A", "N/A", "A+", 0.0);
    }
    return Donor(
        p[0],                                   //name
        p[1].toInt(),                           //age
        p[2],                                   //contact
        p[3],                                   //city
        p[4],                                   //bloodGroup
        p[5].toDouble(),                        //weight
        QDate::fromString(p[6], "yyyy-MM-dd")  //lastDonationDate
    );
}
