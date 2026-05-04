#ifndef BLOODREQUEST_H
#define BLOODREQUEST_H

#include <string>//std::string replaces QString for all string member variables and method signatures
#include <QDate>//for handling the request date

class BloodRequest {
private:
    std::string requestID;
    std::string patientName;
    std::string hospitalName;
    std::string bloodGroup;
    int         units;
    std::string status;
    QDate       requestDate;

public:
    //default constructor for fromFileString()
    BloodRequest() : units(0), status("Pending"),
        requestDate(QDate::currentDate()) {
    }

    //constructor
    BloodRequest(const std::string& requestID, const std::string& patientName,
        const std::string& hospitalName, const std::string& bloodGroup, int units);

    //getters
    std::string getRequestId()           const { return requestID; }
    std::string getPatientName()         const { return patientName; }
    std::string getHospitalName()        const { return hospitalName; }
    std::string getBloodGroup()          const { return bloodGroup; }
    int         getUnits()               const { return units; }
    std::string getStatus()              const { return status; }
    int         getUnitsRequired()       const { return units; }          //used in FileManager
    std::string getRequiredBloodGroup()  const { return bloodGroup; }     //used by FileManager
    QDate       getRequestDate()         const { return requestDate; }

    //setter/accessor
    void approve() { if (status == "Pending") status = "Approved"; }
    void reject() { if (status == "Pending") status = "Rejected"; }

    //functions added for backward compat
    void approveRequest() { approve(); }
    void rejectRequest() { reject(); }
    void setRequestDate(const QDate& d) { requestDate = d; }

    //file input and output required by FileManager
    std::string         toFileString()                       const;
    static BloodRequest fromFileString(const std::string& line);

    //save old files
    void saveToFile();
};

#endif