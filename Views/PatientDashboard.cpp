#include "PatientDashboard.h"//include the header file for the PatientDashboard class
#include "BloodRequestForm.h"//include the header file for the BloodRequestForm class
#include "LandingPage.h"//include the header file for the LandingPage class
#include "../Utilities/FileManager.h"//for FileManager::loadRequests() and FileManager::logActivity()
#include "../Models/BloodRequest.h"//for BloodRequest class
#include <QMessageBox>//for QMessageBox
#include <QHeaderView>//for QHeaderView used in setting up the table
#include <QColor>//for colour-coding status cells
#include <QString>//used only at Qt UI boundary via fromStdString
#include <string>//for std::string used throughout

PatientDashboard::PatientDashboard(const std::string& username, QWidget* parent)
    : QWidget(parent), currentUsername(username)
{
    //convert std::string to QString only at Qt window title boundary
    setWindowTitle("Patient Dashboard - " + QString::fromStdString(username));
    setMinimumSize(680, 480);

    lblTitle = new QLabel("🏥 Patient Dashboard — " + QString::fromStdString(currentUsername), this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #2c3e50;");

    //request table setup
    tblRequests = new QTableWidget(this);
    tblRequests->setColumnCount(6);
    tblRequests->setHorizontalHeaderLabels({
        "Request ID", "Blood Group", "Units", "Hospital", "Date", "Status" });
    tblRequests->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblRequests->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblRequests->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblRequests->setAlternatingRowColors(true);

    //buttons setup
    btnNewRequest = new QPushButton("➕ New Request", this);
    btnRefresh = new QPushButton("🔄 Refresh", this);
    btnLogout = new QPushButton("🚪 Logout", this);

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnNewRequest);
    btnRow->addWidget(btnRefresh);
    btnRow->addStretch();
    btnRow->addWidget(btnLogout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setSpacing(12);
    layout->addWidget(lblTitle);
    layout->addWidget(tblRequests);
    layout->addLayout(btnRow);
    setLayout(layout);

    setStyleSheet(R"(
        QWidget { background: #fff; font-family: Arial; font-size: 13px; }
        QTableWidget { border: 1px solid #ddd; gridline-color: #f0f0f0; }
        QHeaderView::section {
            background: #c0392b; color: white; padding: 7px; font-weight: bold;
        }
        QTableWidget::item:selected { background: #fadbd8; color: #2c3e50; }
        QPushButton {
            background: #c0392b; color: white;
            border-radius: 6px; padding: 9px 16px; font-size: 13px; font-weight: bold;
        }
        QPushButton:hover { background: #e74c3c; }
    )");

    connect(btnNewRequest, &QPushButton::clicked, this, &PatientDashboard::onNewRequestClicked);
    connect(btnRefresh, &QPushButton::clicked, this, &PatientDashboard::onRefreshClicked);
    connect(btnLogout, &QPushButton::clicked, this, &PatientDashboard::onLogout);

    loadRequests();
}

//load the blood requests submitted by the logged-in patient and display them in the table
void PatientDashboard::loadRequests()
{
    tblRequests->setRowCount(0);

    QList<BloodRequest> requests = FileManager::loadRequests();

    for (const BloodRequest& req : requests) {
        //only show requests submitted by THIS logged-in user
        //std::string comparison — no Qt involved here
        if (req.getPatientName() != currentUsername)
            continue;

        int row = tblRequests->rowCount();
        tblRequests->insertRow(row);

        //convert std::string to QString only at Qt table item boundary
        tblRequests->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(req.getRequestId())));
        tblRequests->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(req.getRequiredBloodGroup())));
        tblRequests->setItem(row, 2, new QTableWidgetItem(QString::number(req.getUnitsRequired())));
        tblRequests->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(req.getHospitalName())));
        tblRequests->setItem(row, 4, new QTableWidgetItem(req.getRequestDate().toString("yyyy-MM-dd")));

        //colour-code the status cell; getStatus() returns std::string
        std::string status = req.getStatus();
        QTableWidgetItem* statusItem = new QTableWidgetItem(QString::fromStdString(status));
        if (status == "Approved")       statusItem->setForeground(QColor("#27ae60"));
        else if (status == "Rejected")  statusItem->setForeground(QColor("#e74c3c"));
        else                            statusItem->setForeground(QColor("#e67e22")); //Pending = orange
        tblRequests->setItem(row, 5, statusItem);
    }

    if (tblRequests->rowCount() == 0) {
        tblRequests->insertRow(0);
        QTableWidgetItem* placeholder = new QTableWidgetItem(
            "No requests yet — click ➕ New Request to submit one.");
        placeholder->setForeground(QColor("#aaa"));
        tblRequests->setItem(0, 0, placeholder);
        tblRequests->setSpan(0, 0, 1, 6);
    }
}

//slot for handling the "New Request" button click - opens the BloodRequestForm and connects its submission signal to refresh the table
void PatientDashboard::onNewRequestClicked()
{
    //BloodRequestForm constructor takes QString for username; convert at boundary
    BloodRequestForm* form = new BloodRequestForm(QString::fromStdString(currentUsername));
    form->setAttribute(Qt::WA_DeleteOnClose);
    form->show();
    //when the form submits, automatically refresh this table
    connect(form, &BloodRequestForm::requestSubmitted, this, &PatientDashboard::loadRequests);
}

void PatientDashboard::onRefreshClicked()
{
    loadRequests();
}

//slot for handling the "Logout" button click
void PatientDashboard::onLogout()
{
    FileManager::logActivity("Patient logged out: " + currentUsername);
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}