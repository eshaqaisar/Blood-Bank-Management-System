#include "FileManager.h"//header file for the FileManager class, which provides static methods for reading and writing data to text files, as well as logging activities in the system. The FileManager class serves as a utility for managing file operations related to users, donors, patients, inventory, requests, and logs in the blood bank management system.
#include <QFile>//for reading and writing files in the file system
#include <QTextStream>//for parsing text data from files and writing text data to files in a structured way
#include <QDateTime>//for adding timestamps to log entries when logging activities in the system
#include <QDebug>//for printing debug messages to the console, especially when file operations fail, to help with troubleshooting and ensuring that the file management is working correctly
#include <QDir>//for creating directories if they do not exist when trying to write files, ensuring that the file operations do not fail due to missing directories
#include <QFileInfo>  //for checking file existence and properties, which can be useful for validating file paths and ensuring that the necessary files are available for reading and writing operations in the system
//define the file paths for the various data files used in the system, such as users.txt for storing user information, donors.txt for donor data, patients.txt for patient data, inventory.txt for blood inventory data, requests.txt for blood request data, and logs.txt for activity logs. These constants are used throughout the FileManager class to ensure that all file operations are performed on the correct files in a consistent manner.
const QString FileManager::USERS_FILE = "Database/users.txt";
const QString FileManager::DONORS_FILE = "Database/donors.txt";
const QString FileManager::PATIENTS_FILE = "Database/patients.txt";
const QString FileManager::INVENTORY_FILE = "Database/inventory.txt";
const QString FileManager::REQUESTS_FILE = "Database/requests.txt";
const QString FileManager::LOGS_FILE = "Database/logs.txt";

QStringList FileManager::readAllLines(const QString& filePath) {
    QStringList lines;
    QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) //if the file cannot be opened for reading (e.g., it does not exist), print a debug message and return an empty list. This allows the application to handle missing files gracefully without crashing, while also providing information in the debug output to help identify issues with file access.
    {
       
        return lines;
    }
    QTextStream in(&file);
	while (!in.atEnd()) //read each line from the file until the end is reached
    {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) lines.append(line);
    }
    file.close();
    return lines;
}

//write a list of lines to a file, overwriting any existing content. This method ensures that the directory for the file exists before attempting to write, preventing errors due to missing directories. It opens the file in write mode, writes each line from the provided list to the file with a newline character, and then closes the file. If the file cannot be opened for writing, it prints a debug message and returns false to indicate failure; otherwise, it returns true on successful write.
bool FileManager::writeAllLines(const QString& filePath, const QStringList& lines) {
	//ensure the directory exists before writing to prevent errors
    QDir().mkpath(QFileInfo(filePath).absolutePath());

    QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))//if the file cannot be opened for writing, print a debug message and return false to indicate failure. This allows the application to handle file write errors gracefully without crashing, while also providing information in the debug output to help identify issues with file access.
    {
        qDebug() << "ERROR: Cannot write to file:" << filePath;
        return false;
    }
    QTextStream out(&file);
	for (const QString& line : lines) //write each line to the file with a newline character
    {
        out << line << "\n";
    }
    file.close();
    return true;
}

//append a single line to the end of a file. This method ensures that the directory for the file exists before attempting to write, preventing errors due to missing directories. It opens the file in append mode, writes the provided line followed by a newline character, and then closes the file. If the file cannot be opened for appending, it prints a debug message and returns false to indicate failure; otherwise, it returns true on successful append.
bool FileManager::appendLine(const QString& filePath, const QString& line) {
    QDir().mkpath(QFileInfo(filePath).absolutePath());
    QFile file(filePath);
	if (!file.open(QIODevice::Append | QIODevice::Text))//if the file cannot be opened for appending, print a debug message and return false to indicate failure. This allows the application to handle file write errors gracefully without crashing, while also providing information in the debug output to help identify issues with file access.
    {
        qDebug() << "ERROR: Cannot append to file:" << filePath;
        return false;
    }
    QTextStream out(&file);
    out << line << "\n";
    file.close();
    return true;
}
//helper method to append a log entry with a timestamp to the logs file, used for recording important activities in the system such as user registrations, donor additions, request submissions, and status changes. This method formats the log entry with the current date and time, followed by the activity description, and appends it to the logs file using the appendLine method.
void FileManager::saveUser(const User& user) {
    appendLine(USERS_FILE, user.toFileString());
    logActivity("New user registered: " + user.getUsername() + " [" + user.getRole() + "]");
}
//save a list of users to the users file, overwriting any existing user data. This method converts each User object in the provided list to its file string representation, collects these strings into a list, and then writes the entire list to the users file using the writeAllLines method. This is useful for updating the user data in bulk, such as after modifying or deleting a user.
void FileManager::saveAllUsers(const QList<User>& users) {
    QStringList lines;
    for (const User& u : users) lines.append(u.toFileString());
    writeAllLines(USERS_FILE, lines);
}
//load all users from the users file and return them as a list of User objects. This method reads all lines from the users file, converts each line from its file string representation back into a User object using the fromFileString method, and collects these User objects into a list that is returned to the caller. This allows other parts of the application to access the user data in a structured way for authentication, display, and management purposes.
QList<User> FileManager::loadUsers() {
    QList<User> users;
    for (const QString& line : readAllLines(USERS_FILE)) {
        users.append(User::fromFileString(line));
    }
    return users;
}

