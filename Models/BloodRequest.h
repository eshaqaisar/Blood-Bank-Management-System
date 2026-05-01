
#ifndef BLOODREQUEST_H
#define BLOODREQUEST_H

#include <QString>
#include <QDate> 


class BloodRequest {
private:
    QString requestID;
    QString patientName;
    QString hospitalName;
    QString bloodGroup;
    int units;
    QString status;
    QDate   requestDate;

public:

    // default constructor  for  fromFileString() 
    BloodRequest() : units(0), status("Pending"),
        requestDate(QDate::currentDate()) {
    }

    // constructor 

    BloodRequest(const QString& requestID,const QString& patientName,
       const QString& hospitalName,const QString& bloodGroup, int unit);

    //getter 

    QString getRequestId()    const { return requestID; }
    QString getPatientName()  const { return patientName; }
    QString getHospitalName() const { return hospitalName; }
    QString getBloodGroup()     const { return bloodGroup; }
    int getUnits()    const { return units; }
    QString getStatus()      const { return status; }
    int     getUnitsRequired()     const { return units; } // used in FileManager 
    QString getRequiredBloodGroup()const { return bloodGroup; }   // used by FileManager
    QDate getRequestDate() const { return requestDate; }

    // setter or accessor

    void approve() { if (status == "Pending") status = "Approved"; }
    void reject() { if (status == "Pending") status = "Rejected"; }


    void approveRequest() { approve(); }
    void rejectRequest() { reject(); }

    void setRequestDate(const QDate& d) { requestDate = d; }

    // File Input and output required by FileManager
    QString      toFileString()                    const;
    static BloodRequest fromFileString(const QString& line);

    // save old files
    void saveToFile();

   
};

#endif