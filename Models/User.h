#pragma once

#include <string>//std::string replaces QString for storing user information such as username, password, and role

class User {
private:
    std::string username;  //unique login ID
    std::string password;  //stored as plain text here (real apps use hashing)
    std::string role;      //"Admin", "Donor", or "Patient"

public:
    //constructor
    User(const std::string& username, const std::string& password, const std::string& role);

    //getters 
    std::string getUsername() const;
    std::string getRole() const;
    // NOTE: no getPassword() — passwords should NOT be accessible from outside!

    //authentication method that checks if the input password matches the stored password for this user. This method is used during the login process to verify that the user has entered the correct password without exposing the actual stored password to the caller. The method returns true if the input password matches the stored password, and false otherwise, allowing for secure authentication while keeping sensitive information protected.
    //returns true only if the given password matches the stored one.
    //caller never sees the actual stored password.
    bool authenticate(const std::string& inputPassword) const;

    //file I/O methods for saving/loading user data to/from a file. The toFileString method formats the user's information as a single line string suitable for writing to a file, while the fromFileString static method parses a line from the file and creates a User object based on that data. This allows for easy storage and retrieval of user information in a simple text file format, facilitating user management in the application.
    std::string toFileString() const;
    static User fromFileString(const std::string& line);

    //static helper: generate a simple password strength score (0-4)
    // 0 = very weak, 4 = strong
    //used by SignupForm to drive the QProgressBar
    static int passwordStrength(const std::string& password);
};