//check if a user with the given username exists in the users file. This method loads all users from the file and iterates through them to see if any user's username matches the provided username. If a match is found, it returns true; otherwise, it returns false after checking all users. This is useful for validating usernames during registration and login processes to ensure uniqueness and existence of user accounts.
bool FileManager::userExists(const QString& username) {
    for (const User& u : loadUsers())
    {
        if (u.getUsername() == username)
            return true;
    }
    return false;
}

//find and return a User object for the given username this method loads all users from the file and iterates through them to find a user whose username matches the provided username. If a match is found, it returns the corresponding User object; if no match is found after checking all users, it returns a User object with the username set to "NOT_FOUND" to indicate that the user does not exist. Callers of this method should check the returned User's getUsername() method to determine if the user was found or not.
User FileManager::findUser(const QString& username) {
    for (const User& u : loadUsers()) {
        if (u.getUsername() == username) return u;
    }
    return User("NOT_FOUND", "", ""); //caller should check getUsername() == "NOT_FOUND"
}

//helper method to append a log entry with a timestamp to the logs file, used for recording important activities in the system such as user registrations, donor additions, request submissions, and status changes. This method formats the log entry with the current date and time, followed by the activity description, and appends it to the logs file using the appendLine method.
void FileManager::saveDonor(const Donor& donor) {
    appendLine(DONORS_FILE, donor.toFileString());
    logActivity("Donor added: " + donor.getName() + " (" + donor.getBloodGroup() + ")");
}
//save a list of donors to the donors file, overwriting any existing donor data. This method converts each Donor object in the provided list to its file string representation, collects these strings into a list, and then writes the entire list to the donors file using the writeAllLines method. This is useful for updating the donor data in bulk, such as after modifying or deleting a donor.
void FileManager::saveAllDonors(const QList<Donor>& donors) {
    QStringList lines;
    for (const Donor& d : donors) lines.append(d.toFileString());
    writeAllLines(DONORS_FILE, lines);
}
//load all donors from the donors file and return them as a list of Donor objects. This method reads all lines from the donors file, converts each line from its file string representation back into a Donor object using the fromFileString method, and collects these Donor objects into a list that is returned to the caller. This allows other parts of the application to access the donor data in a structured way for display, management, and processing purposes.
QList<Donor> FileManager::loadDonors() {
    QList<Donor> donors;
    for (const QString& line : readAllLines(DONORS_FILE))
    {
        donors.append(Donor::fromFileString(line));
    }
    return donors;
}
//delete a donor by name from the donors file. This method loads all donors, removes the donor whose name matches the provided name, and then saves the updated list of donors back to the file. It also logs the deletion activity. Note that this method assumes that donor names are unique; if there are multiple donors with the same name, it will delete all of them. In a real application, it might be better to use a unique identifier for donors instead of names to avoid this issue.
void FileManager::deleteDonor(const QString& name) {
    QList<Donor> donors = loadDonors();
    //remove the donor whose name matches
    donors.removeIf([&name](const Donor& d) { return d.getName() == name; });
    saveAllDonors(donors); // Rewrite file without the deleted donor
    logActivity("Donor deleted: " + name);
}
//helper method to append a log entry with a timestamp to the logs file, used for recording important activities in the system such as user registrations, donor additions, request submissions, and status changes. This method formats the log entry with the current date and time, followed by the activity description, and appends it to the logs file using the appendLine method.
void FileManager::savePatient(const Patient& patient) {
    appendLine(PATIENTS_FILE, patient.toFileString());
    logActivity("Patient added: " + patient.getName());
}
//save a list of patients to the patients file, overwriting any existing patient data. This method converts each Patient object in the provided list to its file string representation, collects these strings into a list, and then writes the entire list to the patients file using the writeAllLines method. This is useful for updating the patient data in bulk, such as after modifying or deleting a patient.
QList<Patient> FileManager::loadPatients() {
    QList<Patient> patients;
	for (const QString& line : readAllLines(PATIENTS_FILE)) //read each line from the patients file, convert it from its file string representation back into a Patient object using the fromFileString method, and collect these Patient objects into a list that is returned to the caller. This allows other parts of the application to access the patient data in a structured way for display, management, and processing purposes.
    {
        patients.append(Patient::fromFileString(line));
    }
    return patients;
}
//helper method to append a log entry with a timestamp to the logs file, used for recording important activities in the system such as user registrations, donor additions, request submissions, and status changes. This method formats the log entry with the current date and time, followed by the activity description, and appends it to the logs file using the appendLine method.

