#include "Patient.h"
#include <QStringList>
#include <QDebug>

// Constructor
Patient::Patient(const QString& name, int age, const QString& contact, const QString& city,
    const QString& requiredBloodGroup, int unitsRequired,
    const QString& hospitalName, const QString& requestStatus)
    : Person(name, age, contact, city)
{
    this->requiredBloodGroup = requiredBloodGroup;
    this->unitsRequired = unitsRequired;
    this->hospitalName = hospitalName;
    this->requestStatus = requestStatus;
}

// Getters
QString Patient::getRequiredBloodGroup() const {
    return requiredBloodGroup;
}

int Patient::getUnitsRequired() const {
    return unitsRequired;
}

QString Patient::getHospitalName() const {
    return hospitalName;
}

QString Patient::getRequestStatus() const {
    return requestStatus;
}

// Setters
void Patient::setRequiredBloodGroup(const QString& bg) {
    requiredBloodGroup = bg;
}

void Patient::setUnitsRequired(int units) {
    unitsRequired = units;
}

void Patient::setHospitalName(const QString& hospital) {
    hospitalName = hospital;
}

void Patient::setRequestStatus(const QString& status) {
    requestStatus = status;
}

// display() → FIXED (no base call)
void Patient::display() const {
    qDebug() << "Name:" << getName();
    qDebug() << "Age:" << getAge();
    qDebug() << "Contact:" << getContact();
    qDebug() << "City:" << getCity();

    qDebug() << "Blood Group Needed:" << requiredBloodGroup;
    qDebug() << "Units Required:" << unitsRequired;
    qDebug() << "Hospital:" << hospitalName;
    qDebug() << "Status:" << requestStatus;
}

// toFileString() → FIXED (uses getters)
QString Patient::toFileString() const {
    return getName() + "," +
        QString::number(getAge()) + "," +
        getContact() + "," +
        getCity() + "," +
        requiredBloodGroup + "," +
        QString::number(unitsRequired) + "," +
        hospitalName + "," +
        requestStatus;
}

// fromFileString() → SAFE
Patient Patient::fromFileString(const QString& line) {
    QStringList parts = line.split(",");

    if (parts.size() < 8) {
        return Patient("", 0, "", "", "", 0, "", "");
    }

    return Patient(
        parts[0],
        parts[1].toInt(),
        parts[2],
        parts[3],
        parts[4],
        parts[5].toInt(),
        parts[6],
        parts[7]
    );
}