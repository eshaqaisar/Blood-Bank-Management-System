#include "Donor.h"
#include <QDebug>
#include <string>//for std::string, std::stoi, std::stod, std::to_string
#include <QString>//used only at Qt API boundary for QDebug output and QDate conversion

//constructor: initializes a Donor object with the provided information and calls the base class constructor to initialize the common person attributes. The initialization list is used to efficiently set the member variables, and no additional logic is needed in the constructor body since all initialization is handled in the list.
Donor::Donor(const std::string& name, int age, const std::string& contact, const std::string& city,
    const std::string& bloodGroup, double weight, const QDate& lastDonationDate)
    : Person(name, age, contact, city),
    bloodGroup(bloodGroup),
    weight(weight),
    lastDonationDate(lastDonationDate),
    historyCount(0)  //initialise history array as empty
{
    //history array entries are default-constructed to empty strings automatically
}

//getters for the Donor-specific attributes
std::string Donor::getBloodGroup() const
{
    return bloodGroup;
}
double Donor::getWeight() const 
{
    return weight;
}
QDate Donor::getLastDonationDate() const
{
    return lastDonationDate;
}
int Donor::getDonationHistoryCount() const
{
    return historyCount;
}

//returns history entry at index; returns empty string if index is out of range
std::string Donor::getDonationHistoryEntry(int index) const {
    if (index < 0 || index >= historyCount) return "";
    return donationHistory[index];
}

//setters for Donor-specific attributes
void Donor::setBloodGroup(const std::string& bg)
{
    bloodGroup = bg;
}
void Donor::setWeight(double w)
{
    weight = w;
}
void Donor::setLastDonationDate(const QDate& date)
{
    lastDonationDate = date;
}

//adds one entry to the history array; silently ignores if array is full
void Donor::addToDonationHistory(const std::string& e) {
    if (historyCount < MAX_DONATION_HISTORY) {
        donationHistory[historyCount++] = e;
    }
}

//eligibility check for blood donation based on the business rules defined in the application. This method evaluates whether the donor meets all the necessary criteria to be eligible for donating blood, such as being an adult, weighing enough, and having waited the required amount of time since their last donation. The method returns true if all conditions are met, indicating that the donor is eligible to donate, and false otherwise.
//business rule logic all three conditions must be true.
bool Donor::isEligible() const {
    // Rule 1: Must be an adult (at least 18 years old)
    if (getAge() < 18) 
        return false;

    // Rule 2: Must be heavy enough to donate safely
    if (weight < 50.0) 
        return false;

    // Rule 3: Must wait 56 days between donations to recover
    //if lastDonationDate is invalid (null), this is their first donation — allowed.
    if (lastDonationDate.isValid()) {
        int daysSince = lastDonationDate.daysTo(QDate::currentDate());
        if (daysSince < 56)
            return false;  // Too soon since last donation
    }

    return true;  //all rules passed, donor is eligible
}

//POLYMORPHISM: Overrides Person::display()
//when called through a Person* pointer that actually points to a Donor,
//THIS function runs, not Person's version.
void Donor::display() const {
    //convert std::string to QString only at the Qt API boundary (QDebug)
    qDebug() << "===== DONOR PROFILE =====";
    qDebug() << "Name      :" << QString::fromStdString(getName());
    qDebug() << "Age       :" << getAge();
    qDebug() << "Contact   :" << QString::fromStdString(getContact());
    qDebug() << "City      :" << QString::fromStdString(getCity());
    qDebug() << "Blood Grp :" << QString::fromStdString(bloodGroup);
    qDebug() << "Weight    :" << weight << "kg";
    qDebug() << "Eligible  :" << (isEligible() ? "YES" : "NO");
    qDebug() << "Last Donated:" << lastDonationDate.toString("dd-MM-yyyy");
}

//file serialization
//calls base class toFileString() and appends Donor-specific fields
// std::to_string replaces QString::number for numeric conversion
std::string Donor::toFileString() const {
    //result: "name,age,contact,city,bloodGroup,weight,yyyy-MM-dd"
    return Person::toFileString() + "," +
        bloodGroup + "," +
        std::to_string(weight) + "," +
        lastDonationDate.toString("yyyy-MM-dd").toStdString();
    //QDate::toString() returns QString; .toStdString() converts it to std::string at boundary
}

//static factory: reconstruct Donor from one line of donors.txt
// std::string is split manually by comma to avoid QStringList
Donor Donor::fromFileString(const std::string& line) {
    //manual split of std::string by comma into a fixed-size array of parts
    std::string parts[10];
    int count = 0;
    std::string token;
    for (char ch : line) {
        if (ch == ',' && count < 9) {
            parts[count++] = token;
            token.clear();
        }
        else {
            token += ch;
        }
    }
    parts[count++] = token; //last field after final comma

    //expected format: name(0),age(1),contact(2),city(3),bloodGroup(4),weight(5),date(6)
    if (count < 7) {
        //return a default/invalid donor if the line is malformed
        return Donor("UNKNOWN", 0, "N/A", "N/A", "A+", 0.0);
    }

    //QDate::fromString needs a QString; convert std::string at the Qt boundary
    return Donor(
        parts[0],                                                     //name
        std::stoi(parts[1]),                                          //age (std::stoi replaces .toInt())
        parts[2],                                                     //contact
        parts[3],                                                     //city
        parts[4],                                                     //bloodGroup
        std::stod(parts[5]),                                          //weight (std::stod replaces .toDouble())
        QDate::fromString(QString::fromStdString(parts[6]), "yyyy-MM-dd") //lastDonationDate
    );
}