void FileManager::saveRequest(const BloodRequest& req) {
    appendLine(REQUESTS_FILE, req.toFileString());
    logActivity("Blood request submitted: " + req.getRequestId() +
        " | " + req.getRequiredBloodGroup() + " x" +
        QString::number(req.getUnitsRequired()));
}
//save a list of blood requests to the requests file, overwriting any existing request data. This method converts each BloodRequest object in the provided list to its file string representation, collects these strings into a list, and then writes the entire list to the requests file using the writeAllLines method. This is useful for updating the request data in bulk, such as after modifying or deleting a request.
void FileManager::saveAllRequests(const QList<BloodRequest>& requests) {
    QStringList lines;
    for (const BloodRequest& r : requests) lines.append(r.toFileString());
    writeAllLines(REQUESTS_FILE, lines);
}
//load all blood requests from the requests file and return them as a list of BloodRequest objects. This method reads all lines from the requests file, converts each line from its file string representation back into a BloodRequest object using the fromFileString method, and collects these BloodRequest objects into a list that is returned to the caller. This allows other parts of the application to access the blood request data in a structured way for display, management, and processing purposes.
QList<BloodRequest> FileManager::loadRequests() {
    QList<BloodRequest> requests;
    for (const QString& line : readAllLines(REQUESTS_FILE)) {
        requests.append(BloodRequest::fromFileString(line));
    }
    return requests;
}

//update just the status of one request (without rewriting unrelated data)
void FileManager::updateRequestStatus(const QString& requestId, const QString& status) {
    QList<BloodRequest> requests = loadRequests();
    for (BloodRequest& req : requests) {
        if (req.getRequestId() == requestId) {
            if (status == "Approved") req.approve();
            else req.reject();
            break;
        }
    }
    saveAllRequests(requests); //rewrite the entire file with updated status
    logActivity("Request " + requestId + " status changed to: " + status);
}

//helper method to append a log entry with a timestamp to the logs file, used for recording important activities in the system such as user registrations, donor additions, request submissions, and status changes. This method formats the log entry with the current date and time, followed by the activity description, and appends it to the logs file using the appendLine method.
void FileManager::logActivity(const QString& activity) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
    appendLine(LOGS_FILE, timestamp + " " + activity);
}
//helper method to export the list of donors to a CSV file at the specified output file path. This method loads all donors, opens the specified file for writing, and writes the donor data in CSV format with a header row. Each donor's information is written as a separate line in the CSV file. If the file cannot be opened for writing, it returns false; otherwise, it returns true on successful export and logs the activity.
bool FileManager::exportDonorsToCSV(const QString& outputFilePath) {
    QList<Donor> donors = loadDonors();
    QFile file(outputFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
	//write the header row for the CSV file, which includes the column names for the donor data. This provides a clear structure for the CSV file and allows it to be easily imported into spreadsheet applications or databases that recognize the header row.
    out << "Name,Age,Contact,City,Blood Group,Weight (kg),Last Donation,Eligible\n";

	//write each donor's information as a separate line in the CSV file, with values separated by commas. The last donation date is formatted as "yyyy-MM-dd" for consistency, and the eligibility status is written as "Yes" or "No" based on the donor's eligibility. This structured format allows the CSV file to be easily read and processed by other applications.
	for (const Donor& d : donors)//iterate through the list of donors and write their information to the CSV file in a structured format
    {
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
//helper method to export the blood inventory data to a CSV file at the specified output file path. This method takes a map of blood groups and their available units, opens the specified file for writing, and writes the inventory data in CSV format with a header row. Each blood group and its corresponding available units are written as separate lines in the CSV file. If the file cannot be opened for writing, it returns false; otherwise, it returns true on successful export and logs the activity.
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
//helper method to generate a simple text-based certificate for a blood request, which includes the request details such as patient name, blood group, units given, hospital, date, and status. The certificate is saved as a text file at the specified output path. If the file cannot be opened for writing, it returns false; otherwise, it returns true on successful generation and logs the activity.
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
