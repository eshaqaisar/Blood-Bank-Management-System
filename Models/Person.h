#ifndef PERSON_H
#define PERSON_H

#include <QString>//for storing and manipulating string data such as name, contact information, and city

class Person {
private:
    QString name;
    int     age;
    QString contact;
    QString city;

public:
    Person(const QString& name, int age, const QString& contact, const QString& city);
    virtual ~Person();

    //getters
    QString getName()    const;
    int     getAge()     const;
    QString getContact() const;
    QString getCity()    const;

    //setters
    void setName(const QString& name);
    void setAge(int age);
    void setContact(const QString& contact);
    void setCity(const QString& city);

    //pure virtual
    virtual void display() const = 0;

    //virtual
    virtual QString toFileString() const;
};

#endif