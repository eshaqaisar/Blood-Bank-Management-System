#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person {
protected:
    QString name;
    int age;
    QString contact;
    QString bloodGroup;

public:
    Person(QString n = "", int a = 0, QString c = "", QString bg = "");
    virtual ~Person() {}

    // Getters
    QString getName() const { return name; }
    int getAge() const { return age; }
    QString getContact() const { return contact; }
    QString getBloodGroup() const { return bloodGroup; }
};

#endif
