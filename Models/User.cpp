#include "User.h"  //include the header file for the User class
#include <QStringList>//for QStringList used in fromFileString()
#include <QChar>//for QChar used in passwordStrength()

//constructor
User::User(const QString& username, const QString& password, const QString& role)
    : username(username), password(password), role(role)
{
}

//getters
QString User::getUsername() const { return username; }
QString User::getRole()     const { return role; }

//authentication 
bool User::authenticate(const QString& inputPassword) const {
    return password == inputPassword;
}

//serialize for file saving 
QString User::toFileString() const {
    return username + "," + password + "," + role;
}

//reconstruct from a line in users.txt (static) 
User User::fromFileString(const QString& line) {
    QStringList parts = line.split(',');
    if (parts.size() < 3)
        return User("unknown", "1234", "Patient");
    return User(parts[0].trimmed(), parts[1].trimmed(), parts[2].trimmed());
}

//password strength calculator (static) 
int User::passwordStrength(const QString& pw) {
    int score = 0;
    if (pw.length() >= 8) 
        ++score;

    bool hasUpper = false;
    for (QChar c : pw) 
        if (c.isUpper()) 
        { 
            hasUpper = true;
            break;
        }
    if (hasUpper) 
        ++score;

    bool hasDigit = false;
    for (QChar c : pw)
        if (c.isDigit())
        {
            hasDigit = true; 
            break;
        }
    if (hasDigit)
        ++score;

    const QString specials = "!@#$%^&*()_+-=[]{}";
    bool hasSpecial = false;
    for (QChar c : pw)
        if (specials.contains(c))
        {
            hasSpecial = true;
            break;
        }
    if (hasSpecial)
        ++score;

    return score;
}