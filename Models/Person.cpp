#include "Person.h"
#include <QDebug>


Person::Person(const QString& name, int age, const QString& contact, const QString& city)
    : name(name), age(age), contact(contact), city(city)
{
    // Body intentionally empty - initialization list handles everything
}

Person::~Person()
{
    // Virtual destructor body - subclasses get a chance to clean up first
}

// ---- GETTER Implementations ----
// These return copies of private data, so external code can READ but not MODIFY.
QString Person::getName()    const { return name; }
int     Person::getAge()     const { return age; }
QString Person::getContact() const { return contact; }
QString Person::getCity()    const { return city; }

// ---- SETTER Implementations ----
// Setters allow controlled modification. You could add validation here.
void Person::setName(const QString& n) { name = n; }
void Person::setAge(int a) { age = a; }
void Person::setContact(const QString& c) { contact = c; }
void Person::setCity(const QString& c) { city = c; }

// ---- Base toFileString ----
// Returns "name,age,contact,city" — subclasses call this then append their own fields.
QString Person::toFileString() const {
    return name + "," + QString::number(age) + "," + contact + "," + city;
}
