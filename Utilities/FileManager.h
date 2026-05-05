#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>//std::string replaces QString for all file paths and string parameters
#include <QList>//QList kept for returning collections to the Qt UI layer
#include <QMap>//QMap kept for inventory map used by Qt UI
#include <QString>//included only for QMap key type and Qt boundary conversions
#include "Donor.h"//for the Donor data structure and operations
#include "Patient.h"//for the Patient data structure and operations
#include "User.h"//for the User data structure and operations, including password strength calculation and user role management
#include "BloodBag.h"//for the BloodBag data structure and operations, including managing blood inventory and updating stock levels
#include "BloodRequest.h"//for the BloodRequest data structure and operations, including managing blood requests and updating their status

class FileManager {
public:
    //file paths for storing data, defined as static constants for easy access throughout the application.
    //std::string replaces QString for all path constants
    static const std::string USERS_FILE;
    static const std::string DONORS_FILE;
    static const std::string PATIENTS_FILE;
    static const std::string INVENTORY_FILE;
    static const std::string REQUESTS_FILE;
    static const std::string LOGS_FILE;

    //user file operations: methods for saving, loading, and managing user data.
    static void        saveUser(const User& user);
    static void        saveAllUsers(const QList<User>& users);
    static QList<User> loadUsers();
    static bool        userExists(const std::string& username);
    static User        findUser(const std::string& username);

    //donor file operations: methods for saving, loading, and managing donor data.
    static void         saveDonor(const Donor& donor);
    static void         saveAllDonors(const QList<Donor>& donors);
    static QList<Donor> loadDonors();
    static void         deleteDonor(const std::string& name);

    //patient file operations: methods for saving, loading, and managing patient data.
    static void           savePatient(const Patient& patient);
    static QList<Patient> loadPatients();

    //blood request operations
    static void              saveRequest(const BloodRequest& req);
    static void              saveAllRequests(const QList<BloodRequest>& requests);
    static QList<BloodRequest> loadRequests();

    //updateRequestStatus: approves or rejects a request ONLY if sufficient inventory exists.
    //FIX: returns false (and does NOT change status) when approving with insufficient stock.
    //     Previously the UI showed "no stock" but still wrote Approved to file.
    static bool updateRequestStatus(const std::string& requestId, const std::string& status,
        BloodInventory& inventory);

    //filter requests by blood group — FIX for admin crash:
    //returns only requests matching the given blood group string.
    //passing "" returns all requests (no filter).
    static QList<BloodRequest> filterRequestsByBloodGroup(const std::string& bloodGroup);

    //activity logging
    static void logActivity(const std::string& activity);

    //CSV Export for Admin
    static bool exportDonorsToCSV(const std::string& outputFilePath);
    static bool exportInventoryToCSV(const std::string& outputFilePath, const QMap<QString, int>& inventory);

    //certificate generation for approved requests
    static bool generateCertificate(const BloodRequest& req, const std::string& outputPath);

    //refreshAll: reloads all data from disk and returns true on success.
    //Call this after any write operation to ensure the UI sees fresh data.
    static bool refreshAll(QList<User>& users, QList<Donor>& donors,
        QList<Patient>& patients, QList<BloodRequest>& requests,
        BloodInventory& inventory);

    //seedDemoData: populates the database files with realistic demo records.
    //Safe to call at first launch; skips seeding if data files already have content.
    static void seedDemoData();

private:
    //utility methods for reading and writing lines of text from/to files.
    //std::string replaces QString for file paths
    static QList<std::string> readAllLines(const std::string& filePath);
    static bool               writeAllLines(const std::string& filePath, const QList<std::string>& lines);
    static bool               appendLine(const std::string& filePath, const std::string& line);
};

#endif // FILEMANAGER_H