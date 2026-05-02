#include "PatientDashboard.h"//header file for the PatientDashboard class
#include "../Utilities/FileManager.h"//for loading blood request data from the file
#include "../Models/BloodRequest.h"//for the BloodRequest data structure and operations, including getters for request details

#include <QHeaderView>//for customizing the appearance of the table headers
#include <QMessageBox>//for showing message boxes to the user (e.g., for information or error messages)


PatientDashboard::PatientDashboard(const QString& username, QWidget* parent)
    : QWidget(parent), currentUsername(username)
{
    setWindowTitle("Patient Dashboard");
    setMinimumSize(600, 400);

    //title
    lblTitle = new QLabel("Patient Dashboard - " + currentUsername, this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    //table
    tblRequests = new QTableWidget(this);
    tblRequests->setColumnCount(6);
    tblRequests->setHorizontalHeaderLabels({
        "Request ID", "Blood Group", "Units", "Hospital", "Date", "Status"
        });

    tblRequests->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblRequests->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //buttons
    btnNewRequest = new QPushButton("New Request", this);
    btnRefresh = new QPushButton("Refresh", this);

    //layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(lblTitle);
    layout->addWidget(tblRequests);
    layout->addWidget(btnNewRequest);
    layout->addWidget(btnRefresh);
    setLayout(layout);

    //connections
    connect(btnNewRequest, &QPushButton::clicked, this, &PatientDashboard::onNewRequestClicked);
    connect(btnRefresh, &QPushButton::clicked, this, &PatientDashboard::onRefreshClicked);

    //load data initially
    loadRequests();
}
//destructor
void PatientDashboard::loadRequests()
{
    tblRequests->setRowCount(0);

    QList<BloodRequest> requests = FileManager::loadRequests();

    for (const BloodRequest& req : requests)
    {
		//only show requests that belong to the current patient
        if (req.getPatientName() != currentUsername)
            continue;

        int row = tblRequests->rowCount();
        tblRequests->insertRow(row);

        tblRequests->setItem(row, 0, new QTableWidgetItem(req.getRequestId()));
        tblRequests->setItem(row, 1, new QTableWidgetItem(req.getRequiredBloodGroup()));
        tblRequests->setItem(row, 2, new QTableWidgetItem(QString::number(req.getUnitsRequired())));
        tblRequests->setItem(row, 3, new QTableWidgetItem(req.getHospitalName()));

		//format the request date as "yyyy-MM-dd" for display in the table
        tblRequests->setItem(row, 4, new QTableWidgetItem(
            req.getRequestDate().toString("yyyy-MM-dd")
        ));

        tblRequests->setItem(row, 5, new QTableWidgetItem(req.getStatus()));
    }
}
//slot that is called when the "Refresh" button is clicked, it calls the loadRequests method to reload the blood request data from the file and update the display. This allows the patient to see the most up-to-date status of their blood requests after making changes or to refresh the view if they suspect it may be outdated.
void PatientDashboard::onRefreshClicked()
{
    loadRequests();
}

//slot that is called when the "New Request" button is clicked, it currently shows a message box indicating that the request form should be opened here. In a complete implementation, this slot would open a new form or dialog where the patient can fill out the details of their blood request, such as selecting the required blood group, specifying the number of units needed, and providing hospital information. After submitting the request form, the new request would be saved to the file and the dashboard would be refreshed to show the updated list of requests.
void PatientDashboard::onNewRequestClicked()
{
    QMessageBox::information(this, "New Request", "Open request form here.");
}