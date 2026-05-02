#pragma once
#include <QWidget>//for QWidget
#include <QLabel>//for QLabel
#include <QTableWidget>//for QTableWidget
#include <QPushButton>//for QPushButton
#include <QVBoxLayout>//for QVBoxLayout
#include <QHBoxLayout>//for QHBoxLayout
#include <QHeaderView>//for QHeaderView used in setting up the table


class PatientDashboard : public QWidget {
    Q_OBJECT

private:
	QString       currentUsername;//to keep track of the logged-in user
	QLabel* lblTitle;//for the dashboard title
	QTableWidget* tblRequests;//to display the patient's blood requests
	QPushButton* btnNewRequest;//to open the form for submitting a new blood request
	QPushButton* btnRefresh;//to reload the blood requests from the file
	QPushButton* btnLogout;//to log out and return to the landing page

public:
    explicit PatientDashboard(const QString& username, QWidget* parent = nullptr);

public slots:
	void onNewRequestClicked(); //opens the BloodRequestForm for submitting a new request
	void onRefreshClicked();//reloads the blood requests from the file and updates the table
	void loadRequests();//helper function to load and display the patient's blood requests in the table
	void onLogout();//logs the activity, opens the LandingPage, and closes the dashboard
};