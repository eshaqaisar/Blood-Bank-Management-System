#include "User.h"  //include the header file for the User class
#include <string>//for std::string used throughout; manual split replaces QStringList

//constructor
User::User(const std::string& username, const std::string& password, const std::string& role)
    : username(username), password(password), role(role)
{
}

//getters
std::string User::getUsername() const { return username; }
std::string User::getRole()     const { return role; }

//authentication: plain string comparison (no Qt needed here)
bool User::authenticate(const std::string& inputPassword) const {
    return password == inputPassword;
}

//serialize for file saving: username,password,role
std::string User::toFileString() const {
    return username + "," + password + "," + role;
}

//reconstruct from a line in users.txt (static)
// manual comma split replaces QStringList / line.split(',')
User User::fromFileString(const std::string& line) {
    std::string parts[5];
    int count = 0;
    std::string token;
    for (char ch : line) {
        if (ch == ',' && count < 4) {
            parts[count++] = token;
            token.clear();
        }
        else {
            token += ch;
        }
    }
    parts[count++] = token; //last field

    if (count < 3)
        return User("unknown", "1234", "Patient");
    return User(parts[0], parts[1], parts[2]);
}

//password strength calculator (static)
// 0 = very weak, 4 = strong; checks length, uppercase, digit, special char
int User::passwordStrength(const std::string& pw) {
    int score = 0;
    if (pw.length() >= 8)
        ++score;

    bool hasUpper = false;
    for (char c : pw)
        if (c >= 'A' && c <= 'Z') { hasUpper = true; break; }
    if (hasUpper) ++score;

    bool hasDigit = false;
    for (char c : pw)
        if (c >= '0' && c <= '9') { hasDigit = true; break; }
    if (hasDigit) ++score;

    //check for special characters without using std::string::find on a set
    const std::string specials = "!@#$%^&*()_+-=[]{}";
    bool hasSpecial = false;
    for (char c : pw)
        for (char s : specials)
            if (c == s) { hasSpecial = true; break; }
    if (hasSpecial) ++score;

    return score;
}