#include "FileManager.h"//header file for the FileManager class, which provides static methods for reading and writing data to text files, as well as logging activities in the system.
#include <QFile>//for reading and writing files in the file system
#include <QTextStream>//for parsing text data from files and writing text data to files in a structured way
#include <QDateTime>//for adding timestamps to log entries when logging activities in the system
#include <QDebug>//for printing debug messages to the console, especially when file operations fail
#include <QDir>//for creating directories if they do not exist when trying to write files
#include <QFileInfo>//for checking file existence and properties
#include <QString>//used ONLY at Qt file I/O boundary via fromStdString / toStdString
#include <string>//for std::string, std::to_string used throughout

//define the file paths as std::string constants
//std::string replaces the original const QString constants
const std::string FileManager::USERS_FILE = "Database/users.txt";
const std::string FileManager::DONORS_FILE = "Database/donors.txt";
const std::string FileManager::PATIENTS_FILE = "Database/patients.txt";
const std::string FileManager::INVENTORY_FILE = "Database/inventory.txt";
const std::string FileManager::REQUESTS_FILE = "Database/requests.txt";
const std::string FileManager::LOGS_FILE = "Database/logs.txt";

// ─────────────────────────────────────────────────────────────────────────────
// PRIVATE UTILITY METHODS
// ─────────────────────────────────────────────────────────────────────────────

//readAllLines: reads every non-empty line from the file and returns them as a QList<std::string>.
//QList is kept here because it integrates naturally with the Qt for-range loops used below.
//std::string replaces QString for the file path parameter and the line storage type.
QList<std::string> FileManager::readAllLines(const std::string& filePath) {
    QList<std::string> lines;
    //convert std::string path to QString only at the QFile API boundary
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) //if the file cannot be opened for reading (e.g., it does not exist), print a debug message and return an empty list. This allows the application to handle missing files gracefully without crashing, while also providing information in the debug output to help identify issues with file access.
    {
        return lines;
    }
    QTextStream in(&file);
    while (!in.atEnd()) //read each line from the file until the end is reached
    {
        QString qline = in.readLine().trimmed();
        if (!qline.isEmpty())
            //convert each line from QString to std::string before storing
            lines.append(qline.toStdString());
    }
    file.close();
    return lines;
}

//write a list of std::string lines to a file, overwriting any existing content. This method ensures that the directory for the file exists before attempting to write, preventing errors due to missing directories.
bool FileManager::writeAllLines(const std::string& filePath, const QList<std::string>& lines) {
    //ensure the directory exists before writing to prevent errors
    QDir().mkpath(QFileInfo(QString::fromStdString(filePath)).absolutePath());

    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) //if the file cannot be opened for writing, print a debug message and return false
    {
        qDebug() << "ERROR: Cannot write to file:" << QString::fromStdString(filePath);
        return false;
    }
    QTextStream out(&file);
    for (const std::string& line : lines) //write each std::string line to the file with a newline character
    {
        out << QString::fromStdString(line) << "\n";
    }
    file.close();
    return true;
}

//append a single std::string line to the end of a file.
bool FileManager::appendLine(const std::string& filePath, const std::string& line) {
    QDir().mkpath(QFileInfo(QString::fromStdString(filePath)).absolutePath());
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::Append | QIODevice::Text)) //if the file cannot be opened for appending, print a debug message and return false
    {
        qDebug() << "ERROR: Cannot append to file:" << QString::fromStdString(filePath);
        return false;
    }
    QTextStream out(&file);
    out << QString::fromStdString(line) << "\n";
    file.close();
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// USER FILE OPERATIONS
// ─────────────────────────────────────────────────────────────────────────────

void FileManager::saveUser(const User& user) {
    appendLine(USERS_FILE, user.toFileString());
    logActivity("New user registered: " + user.getUsername() + " [" + user.getRole() + "]");
}

//save a list of users to the users file, overwriting any existing user data.
void FileManager::saveAllUsers(const QList<User>& users) {
    QList<std::string> lines;
    for (const User& u : users) lines.append(u.toFileString());
    writeAllLines(USERS_FILE, lines);
}

