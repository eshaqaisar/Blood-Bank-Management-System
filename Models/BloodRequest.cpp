#include "BloodRequest.h"//header file for the BloodRequest class, which defines the data structure and operations for blood requests, including serialization to and from file strings, and saving to a file
#include <QFile> //for reading and writing request data from/to the file
#include <QTextStream>//for parsing request data from the file and writing approval certificates
//constructor

BloodRequest::BloodRequest(const QString& requestID,
                           const QString& patientName,
                           const QString& hospitalName,
                           const QString& bloodGroup,
                            int units )
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
QString BloodRequest::toFileString() const {
    return requestID + "," +
        patientName + "," +
        hospitalName + "," +
        bloodGroup + "," +
        QString::number(units) + "," +
        status + "," +
        requestDate.toString("yyyy-MM-dd");
}

BloodRequest BloodRequest::fromFileString(const QString& line) {
    QStringList p = line.split(',');
    if (p.size() < 5)
        return BloodRequest();
    BloodRequest req(p[0].trimmed(), p[1].trimmed(),
        p[2].trimmed(), p[3].trimmed(),
        p[4].trimmed().toInt());
    if (p.size() >= 6)
    {
        QString s = p[5].trimmed();
        if (s == "Approved") req.approve();
        else if (s == "Rejected") req.reject();
    }
    if (p.size() >= 7)
        req.requestDate = QDate::fromString(p[6].trimmed(), "yyyy-MM-dd");
    return req;
}
QDate BloodRequest::getRequestDate() const {
    return requestDate; // make sure this variable exists
}

// file handling

void BloodRequest::saveToFile() {
    QFile file("Database/requests.txt");
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << toFileString() << "\n";
        file.close();
    }
}