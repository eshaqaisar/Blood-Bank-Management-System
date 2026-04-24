#include "FileManager.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDir>

// ============================================================
// FileManager.cpp  |  Domain 4: Esha Qaisar
// ============================================================

// ---- File Path Constants ----
// All text files are stored in the "Database/" folder.
// If you rename/move this folder, only change it here.
const QString FileManager::USERS_FILE = "Database/users.txt";
const QString FileManager::DONORS_FILE = "Database/donors.txt";
const QString FileManager::PATIENTS_FILE = "Database/patients.txt";
const QString FileManager::INVENTORY_FILE = "Database/inventory.txt";
const QString FileManager::REQUESTS_FILE = "Database/requests.txt";
const QString FileManager::LOGS_FILE = "Database/logs.txt";

// ============================================================
// PRIVATE HELPER METHODS
// ============================================================

// Read all non-empty lines from a file into a QStringList
QStringList FileManager::readAllLines(const QString& filePath) {
    QStringList lines;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // File might not exist yet — that's OK, just return empty list
        return lines;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) lines.append(line);
    }
    file.close();
    return lines;
}

// Overwrite a file with all lines from the list
bool FileManager::writeAllLines(const QString& filePath, const QStringList& lines) {
    // Make sure the Database folder exists
    QDir().mkpath(QFileInfo(filePath).absolutePath());

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "ERROR: Cannot write to file:" << filePath;
        return false;
    }
    QTextStream out(&file);
    for (const QString& line : lines) {
        out << line << "\n";
    }
    file.close();
    return true;
}

// Append a single line to the end of a file (faster than rewriting all)
bool FileManager::appendLine(const QString& filePath, const QString& line) {
    QDir().mkpath(QFileInfo(filePath).absolutePath());
    QFile file(filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "ERROR: Cannot append to file:" << filePath;
        return false;
    }
    QTextStream out(&file);
    out << line << "\n";
    file.close();
    return true;
}

// ============================================================
// USER FILE OPERATIONS
// ============================================================

void FileManager::saveUser(const User& user) {
    appendLine(USERS_FILE, user.toFileString());
    logActivity("New user registered: " + user.getUsername() + " [" + user.getRole() + "]");
}

void FileManager::saveAllUsers(const QList<User>& users) {
    QStringList lines;
    for (const User& u : users) lines.append(u.toFileString());
    writeAllLines(USERS_FILE, lines);
}

QList<User> FileManager::loadUsers() {
    QList<User> users;
    for (const QString& line : readAllLines(USERS_FILE)) {
        users.append(User::fromFileString(line));
    }
    return users;
}

// Check if a username already exists in users.txt
bool FileManager::userExists(const QString& username) {
    for (const User& u : loadUsers()) {
        if (u.getUsername() == username) return true;
    }
    return false;
}

// Find and return a specific user by username
User FileManager::findUser(const QString& username) {
    for (const User& u : loadUsers()) {
        if (u.getUsername() == username) return u;
    }
    return User("NOT_FOUND", "", ""); // Caller should check getUsername() == "NOT_FOUND"
}

// ============================================================
// DONOR FILE OPERATIONS
// ============================================================

void FileManager::saveDonor(const Donor& donor) {
    appendLine(DONORS_FILE, donor.toFileString());
    logActivity("Donor added: " + donor.getName() + " (" + donor.getBloodGroup() + ")");
}

void FileManager::saveAllDonors(const QList<Donor>& donors) {
    QStringList lines;
    for (const Donor& d : donors) lines.append(d.toFileString());
    writeAllLines(DONORS_FILE, lines);
}

QList<Donor> FileManager::loadDonors() {
    QList<Donor> donors;
    for (const QString& line : readAllLines(DONORS_FILE)) {
        donors.append(Donor::fromFileString(line));
    }
    return donors;
}

void FileManager::deleteDonor(const QString& name) {
    QList<Donor> donors = loadDonors();
    // Remove the donor whose name matches
    donors.removeIf([&name](const Donor& d) { return d.getName() == name; });
    saveAllDonors(donors); // Rewrite file without the deleted donor
    logActivity("Donor deleted: " + name);
}

// ============================================================
// PATIENT FILE OPERATIONS
// ============================================================