//load all users from the users file and return them as a QList<User>.
QList<User> FileManager::loadUsers() {
    QList<User> users;
    for (const std::string& line : readAllLines(USERS_FILE)) {
        users.append(User::fromFileString(line));
    }
    return users;
}

//check if a user with the given username exists in the users file.
bool FileManager::userExists(const std::string& username) {
    for (const User& u : loadUsers()) {
        if (u.getUsername() == username) return true;
    }
    return false;
}

//find and return a User object for the given username.
//caller should check getUsername() == "NOT_FOUND" to detect failure.
User FileManager::findUser(const std::string& username) {
    for (const User& u : loadUsers()) {
        if (u.getUsername() == username) return u;
    }
    return User("NOT_FOUND", "", ""); //caller should check getUsername() == "NOT_FOUND"
}

// ─────────────────────────────────────────────────────────────────────────────
// DONOR FILE OPERATIONS
// ─────────────────────────────────────────────────────────────────────────────

void FileManager::saveDonor(const Donor& donor) {
    appendLine(DONORS_FILE, donor.toFileString());
    logActivity("Donor added: " + donor.getName() + " (" + donor.getBloodGroup() + ")");
}

//save a list of donors to the donors file, overwriting any existing donor data.
void FileManager::saveAllDonors(const QList<Donor>& donors) {
    QList<std::string> lines;
    for (const Donor& d : donors) lines.append(d.toFileString());
    writeAllLines(DONORS_FILE, lines);
}

//load all donors from the donors file and return them as a QList<Donor>.
QList<Donor> FileManager::loadDonors() {
    QList<Donor> donors;
    for (const std::string& line : readAllLines(DONORS_FILE)) {
        donors.append(Donor::fromFileString(line));
    }
    return donors;
}

//delete a donor by name from the donors file.
//FIX: replaced removeIf lambda (advanced STL) with a basic loop to stay within taught concepts
void FileManager::deleteDonor(const std::string& name) {
    QList<Donor> donors = loadDonors();
    QList<Donor> kept;
    for (const Donor& d : donors) {
        //keep all donors whose name does NOT match the one to delete
        if (d.getName() != name) kept.append(d);
    }
    saveAllDonors(kept); //rewrite file without the deleted donor
    logActivity("Donor deleted: " + name);
}

// ─────────────────────────────────────────────────────────────────────────────
// PATIENT FILE OPERATIONS
// ─────────────────────────────────────────────────────────────────────────────

void FileManager::savePatient(const Patient& patient) {
    appendLine(PATIENTS_FILE, patient.toFileString());
    logActivity("Patient added: " + patient.getName());
}

//load all patients from the patients file and return them as a QList<Patient>.
QList<Patient> FileManager::loadPatients() {
    QList<Patient> patients;
    for (const std::string& line : readAllLines(PATIENTS_FILE)) //read each line from the patients file
    {
        patients.append(Patient::fromFileString(line));
    }
    return patients;
}

// ─────────────────────────────────────────────────────────────────────────────
// BLOOD REQUEST OPERATIONS
// ─────────────────────────────────────────────────────────────────────────────

void FileManager::saveRequest(const BloodRequest& req) {
    appendLine(REQUESTS_FILE, req.toFileString());
    logActivity("Blood request submitted: " + req.getRequestId() +
        " | " + req.getRequiredBloodGroup() +
        " x" + std::to_string(req.getUnitsRequired()));
}

//save a list of blood requests to the requests file, overwriting any existing request data.
void FileManager::saveAllRequests(const QList<BloodRequest>& requests) {
    QList<std::string> lines;
    for (const BloodRequest& r : requests) lines.append(r.toFileString());
    writeAllLines(REQUESTS_FILE, lines);
}

//load all blood requests from the requests file and return them as a QList<BloodRequest>.
QList<BloodRequest> FileManager::loadRequests() {
    QList<BloodRequest> requests;
    for (const std::string& line : readAllLines(REQUESTS_FILE)) {
        requests.append(BloodRequest::fromFileString(line));
    }
    return requests;
}

