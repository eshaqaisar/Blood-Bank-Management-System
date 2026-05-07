#include "Patient.h"
#include <string>//for std::string, std::stoi, std::to_string
#include <QDebug>//used only at Qt boundary for debug output; convert std::string→QString there

//constructor
Patient::Patient(const std::string& name, int age, const std::string& contact, const std::string& city,
    const std::string& requiredBloodGroup, int unitsRequired,
    const std::string& hospitalName, const std::string& requestStatus)
    : Person(name, age, contact, city)
{
    this->requiredBloodGroup = requiredBloodGroup;
    this->unitsRequired = unitsRequired;
    this->hospitalName = hospitalName;
    this->requestStatus = requestStatus;
}

//getters
std::string Patient::getRequiredBloodGroup() const
{
    return requiredBloodGroup;
}
int Patient::getUnitsRequired() const 
{
    return unitsRequired;
}
std::string Patient::getHospitalName() const 
{
    return hospitalName;
}
std::string Patient::getRequestStatus() const
{
    return requestStatus;
}

//setters
void Patient::setRequiredBloodGroup(const std::string& bg) 
{
    requiredBloodGroup = bg;
}
void Patient::setUnitsRequired(int units)
{
    unitsRequired = units;
}
void Patient::setHospitalName(const std::string& hospital)
{ 
    hospitalName = hospital;
}
void Patient::setRequestStatus(const std::string& status)
{
    requestStatus = status;
}

//display() uses getters; converts to QString only at QDebug boundary
void Patient::display() const {
    qDebug() << "Name:" << QString::fromStdString(getName());
    qDebug() << "Age:" << getAge();
    qDebug() << "Contact:" << QString::fromStdString(getContact());
    qDebug() << "City:" << QString::fromStdString(getCity());
    qDebug() << "Blood Group Needed:" << QString::fromStdString(requiredBloodGroup);
    qDebug() << "Units Required:" << unitsRequired;
    qDebug() << "Hospital:" << QString::fromStdString(hospitalName);
    qDebug() << "Status:" << QString::fromStdString(requestStatus);
}

//toFileString() returns a comma-separated std::string
// std::to_string replaces QString::number for int conversion
std::string Patient::toFileString() const {
    return getName() + "," +
        std::to_string(getAge()) + "," +
        getContact() + "," +
        getCity() + "," +
        requiredBloodGroup + "," +
        std::to_string(unitsRequired) + "," +
        hospitalName + "," +
        requestStatus;
}

//fromFileString() - manual comma split replaces QStringList / line.split(',')
Patient Patient::fromFileString(const std::string& line) {
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
    parts[count++] = token; //last field

    if (count < 8) {
        return Patient("", 0, "", "", "", 0, "", "");
    }

    return Patient(
        parts[0],               //name
        std::stoi(parts[1]),    //age
        parts[2],               //contact
        parts[3],               //city
        parts[4],               //required blood group
        std::stoi(parts[5]),    //units required
        parts[6],               //hospital name
        parts[7]                //request status
    );
}