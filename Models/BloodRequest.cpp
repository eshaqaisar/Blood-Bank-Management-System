#include "BloodRequest.h"
#include <QFile> 
#include <QTextStream>




// Constructor
BloodRequest::BloodRequest(QString requestID, QString patientName,
    QString hospitalName, QString BloodGroup, int units) {

    this->requestID = requestID;
    this->patientName = patientName;
    this->hospitalName = hospitalName;
    this->BloodGroup = BloodGroup;
    this->units = units;

}

// Getters
QString BloodRequest::getRequestId() {
    return requestID;
}
QString BloodRequest::getPatientName() {
    return patientName;
}
QString BloodRequest::getHospitalName() {
    return hospitalName;
}
QString BloodRequest::getBloodGroup() {
    return BloodGroup;
}
int BloodRequest::getUnits() {
    return units;
}
QString BloodRequest::getStatus() {
    return status;
}

// Approve / Reject

void BloodRequest::approveRequest() {

    // first we will check if the request is pending 

    if (status == "Pending") {
        status = "Approved";
    }
}

void BloodRequest::rejectRequest() {

   //  checking 

    if (status == "Pending") {
        status = "Rejected";
    }
}
// file handling

void BloodRequest::saveToFile() {
    QFile file("Database/requests.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << requestID << "," << patientName << ","
            << hospitalName << "," << BloodGroup << ","
            << units << "," << status << "\n";
        file.close();
    }
}