//updateRequestStatus: FIXED approve-with-no-stock bug.
//Previously the UI showed "insufficient stock" but still wrote Approved to file.
//Now: if status == "Approved", we first check inventory via useBag().
//useBag() returns false when stock is insufficient, and we return false to the caller
//without writing anything — so the request stays Pending and no stock is deducted.
bool FileManager::updateRequestStatus(const std::string& requestId,
    const std::string& status,
    BloodInventory& inventory) {
    QList<BloodRequest> requests = loadRequests();
    bool found = false;

    for (BloodRequest& req : requests) {
        if (req.getRequestId() == requestId) {
            found = true;

            if (status == "Approved") {
                //attempt to deduct from inventory BEFORE changing status
                //useBag() returns false if there is not enough stock
                bool deducted = inventory.useBag(req.getRequiredBloodGroup(),
                    req.getUnitsRequired());
                if (!deducted) {
                    //not enough blood available — do NOT approve, return false to caller
                    logActivity("Approval FAILED (insufficient stock) for request: " + requestId);
                    return false; //caller must show error; status stays Pending
                }
                //stock successfully deducted — now mark as approved
                req.approve();
                //persist the updated inventory to disk
                inventory.save(INVENTORY_FILE);
                logActivity("Request " + requestId + " APPROVED. Inventory updated.");

            }
            else {
                //rejecting does not touch inventory
                req.reject();
                logActivity("Request " + requestId + " REJECTED.");
            }
            break;
        }
    }

    if (!found) {
        logActivity("updateRequestStatus: request ID not found: " + requestId);
        return false;
    }

    saveAllRequests(requests); //rewrite the entire file with updated status
    return true;
}

//filterRequestsByBloodGroup: FIX for admin blood-group filter crash.
//The original code passed a QComboBox-selected QString directly into a comparison
//that could be empty or mismatched, causing undefined behaviour.
//This method safely returns all requests whose bloodGroup matches the filter.
//Passing an empty string ("") disables the filter and returns everything.
QList<BloodRequest> FileManager::filterRequestsByBloodGroup(const std::string& bloodGroup) {
    QList<BloodRequest> all = loadRequests();
    //if no filter provided, return all requests without crashing
    if (bloodGroup.empty()) return all;

    QList<BloodRequest> filtered;
    for (const BloodRequest& req : all) {
        //safe std::string comparison — no Qt QString involved here
        if (req.getRequiredBloodGroup() == bloodGroup) {
            filtered.append(req);
        }
    }
    return filtered;
}

// ─────────────────────────────────────────────────────────────────────────────
// ACTIVITY LOGGING
// ─────────────────────────────────────────────────────────────────────────────

//append a log entry with a timestamp to the logs file.
void FileManager::logActivity(const std::string& activity) {
    //QDateTime is a Qt type; convert its output to std::string at the boundary
    std::string timestamp = QDateTime::currentDateTime()
        .toString("[yyyy-MM-dd HH:mm:ss]")
        .toStdString();
    appendLine(LOGS_FILE, timestamp + " " + activity);
}

// ─────────────────────────────────────────────────────────────────────────────
// CSV EXPORT
// ─────────────────────────────────────────────────────────────────────────────

