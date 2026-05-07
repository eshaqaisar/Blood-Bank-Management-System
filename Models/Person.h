#ifndef PERSON_H
#define PERSON_H

#include <string>//std::string replaces QString for all member variables and method signatures

class Person {
private:
    std::string name;
    int age;
    std::string contact;
    std::string city;

public:
    Person(const std::string& name, int age, const std::string& contact, const std::string& city);
    virtual ~Person();

    //getters
    std::string getName() const;
    int getAge() const;
    std::string getContact() const;
    std::string getCity() const;

    //setters
    void setName(const std::string& name);
    void setAge(int age);
    void setContact(const std::string& contact);
    void setCity(const std::string& city);

    //pure virtual
    virtual void display() const = 0;

    //virtual - returns comma-separated base fields: name,age,contact,city
    virtual std::string toFileString() const;
};

#endif