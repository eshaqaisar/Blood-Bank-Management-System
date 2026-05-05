#include "BloodRequest.h"//header file for the BloodRequest class, which defines the data structure and operations for blood requests, including serialization to and from file strings, and saving to a file
#include <QFile>//for reading and writing request data from/to the file
#include <QTextStream>//for parsing request data from the file and writing approval certificates
#include <QString>//used only at Qt file I/O boundary via QString::fromStdString / .toStdString()
#include <string>//for std::string, std::stoi, std::to_string used throughout

//constructor
BloodRequest::BloodRequest(const std::string& requestID,
    const std::string& patientName,
    const std::string& hospitalName,
    const std::string& bloodGroup,
    int units)
    : requestID(requestID),
    patientName(patientName),
    hospitalName(hospitalName),
    bloodGroup(bloodGroup),
    units(units),
    status("Pending"),          //FIX: was never initialized → undefined status
    requestDate(QDate::currentDate())
{
}

//serialize: requestID,patientName,hospitalName,bloodGroup,units,status,date
// std::to_string replaces QString::number; QDate::toString converted at boundary
std::string BloodRequest::toFileString() const {
    return requestID + "," +
        patientName + "," +
        hospitalName + "," +
        bloodGroup + "," +
        std::to_string(units) + "," +
        status + "," +
        requestDate.toString("yyyy-MM-dd").toStdString();
}

//deserialize from a line in requests.txt
// manual comma split replaces QStringList / line.split(',')
BloodRequest BloodRequest::fromFileString(const std::string& line) {
    std::string parts[10];
    int count = 0;
    std::string token;
    for (char ch : line) {
        if (ch == ',' && count < 9) {
            parts[count++] = token;
            token.clear();
        }
        else {
            token += ch;
        }
    }
    parts[count++] = token; //last field

    if (count < 5)
        return BloodRequest();

    BloodRequest req(parts[0], parts[1], parts[2], parts[3], std::stoi(parts[4]));

    if (count >= 6) {
        std::string s = parts[5];
        if (s == "Approved") req.approve();
        else if (s == "Rejected") req.reject();
    }
    if (count >= 7) {
        //QDate::fromString needs QString; convert std::string at the Qt boundary
        req.requestDate = QDate::fromString(QString::fromStdString(parts[6]), "yyyy-MM-dd");
    }
    return req;
}

//file handling: append this request to requests.txt
void BloodRequest::saveToFile() {
    QFile file("Database/requests.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::fromStdString(toFileString()) << "\n";
        file.close();
    }
}