//export the list of donors to a CSV file at the specified output file path.
bool FileManager::exportDonorsToCSV(const std::string& outputFilePath) {
    QList<Donor> donors = loadDonors();
    QFile file(QString::fromStdString(outputFilePath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    //write the header row for the CSV file
    out << "Name,Age,Contact,City,Blood Group,Weight (kg),Last Donation,Eligible\n";

    //write each donor's information as a separate line in the CSV file
    for (const Donor& d : donors) {
        out << QString::fromStdString(d.getName()) << ","
            << d.getAge() << ","
            << QString::fromStdString(d.getContact()) << ","
            << QString::fromStdString(d.getCity()) << ","
            << QString::fromStdString(d.getBloodGroup()) << ","
            << d.getWeight() << ","
            << d.getLastDonationDate().toString("yyyy-MM-dd") << ","
            << (d.isEligible() ? "Yes" : "No") << "\n";
    }
    file.close();
    logActivity("CSV exported: Donor list → " + outputFilePath);
    return true;
}

//export the blood inventory data to a CSV file.
bool FileManager::exportInventoryToCSV(const std::string& outputFilePath,
    const QMap<QString, int>& inventory) {
    QFile file(QString::fromStdString(outputFilePath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << "Blood Group,Available Units\n";
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        out << it.key() << "," << it.value() << "\n";
    }
    file.close();
    logActivity("CSV exported: Inventory → " + outputFilePath);
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// CERTIFICATE GENERATION
// ─────────────────────────────────────────────────────────────────────────────

//generate a simple text certificate for an approved blood request.
bool FileManager::generateCertificate(const BloodRequest& req, const std::string& outputPath) {
    QFile file(QString::fromStdString(outputPath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << "================================================\n";
    out << "   BLOOD BANK SYSTEM - DONATION CERTIFICATE     \n";
    out << "================================================\n\n";
    out << "Request ID   : " << QString::fromStdString(req.getRequestId()) << "\n";
    out << "Patient Name : " << QString::fromStdString(req.getPatientName()) << "\n";
    out << "Blood Group  : " << QString::fromStdString(req.getRequiredBloodGroup()) << "\n";
    out << "Units Given  : " << req.getUnitsRequired() << "\n";
    out << "Hospital     : " << QString::fromStdString(req.getHospitalName()) << "\n";
    out << "Date         : " << req.getRequestDate().toString("dd-MM-yyyy") << "\n";
    out << "Status       : " << QString::fromStdString(req.getStatus()) << "\n\n";
    out << "================================================\n";
    out << "  This certificate confirms the above transfusion.\n";
    out << "================================================\n";

    file.close();
    logActivity("Certificate generated for: " + req.getRequestId());
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// REFRESH ALL — call after any write to reload every list from disk
// ─────────────────────────────────────────────────────────────────────────────

//refreshAll: reloads users, donors, patients, requests, and inventory from disk.
//The UI should call this after any add/edit/delete/approve operation so that
//tables and views show up-to-date data without restarting the application.
bool FileManager::refreshAll(QList<User>& users,
    QList<Donor>& donors,
    QList<Patient>& patients,
    QList<BloodRequest>& requests,
    BloodInventory& inventory) {
    users = loadUsers();
    donors = loadDonors();
    patients = loadPatients();
    requests = loadRequests();
    inventory.load(INVENTORY_FILE);       //BloodInventory::load clears and reloads bags
    inventory.removeExpiredBags();        //always clean expired bags on refresh
    logActivity("Full data refresh performed.");
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// SEED DEMO DATA — populates all files with realistic starting records
// ─────────────────────────────────────────────────────────────────────────────

//seedDemoData: writes demo records to all database files.
//Checks whether each file already has content before writing, so running
//this multiple times will NOT duplicate records.
void FileManager::seedDemoData() {
    //ensure the Database directory exists
    QDir().mkpath("Database");

    // ── USERS ──────────────────────────────────────────────────────────────
    //only seed users if the file is empty (admin record might already be there)
    if (readAllLines(USERS_FILE).isEmpty()) {
        appendLine(USERS_FILE, "admin,admin123,Admin");
        appendLine(USERS_FILE, "donor1,donor123,Donor");
        appendLine(USERS_FILE, "donor2,donor456,Donor");
        appendLine(USERS_FILE, "patient1,pat123,Patient");
        appendLine(USERS_FILE, "patient2,pat456,Patient");
        logActivity("Demo users seeded.");
    }

    // ── DONORS ─────────────────────────────────────────────────────────────
    if (readAllLines(DONORS_FILE).isEmpty()) {
        //format: name,age,contact,city,bloodGroup,weight,lastDonationDate
        appendLine(DONORS_FILE, "Ali Hassan,24,03001234567,Lahore,A+,72.5,2025-01-10");
        appendLine(DONORS_FILE, "Sara Khan,30,03111234567,Karachi,O-,65.0,2024-11-20");
        appendLine(DONORS_FILE, "Usman Tariq,22,03211234567,Islamabad,B+,80.0,");
        appendLine(DONORS_FILE, "Nida Rehman,28,03311234567,Lahore,AB+,58.0,2025-02-14");
        appendLine(DONORS_FILE, "Hamza Iqbal,35,03411234567,Faisalabad,O+,90.0,2024-12-05");
        appendLine(DONORS_FILE, "Fatima Malik,26,03511234567,Multan,A-,55.0,2025-03-01");
        appendLine(DONORS_FILE, "Bilal Ahmed,29,03611234567,Rawalpindi,B-,68.0,");
        appendLine(DONORS_FILE, "Zara Siddiqui,32,03711234567,Lahore,AB-,62.0,2024-10-30");
        logActivity("Demo donors seeded.");
    }

    // ── PATIENTS ───────────────────────────────────────────────────────────
    if (readAllLines(PATIENTS_FILE).isEmpty()) {
        //format: name,age,contact,city,requiredBloodGroup,unitsRequired,hospitalName,requestStatus
        appendLine(PATIENTS_FILE, "Rana Asif,45,03021234567,Lahore,A+,2,Services Hospital,Pending");
        appendLine(PATIENTS_FILE, "Mehwish Ali,38,03121234567,Karachi,O-,1,Aga Khan Hospital,Pending");
        appendLine(PATIENTS_FILE, "Tariq Javed,60,03221234567,Islamabad,B+,3,PIMS Hospital,Pending");
        appendLine(PATIENTS_FILE, "Amina Bibi,50,03321234567,Lahore,AB+,1,Mayo Hospital,Pending");
        appendLine(PATIENTS_FILE, "Imran Shah,33,03421234567,Peshawar,O+,2,KTH Hospital,Pending");
        logActivity("Demo patients seeded.");
    }

    // ── INVENTORY ──────────────────────────────────────────────────────────
    if (readAllLines(INVENTORY_FILE).isEmpty()) {
        //format: bloodGroup,units,collectionDate,donorName
        //use recent dates so bags are not expired (within 42 days of today)
        //QDate::currentDate used via Qt; store as fixed recent dates for demo
        appendLine(INVENTORY_FILE, "A+,10,2026-04-20,Ali Hassan");
        appendLine(INVENTORY_FILE, "O-,6,2026-04-18,Sara Khan");
        appendLine(INVENTORY_FILE, "B+,8,2026-04-22,Hamza Iqbal");
        appendLine(INVENTORY_FILE, "AB+,4,2026-04-25,Nida Rehman");
        appendLine(INVENTORY_FILE, "O+,12,2026-04-15,Hamza Iqbal");
        appendLine(INVENTORY_FILE, "A-,5,2026-04-28,Fatima Malik");
        appendLine(INVENTORY_FILE, "B-,3,2026-04-30,Bilal Ahmed");
        appendLine(INVENTORY_FILE, "AB-,2,2026-05-01,Zara Siddiqui");
        logActivity("Demo inventory seeded.");
    }

    // ── REQUESTS ───────────────────────────────────────────────────────────
    if (readAllLines(REQUESTS_FILE).isEmpty()) {
        //format: requestID,patientName,hospitalName,bloodGroup,units,status,date
        appendLine(REQUESTS_FILE, "REQ001,Rana Asif,Services Hospital,A+,2,Pending,2026-05-01");
        appendLine(REQUESTS_FILE, "REQ002,Mehwish Ali,Aga Khan Hospital,O-,1,Approved,2026-04-28");
        appendLine(REQUESTS_FILE, "REQ003,Tariq Javed,PIMS Hospital,B+,3,Pending,2026-05-02");
        appendLine(REQUESTS_FILE, "REQ004,Amina Bibi,Mayo Hospital,AB+,1,Rejected,2026-04-29");
        appendLine(REQUESTS_FILE, "REQ005,Imran Shah,KTH Hospital,O+,2,Pending,2026-05-03");
        logActivity("Demo requests seeded.");
    }
}