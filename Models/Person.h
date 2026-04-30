#ifndef PERSON_H
#define PERSON_H

#include <QString>



class Person {
private:
    // Private = Encapsulation. Only accessible through getter/setter methods.
    QString name;
    int     age;
    QString contact;
    QString city;

public:
    // Constructor: sets up a Person with basic info
    Person(const QString& name, int age, const QString& contact, const QString& city);

    // Virtual destructor: REQUIRED when using polymorphism with pointers.
    // Without this, deleting a Donor* via a Person* would only call Person's destructor.
    virtual ~Person();

    // ---- GETTERS (Read-only access to private data) ----
    QString getName()    const;
    int     getAge()     const;
    QString getContact() const;
    QString getCity()    const;

    // ---- SETTERS (Controlled write access) ----
    void setName(const QString& name);
    void setAge(int age);
    void setContact(const QString& contact);
    void setCity(const QString& city);

    // ---- PURE VIRTUAL FUNCTION = ABSTRACTION ----
    // Every subclass MUST override this. Person cannot be instantiated because of this.
    // POLYMORPHISM: When called on a Person* pointer, the correct
    //               subclass version runs automatically.
    virtual void display() const = 0;

    // ---- Virtual file serialization helper ----
    // Converts object to a comma-separated string for saving to .txt file.
    // Subclasses override this to add their own fields.
    virtual QString toFileString() const;
};

#endif // PERSON_H
