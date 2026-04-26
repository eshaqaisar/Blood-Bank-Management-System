# pragma once 
#include <QString>
#include <QList>
#include <QStringList>
#include "Models/Donor.h"
#include "Models/Patient.h"
#include "Models/User.h"
#include "Models/BloodBag.h"
#include "Models/BloodRequest.h"

// ============================================================
// FileManager.h  |  Domain 4: Esha Qaisar (Team Lead)
// OOP Concepts: ABSTRACTION + ENCAPSULATION
// ============================================================
//
// FileManager is a utility class that handles ALL file I/O.
// Every other class calls FileManager instead of directly
// opening files — this keeps file logic in ONE place.
//
// ABSTRACTION:
//   Other classes don't need to know HOW files are opened,
//   formatted, or closed. They just call:
//       FileManager::saveDonor(d);
//       FileManager::loadDonors();
//
// All methods are STATIC — you don't create a FileManager object.
// FileManager is a "namespace with methods" rather than an object.
//
// FILE PATHS: All .txt files are in the "Database/" folder.
// ============================================================

class FileManager {
public:
    // ---- File Path Constants ----
    // Centralized so changing the path only requires changing it here.
    static const QString USERS_FILE;
    static const QString DONORS_FILE;
    static const QString PATIENTS_FILE;
    static const QString INVENTORY_FILE;
    static const QString REQUESTS_FILE;
    static const QString LOGS_FILE;

    // ---- User File Operations ----
    static void       saveUser(const User& user);         // Append one user to file
    static void       saveAllUsers(const QList<User>& users); // Overwrite with full list
    static QList<User> loadUsers();                        // Read all users
    static bool       userExists(const QString& username); // Check for duplicate
    static User       findUser(const QString& username);   // Find by username

    // ---- Donor File Operations ----
    static void        saveDonor(const Donor& donor);
    static void        saveAllDonors(const QList<Donor>& donors);
    static QList<Donor> loadDonors();
    static void        deleteDonor(const QString& name);  // Remove by name

    // ---- Patient File Operations ----
    static void          savePatient(const Patient& patient);
    static QList<Patient> loadPatients();

    // ---- Blood Request File Operations ----
    static void              saveRequest(const BloodRequest& req);
    static void              saveAllRequests(const QList<BloodRequest>& requests);
    static QList<BloodRequest> loadRequests();
    static void              updateRequestStatus(const QString& requestId, const QString& status);

    // ---- Activity Logging ----
    // Every important event is recorded in logs.txt with a timestamp.
    static void logActivity(const QString& activity);

    // ---- CSV Export for Admin ----
    // Generates a comma-separated file openable in Excel/Sheets.
    static bool exportDonorsToCSV(const QString& outputFilePath);
    static bool exportInventoryToCSV(const QString& outputFilePath, const QMap<QString, int>& inventory);

    // ---- Certificate / Receipt Generation ----
    // Writes a formatted text receipt for approved blood requests.
    static bool generateCertificate(const BloodRequest& req, const QString& outputPath);

private:
    // ---- Generic Helpers (private — only used internally) ----
    static QStringList readAllLines(const QString& filePath);
    static bool        writeAllLines(const QString& filePath, const QStringList& lines);
    static bool        appendLine(const QString& filePath, const QString& line);
};

