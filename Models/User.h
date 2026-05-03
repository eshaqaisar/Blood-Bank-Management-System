# pragma once

#include <QString>//for storing user information such as username, password, and role, and for handling string operations related to user data. The QString class provides a convenient and efficient way to manage text data in the application, including methods for parsing, formatting, and manipulating strings as needed for user authentication and file handling.


class User {
private:
    QString username;  //unique login ID
    QString password;  //stored as plain text here (real apps use hashing)
    QString role;      //"Admin", "Donor", or "Patient"

public:
    //constructor
    User(const QString& username, const QString& password, const QString& role);

	//getters (no setters to keep it immutable after creation)
    QString getUsername() const;
    QString getRole()     const;
    // NOTE:no getPassword() — passwords should NOT be accessible from outside!

	//authentication method that checks if the input password matches the stored password for this user. This method is used during the login process to verify that the user has entered the correct password without exposing the actual stored password to the caller. The method returns true if the input password matches the stored password, and false otherwise, allowing for secure authentication while keeping sensitive information protected.
    //returns true only if the given password matches the stored one.
    //caller never sees the actual stored password.
    bool authenticate(const QString& inputPassword) const;

	//file I/O methods for saving/loading user data to/from a file. The toFileString method formats the user's information as a single line string suitable for writing to a file, while the fromFileString static method parses a line from the file and creates a User object based on that data. This allows for easy storage and retrieval of user information in a simple text file format, facilitating user management in the application.
    QString toFileString()         const;
    static User fromFileString(const QString& line);

    //static helper: generate a simple password strength score (0-4) 
    // 0 = very weak, 4 = strong
    //used by SignupForm to drive the QProgressBar
    static int passwordStrength(const QString& password);
};

