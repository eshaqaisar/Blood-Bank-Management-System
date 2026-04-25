
#ifndef BLOODREQUEST_H
#define BLOODREQUEST_H

#include <string>
using namespace std;

class BloodRequest {
private:
    string requestID;
    string patientName;
    string hospitalName;
    string BloodGroup;
    int units;
    string status;

public:

    // constructor 

    BloodRequest(std::string requestID, std::string patientName,
        std::string hospitalName, std::string BloodGroup, int unit);

    //getter 

    string getRequestId();
    string getPatientName();
    string getHospitalName();
    string getBloodGroup();
    int getUnits();
    string getStatus();

    // setter or accessor

    void approveRequest();
    void rejectRequest();

    // display 

    void displayRequestInfo();

    // file handling
    void saveToFile();

};

#endif