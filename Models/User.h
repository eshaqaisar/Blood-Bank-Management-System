# pragma once

#include <QString>

// ============================================================
// User.h  |  Domain 4: Esha Qaisar (Team Lead)
// OOP Concepts: ENCAPSULATION
// ============================================================
//
// User handles AUTHENTICATION only (login credentials + role).
// It is NOT derived from Person — a User account is separate
// from the personal profile of a Donor or Patient.
//
// ENCAPSULATION:
//   The password is private and never returned directly.
//   You must call authenticate() to verify it — the password
//   itself stays hidden inside the object.
//
// ROLE-BASED ACCESS CONTROL (RBAC):
//   The role field controls what the user can do:
//     "Admin"   → full system access
//     "Donor"   → donor dashboard only
//     "Patient" → patient dashboard only
// ============================================================

class User {
private:
    QString username;  // Unique login ID
    QString password;  // Stored as plain text here (real apps use hashing)
    QString role;      // "Admin", "Donor", or "Patient"

public:
    // Constructor
    User(const QString& username, const QString& password, const QString& role);

    // ---- Getters ----
    QString getUsername() const;
    QString getRole()     const;
    // NOTE: No getPassword() — passwords should NOT be accessible from outside!

    // ---- Authentication ----
    // Returns true only if the given password matches the stored one.
    // Caller never sees the actual stored password.
    bool authenticate(const QString& inputPassword) const;

    // ---- File Handling ----
    // Format: username,password,role
    QString toFileString()         const;
    static User fromFileString(const QString& line);

    // ---- Static helper: Generate a simple password strength score (0-4) ----
    // 0 = very weak, 4 = strong
    // Used by SignupForm to drive the QProgressBar
    static int passwordStrength(const QString& password);
};

