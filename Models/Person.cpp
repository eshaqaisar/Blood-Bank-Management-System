#include "Person.h"

Person::Person(const QString& name, int age, const QString& contact, const QString& city)
    : name(name), age(age), contact(contact), city(city)
{
}

Person::~Person() {}

// Getters
QString Person::getName() const { return name; }
int Person::getAge() const { return age; }
QString Person::getContact() const { return contact; }
QString Person::getCity() const { return city; }

// Setters
void Person::setName(const QString& n) { name = n; }
void Person::setAge(int a) { age = a; }
void Person::setContact(const QString& c) { contact = c; }
void Person::setCity(const QString& c) { city = c; }

// Base file string
QString Person::toFileString() const {
    return name + "," +
        QString::number(age) + "," +
        contact + "," +
        city;
}