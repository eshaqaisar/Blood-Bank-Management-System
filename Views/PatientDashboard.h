#pragma once
#include <QWidget>//base class for all UI elements in Qt
#include <QLabel>//for displaying text such as the dashboard title and request information
#include <QTableWidget>//for displaying the patient's blood requests in a tabular format
#include <QPushButton>//for action buttons like "New Request" and "Refresh"
#include <QVBoxLayout>//for organizing the layout of the dashboard in a vertical manner

class PatientDashboard : public QWidget {
    Q_OBJECT

private:
	QString currentUsername; //stores the username of the currently logged-in patient, used to filter and display only their blood requests in the dashboard  
	QLabel* lblTitle;//label for the dashboard title, which includes the patient's username for a personalized touch and is styled to be larger and bold to clearly indicate the purpose of the page
	QTableWidget* tblRequests;//table widget to display the patient's blood requests, with columns for request ID, blood group, units requested, hospital name, request date, and status. The table is set to be read-only to prevent accidental edits and is styled to match the overall design of the dashboard
	QPushButton* btnNewRequest;//button to allow the patient to create a new blood request, which will open a form for entering the request details. The button is styled to be prominent and inviting for patients to click and is connected to a slot that will handle the click event and open the new request form
	QPushButton* btnRefresh;//button to allow the patient to refresh the list of blood requests, which will reload the request data from the file and update the display. This is useful for patients to see the most up-to-date status of their requests after making changes or to refresh the view if they suspect it may be outdated. The button is styled to be easily identifiable and is connected to a slot that will handle the click event and refresh the request list

public:
    PatientDashboard(const QString& username, QWidget* parent = nullptr); 

public slots:
	void onNewRequestClicked();//slot that is called when the "New Request" button is clicked, it currently shows a message box indicating that the request form should be opened here. In a complete implementation, this slot would open a new form or dialog where the patient can fill out the details of their blood request, such as selecting the required blood group, specifying the number of units needed, and providing hospital information. After submitting the request form, the new request would be saved to the file and the dashboard would be refreshed to show the updated list of requests.
	void onRefreshClicked();//slot that is called when the "Refresh" button is clicked, it calls the loadRequests method to reload the blood request data from the file and update the display. This allows the patient to see the most up-to-date status of their blood requests after making changes or to refresh the view if they suspect it may be outdated.
	void loadRequests();//method to load blood request data from the file and populate the requests table. This method retrieves all blood requests using the FileManager utility, filters them to show only the requests that belong to the currently logged-in patient (using the currentUsername member variable), and populates the table with the relevant information for each request. The request date is formatted as "yyyy-MM-dd" for display in the table, and the method ensures that only the patient's own requests are shown in their dashboard.
};