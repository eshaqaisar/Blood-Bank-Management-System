#include "Person.h"//header file for the Person class
#include <string>//for std::string and std::to_string used throughout

Person::Person(const std::string& name, int age, const std::string& contact, const std::string& city)
    : name(name), age(age), contact(contact), city(city)
{
}

Person::~Person() {}

//getters
std::string Person::getName()    const { return name; }
int         Person::getAge()     const { return age; }
std::string Person::getContact() const { return contact; }
std::string Person::getCity()    const { return city; }

//setters
void Person::setName(const std::string& n) { name = n; }
void Person::setAge(int a) { age = a; }
void Person::setContact(const std::string& c) { contact = c; }
void Person::setCity(const std::string& c) { city = c; }

//base file string: name,age,contact,city
// std::to_string replaces QString::number for int-to-string conversion
std::string Person::toFileString() const {
    return name + "," +
        std::to_string(age) + "," +
        contact + "," +
        city;
}