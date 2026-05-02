#include "Models/User.h"//header file for the User class, which defines the user data structure and operations, including authentication and password strength calculation
#include <QStringList>  //for splitting strings when reconstructing a User object from a line in the users.txt file
#include <QChar>        //for checking character types when calculating password strength (e.g., uppercase letters, digits, special characters)

//constructor
User::User(const QString& username, const QString& password, const QString& role)
    : username(username), password(password), role(role)
{
}

//getters
QString User::getUsername() const { return username; }
QString User::getRole() const { return role; }

//aunthentication method that checks if the input password matches the stored password for the user. In a real application, the password would be hashed and salted for security, but for simplicity, this example uses plain text comparison. The method returns true if the passwords match, indicating successful authentication, and false otherwise.
bool User::authenticate(const QString& inputPassword) const {
    return password == inputPassword;
}

//serialize the user data into a string format suitable for saving to the users.txt file. The format used is "username,password,role", which allows for easy reconstruction of the User object later. This method is used when saving user data to the file, ensuring that all necessary information is stored in a consistent and parseable format.
QString User::toFileString() const {
    return username + "," + password + "," + role;
}

//static method to reconstruct a User object from a line in the users.txt file. The method splits the input string by commas to extract the username, password, and role. If the line does not contain at least three parts, it returns a default User object with placeholder values. This method is used when loading user data from the file, allowing for easy conversion from the stored string format back into a User object.
User User::fromFileString(const QString& line) {
    QStringList parts = line.split(',');
    if (parts.size() < 3)
        return User("unknown", "1234", "Patient");
    return User(parts[0], parts[1], parts[2]);
}

//static method to calculate the strength of a password based on certain criteria. The method checks for the length of the password, the presence of uppercase letters, digits, and special characters. Each criterion contributes to the overall strength score, which ranges from 0 to 4. This method is used in the login form to provide feedback to the user about how strong their password is as they type it in.
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