
#ifndef BLOODREQUEST_H
#define BLOODREQUEST_H

#include <QString>


class BloodRequest {
private:
    QString requestID;
    QString patientName;
    QString hospitalName;
    QString BloodGroup;
    int units;
    QString status;

public:

    // constructor 

    BloodRequest(QString requestID, QString patientName,
        QString hospitalName, QString BloodGroup, int unit);

    //getter 

    QString getRequestId();
    QString getPatientName();
    QString getHospitalName();
    QString getBloodGroup();
    int getUnits();
    QString getStatus();

    // setter or accessor

    void approveRequest();
    void rejectRequest();

    // display 

    void displayRequestInfo();

    // file handling
    void saveToFile();

};

#endif