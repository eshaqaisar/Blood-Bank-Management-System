#include "PatientDashboard.h"
#include "../Utilities/FileManager.h"
#include "../Models/BloodRequest.h"

#include <QHeaderView>
#include <QMessageBox>

// ============================================================
// Constructor
// ============================================================
PatientDashboard::PatientDashboard(const QString& username, QWidget* parent)
    : QWidget(parent), currentUsername(username)
{
    setWindowTitle("Patient Dashboard");
    setMinimumSize(600, 400);

    // Title
    lblTitle = new QLabel("Patient Dashboard - " + currentUsername, this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    // Table
    tblRequests = new QTableWidget(this);
    tblRequests->setColumnCount(6);
    tblRequests->setHorizontalHeaderLabels({
        "Request ID", "Blood Group", "Units", "Hospital", "Date", "Status"
        });

    tblRequests->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblRequests->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Buttons
    btnNewRequest = new QPushButton("New Request", this);
    btnRefresh = new QPushButton("Refresh", this);

    // Layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(lblTitle);
    layout->addWidget(tblRequests);
    layout->addWidget(btnNewRequest);
    layout->addWidget(btnRefresh);
    setLayout(layout);

    // Connections
    connect(btnNewRequest, &QPushButton::clicked, this, &PatientDashboard::onNewRequestClicked);
    connect(btnRefresh, &QPushButton::clicked, this, &PatientDashboard::onRefreshClicked);

    // Load data initially
    loadRequests();
}

// ============================================================
// Load Requests for this patient
// ============================================================
void PatientDashboard::loadRequests()
{
    tblRequests->setRowCount(0);

    QList<BloodRequest> requests = FileManager::loadRequests();

    for (const BloodRequest& req : requests)
    {
        // Only show requests for this logged-in patient
        if (req.getPatientName() != currentUsername)
            continue;

        int row = tblRequests->rowCount();
        tblRequests->insertRow(row);

        tblRequests->setItem(row, 0, new QTableWidgetItem(req.getRequestId()));
        tblRequests->setItem(row, 1, new QTableWidgetItem(req.getRequiredBloodGroup()));
        tblRequests->setItem(row, 2, new QTableWidgetItem(QString::number(req.getUnitsRequired())));
        tblRequests->setItem(row, 3, new QTableWidgetItem(req.getHospitalName()));

        // ⚠️ IMPORTANT: adjust if your function name is different
        tblRequests->setItem(row, 4, new QTableWidgetItem(
            req.getRequestDate().toString("yyyy-MM-dd")
        ));

        tblRequests->setItem(row, 5, new QTableWidgetItem(req.getStatus()));
    }
}

// ============================================================
// Button: Refresh
// ============================================================
void PatientDashboard::onRefreshClicked()
{
    loadRequests();
}

// ============================================================
// Button: New Request
// ============================================================
void PatientDashboard::onNewRequestClicked()
{
    QMessageBox::information(this, "New Request", "Open request form here.");
}