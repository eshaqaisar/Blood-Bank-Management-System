#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>//for handling file paths and string data
#include <QList>//for using QList to manage collections of users, donors, patients, etc.
#include <QStringList>//for reading and writing lines of text from/to files
#include "../Models/Donor.h"//for the Donor data structure and operations
#include "../Models/Patient.h"//for the Patient data structure and operations
#include "../Models/User.h"//for the User data structure and operations, including password strength calculation and user role management
#include "../Models/BloodBag.h"//for the BloodBag data structure and operations, including managing blood inventory and updating stock levels
#include "../Models/BloodRequest.h"//for the BloodRequest data structure and operations, including managing blood requests and updating their status

class FileManager {
public:
	//file paths for storing data, defined as static constants for easy access throughout the application. These paths point to text files in the "Database" directory where user, donor, patient, inventory, request, and log data are stored in a simple line-based format.
    static const QString USERS_FILE;
    static const QString DONORS_FILE;
    static const QString PATIENTS_FILE;
    static const QString INVENTORY_FILE;
    static const QString REQUESTS_FILE;
    static const QString LOGS_FILE;

	//user file operations: methods for saving, loading, and managing user data. These methods allow the application to persist user information in the "users.txt" file, check for duplicate usernames, and find users by their username. The saveUser method appends a single user to the file, while saveAllUsers overwrites the file with a full list of users. The loadUsers method reads all users from the file and returns them as a QList<User>.
    static void       saveUser(const User& user);         // Append one user to file
    static void       saveAllUsers(const QList<User>& users); // Overwrite with full list
    static QList<User> loadUsers();                        // Read all users
    static bool       userExists(const QString& username); // Check for duplicate
    static User       findUser(const QString& username);   // Find by username

	//donor file operations: methods for saving, loading, and managing donor data. These methods allow the application to persist donor information in the "donors.txt" file, including adding new donors, loading all donors into a list, and deleting donors by name. The saveDonor method appends a single donor to the file, while saveAllDonors overwrites the file with a full list of donors. The loadDonors method reads all donors from the file and returns them as a QList<Donor>. The deleteDonor method removes a donor from the file based on their name, which is used when a donor is deleted from the system.
    static void        saveDonor(const Donor& donor);
    static void        saveAllDonors(const QList<Donor>& donors);
    static QList<Donor> loadDonors();
    static void        deleteDonor(const QString& name);  // Remove by name

	//patient file operations: methods for saving, loading, and managing patient data. These methods allow the application to persist patient information in the "patients.txt" file, including adding new patients and loading all patients into a list. The savePatient method appends a single patient to the file, while saveAllPatients overwrites the file with a full list of patients. The loadPatients method reads all patients from the file and returns them as a QList<Patient>.
    static void          savePatient(const Patient& patient);
    static QList<Patient> loadPatients();
	//no deletePatient method is provided since patients are not deleted in the current application flow
    static void              saveRequest(const BloodRequest& req);
    static void              saveAllRequests(const QList<BloodRequest>& requests);
    static QList<BloodRequest> loadRequests();
    static void              updateRequestStatus(const QString& requestId, const QString& status);

   
	//activity logging: method for appending activity logs to the "logs.txt" file. This method allows the application to record important events and actions taken by users, such as logins, donor additions, request approvals, etc. Each log entry is appended as a new line in the log file, which can be used for auditing and monitoring purposes.
    static void logActivity(const QString& activity);

    //CSV Export for Admin 
    static bool exportDonorsToCSV(const QString& outputFilePath);
    static bool exportInventoryToCSV(const QString& outputFilePath, const QMap<QString, int>& inventory);

	//certificate generation for approved requests: method for generating a text-based certificate for approved blood requests. This method takes a BloodRequest object and an output file path, and creates a certificate that includes the patient's name, blood group, units approved, date and time of approval, and a status indicating that the request has been approved. The certificate is saved as a text file in the specified output path.
    static bool generateCertificate(const BloodRequest& req, const QString& outputPath);

private:
	//utility methods for reading and writing lines of text from/to files. These methods provide a simple interface for file operations, allowing the application to read all lines from a file into a QStringList or write a list of lines to a file. The readAllLines method reads the entire contents of a file and returns it as a list of strings, while the writeAllLines method takes a list of strings and writes them to a file, overwriting any existing content. The appendLine method allows for adding a single line to the end of a file without overwriting the existing content.
    static QStringList readAllLines(const QString& filePath);
    static bool        writeAllLines(const QString& filePath, const QStringList& lines);
    static bool        appendLine(const QString& filePath, const QString& line);
};

#endif // FILEMANAGER_H
