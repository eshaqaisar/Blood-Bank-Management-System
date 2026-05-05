#pragma once
#include <QWidget>//for QWidget
#include <QLabel>//for QLabel
#include <QTableWidget>//for QTableWidget
#include <QPushButton>//for QPushButton
#include <QVBoxLayout>//for QVBoxLayout
#include <QHBoxLayout>//for QHBoxLayout
#include <QHeaderView>//for QHeaderView used in setting up the table
#include <string>//std::string replaces QString for currentUsername member variable

class PatientDashboard : public QWidget {
    Q_OBJECT

private:
    std::string   currentUsername; //std::string replaces QString
    QLabel* lblTitle;
    QTableWidget* tblRequests;
    QPushButton* btnNewRequest;
    QPushButton* btnRefresh;
    QPushButton* btnLogout;

public:
    //constructor takes std::string; converts to QString only at UI boundary
    explicit PatientDashboard(const std::string& username, QWidget* parent = nullptr);

public slots:
    void onNewRequestClicked(); //opens the BloodRequestForm for submitting a new request
    void onRefreshClicked();    //reloads the blood requests from the file and updates the table
    void loadRequests();        //helper function to load and display the patient's blood requests
    void onLogout();            //logs the activity, opens the LandingPage, and closes the dashboard
};