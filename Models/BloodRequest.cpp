#include "BloodRequest.h"
#include <iostream>
#include <fstream>
using namespace std;

// Constructor
BloodRequest::BloodRequest(string requestID, string patientName,
    string hospitalName, string BloodGroup, int units) {

    this->requestID = requestID;
    this->patientName = patientName;
    this->hospitalName = hospitalName;
    this->BloodGroup = BloodGroup;
    this->units = units;

}

// Getters
string BloodRequest::getRequestId() {
    return requestID;
}
string BloodRequest::getPatientName() {
    return patientName;
}
string BloodRequest::getHospitalName() {
    return hospitalName;
}
string BloodRequest::getBloodGroup() {
    return BloodGroup;
}
int BloodRequest::getUnits() {
    return units;
}
string BloodRequest::getStatus() {
    return status;
}

// Approve / Reject

void BloodRequest::approveRequest() {

    // first we will check if the request is pending 

    if (status == "Pending") {
        status = "Approved";
        cout << " Request " << requestID << " has been APPROVED" << endl;
    }
    else {
        std::cout << " Cannot approve - Request is already " << status << endl;
    }
}

void BloodRequest::rejectRequest() {

   //  checking 

    if (status == "Pending") {
        status = "Rejected";
        cout << " Request " << requestID << " has been REJECTED" << endl;
    }
    else {
        cout << " Cannot reject - Request is already " << status << endl;
    }
}

// Display
void BloodRequest::displayRequestInfo() {

    cout << "\n********* BLOOD REQUEST **********\n" << endl;

    cout << "Request ID:    " << requestID << endl;
    cout << "Patient Name:  " << patientName << endl;
    cout << "Hospital:      " << hospitalName << endl;
    cout << "Blood Group:   " << BloodGroup << endl;
    cout << "Units Needed:  " << units << endl;
    cout << "Status:        " << status << endl;

    cout << "====================================" << endl;

    }

// file handling

void BloodRequest::saveToFile() {
    ofstream file("Database/requests.txt", ios::app);
    if (file.is_open()) {
        file << requestID << ","
            << patientName << ","
            << hospitalName << ","
            << BloodGroup << ","
            << units << ","
            << status << "\n";
        file.close();
    }
}