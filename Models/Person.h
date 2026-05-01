#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person {
private:
    QString name;
    int     age;
    QString contact;
    QString city;

public:
    Person(const QString& name, int age, const QString& contact, const QString& city);
    virtual ~Person();

    // Getters
    QString getName()    const;
    int     getAge()     const;
    QString getContact() const;
    QString getCity()    const;

    // Setters
    void setName(const QString& name);
    void setAge(int age);
    void setContact(const QString& contact);
    void setCity(const QString& city);

    // Pure virtual
    virtual void display() const = 0;

    // Virtual
    virtual QString toFileString() const;
};

#endif