void FileManager::savePatient(const Patient& patient) {
    appendLine(PATIENTS_FILE, patient.toFileString());
    logActivity("Patient added: " + patient.getName());
}

QList<Patient> FileManager::loadPatients() {
    QList<Patient> patients;
    for (const QString& line : readAllLines(PATIENTS_FILE)) {
        patients.append(Patient::fromFileString(line));
    }
    return patients;
}

// ============================================================
// BLOOD REQUEST FILE OPERATIONS
// ============================================================

void FileManager::saveRequest(const BloodRequest& req) {
    appendLine(REQUESTS_FILE, req.toFileString());
    logActivity("Blood request submitted: " + req.getRequestId() +
        " | " + req.getRequiredBloodGroup() + " x" +
        QString::number(req.getUnitsRequired()));
}

void FileManager::saveAllRequests(const QList<BloodRequest>& requests) {
    QStringList lines;
    for (const BloodRequest& r : requests) lines.append(r.toFileString());
    writeAllLines(REQUESTS_FILE, lines);
}

QList<BloodRequest> FileManager::loadRequests() {
    QList<BloodRequest> requests;
    for (const QString& line : readAllLines(REQUESTS_FILE)) {
        requests.append(BloodRequest::fromFileString(line));
    }
    return requests;
}

// Update just the status of one request (without rewriting unrelated data)
void FileManager::updateRequestStatus(const QString& requestId, const QString& status) {
    QList<BloodRequest> requests = loadRequests();
    for (BloodRequest& req : requests) {
        if (req.getRequestId() == requestId) {
            if (status == "Approved") req.approve();
            else req.reject();
            break;
        }
    }
    saveAllRequests(requests); // Rewrite the entire file with updated status
    logActivity("Request " + requestId + " status changed to: " + status);
}

// ============================================================
// ACTIVITY LOGGING
// ============================================================

// Every important event is saved to logs.txt with timestamp.
// This creates an audit trail of all system activity.
void FileManager::logActivity(const QString& activity) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
    appendLine(LOGS_FILE, timestamp + " " + activity);
}

// ============================================================
// CSV EXPORT (One-Click Excel Export)
// ============================================================

bool FileManager::exportDonorsToCSV(const QString& outputFilePath) {
    QList<Donor> donors = loadDonors();
    QFile file(outputFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    // Write CSV header row
    out << "Name,Age,Contact,City,Blood Group,Weight (kg),Last Donation,Eligible\n";

    // Write one row per donor
    for (const Donor& d : donors) {
        out << d.getName() << ","
            << d.getAge() << ","
            << d.getContact() << ","
            << d.getCity() << ","
            << d.getBloodGroup() << ","
            << d.getWeight() << ","
            << d.getLastDonationDate().toString("yyyy-MM-dd") << ","
            << (d.isEligible() ? "Yes" : "No") << "\n";
    }
    file.close();
    logActivity("CSV exported: Donor list → " + outputFilePath);
    return true;
}

bool FileManager::exportInventoryToCSV(const QString& outputFilePath,
    const QMap<QString, int>& inventory) {
    QFile file(outputFilePath);
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

// ============================================================
// CERTIFICATE / RECEIPT GENERATION (Domain 3: Noor Fatima)
// ============================================================

// Generates a formatted text receipt for an approved blood request.
// Saved as a .txt file named after the request ID.
bool FileManager::generateCertificate(const BloodRequest& req, const QString& outputPath) {
    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << "================================================\n";
    out << "   BLOOD BANK SYSTEM - DONATION CERTIFICATE     \n";
    out << "================================================\n\n";
    out << "Request ID   : " << req.getRequestId() << "\n";
    out << "Patient Name : " << req.getPatientName() << "\n";
    out << "Blood Group  : " << req.getRequiredBloodGroup() << "\n";
    out << "Units Given  : " << req.getUnitsRequired() << "\n";
    out << "Hospital     : " << req.getHospitalName() << "\n";
    out << "Date         : " << req.getRequestDate().toString("dd-MM-yyyy") << "\n";
    out << "Status       : " << req.getStatus() << "\n\n";
    out << "================================================\n";
    out << "  This certificate confirms the above transfusion.\n";
    out << "================================================\n";

    file.close();
    logActivity("Certificate generated for: " + req.getRequestId());
    return true;
}