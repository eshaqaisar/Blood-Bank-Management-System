#include "Models/User.h"
#include <QStringList>   // for split()
#include <QChar>         // for isUpper(), isDigit()

// Constructor
User::User(const QString& username, const QString& password, const QString& role)
    : username(username), password(password), role(role)
{
}

// ---- Getters ----
QString User::getUsername() const { return username; }
QString User::getRole() const { return role; }

// ---- Authentication ----
bool User::authenticate(const QString& inputPassword) const {
    return password == inputPassword;
}

// ---- Serialize for file saving ----
QString User::toFileString() const {
    return username + "," + password + "," + role;
}

// ---- Reconstruct from a line in users.txt (static) ----
User User::fromFileString(const QString& line) {
    QStringList parts = line.split(',');
    if (parts.size() < 3)
        return User("unknown", "1234", "Patient");
    return User(parts[0], parts[1], parts[2]);
}

// ---- Password Strength Calculator (static) ----
int User::passwordStrength(const QString& pw) {
    int score = 0;

    if (pw.length() >= 8) ++score;

    bool hasUpper = false;
    for (QChar c : pw) if (c.isUpper()) { hasUpper = true; break; }
    if (hasUpper) ++score;

    bool hasDigit = false;
    for (QChar c : pw) if (c.isDigit()) { hasDigit = true; break; }
    if (hasDigit) ++score;

    const QString specials = "!@#$%^&*()_+-=[]{}";
    bool hasSpecial = false;
    for (QChar c : pw) if (specials.contains(c)) { hasSpecial = true; break; }
    if (hasSpecial) ++score;

    return score;
}