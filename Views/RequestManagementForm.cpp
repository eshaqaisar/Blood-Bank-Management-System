#include "RequestManagementForm.h"//include the header file for the RequestManagementForm class
#include "../Utilities/FileManager.h"//for FileManager::loadRequests() and FileManager::updateRequestStatus()
#include "../Models/BloodRequest.h"//for BloodRequest class
#include "../Models/BloodBag.h"//for BloodBag class
#include <QHeaderView>//for QHeaderView used in setting up the table
#include <QMessageBox>//for QMessageBox
#include <QDateTime>//for QDateTime used in certificate generation
#include <QFile>//for QFile used in certificate generation
#include <QTextStream>//for QTextStream used in certificate generation
#include <QDir>//for QDir::mkpath() used in certificate generation
#include <QColor>//for QColor used in status coloring

RequestManagementForm::RequestManagementForm(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    QLabel* title = new QLabel("📋 Blood Request Management", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");

	//request table setup
    requestsTable = new QTableWidget(this);
    requestsTable->setColumnCount(6);
    requestsTable->setHorizontalHeaderLabels(
        { "Request ID", "Patient Name", "Hospital", "Blood Group", "Units", "Status" });
    requestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    requestsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    requestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    requestsTable->setAlternatingRowColors(true);
	//buttons setup
    approveBtn = new QPushButton("✅ Approve", this);
    rejectBtn = new QPushButton("❌ Reject", this);
    refreshBtn = new QPushButton("🔄 Refresh", this);
	//simple styling for a cleaner look
    approveBtn->setStyleSheet("background:#27ae60; color:white; border-radius:5px; padding:8px; font-weight:bold;");
    rejectBtn->setStyleSheet("background:#e74c3c; color:white; border-radius:5px; padding:8px; font-weight:bold;");
    refreshBtn->setStyleSheet("background:#2980b9; color:white; border-radius:5px; padding:8px; font-weight:bold;");
	//arrange buttons in a horizontal layout
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(approveBtn);
    btnLayout->addWidget(rejectBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(refreshBtn);
	//assemble the main layout
    mainLayout->addWidget(title);
    mainLayout->addWidget(requestsTable);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);
	//connect buttons to their respective slots
    connect(approveBtn, &QPushButton::clicked, this, &RequestManagementForm::onApproveClicked);
    connect(rejectBtn, &QPushButton::clicked, this, &RequestManagementForm::onRejectClicked);
    connect(refreshBtn, &QPushButton::clicked, this, &RequestManagementForm::onRefreshClicked);

    loadRequests();
}

RequestManagementForm::~RequestManagementForm() {}
//fetch all pending blood requests from the file and display them in the table
void RequestManagementForm::loadRequests()
{
    requestsTable->setRowCount(0);
    requestIds.clear();

	
    QList<BloodRequest> requests = FileManager::loadRequests();

    for (const BloodRequest& req : requests) {
		//only show requests that are still pending approval
        if (req.getStatus() != "Pending") continue;

        int row = requestsTable->rowCount();
        requestsTable->insertRow(row);

        requestsTable->setItem(row, 0, new QTableWidgetItem(req.getRequestId()));
        requestsTable->setItem(row, 1, new QTableWidgetItem(req.getPatientName()));
        requestsTable->setItem(row, 2, new QTableWidgetItem(req.getHospitalName()));
        requestsTable->setItem(row, 3, new QTableWidgetItem(req.getRequiredBloodGroup()));
        requestsTable->setItem(row, 4, new QTableWidgetItem(QString::number(req.getUnitsRequired())));

        QTableWidgetItem* statusItem = new QTableWidgetItem(req.getStatus());
        statusItem->setForeground(QColor("#e67e22")); // Orange = Pending
        requestsTable->setItem(row, 5, statusItem);

		// Keep track of the request IDs in the same order as they appear in the table
        requestIds.append(req.getRequestId());
    }

    if (requestsTable->rowCount() == 0) 
    {
        requestsTable->insertRow(0);
        QTableWidgetItem* ph = new QTableWidgetItem("No pending requests.");
        ph->setForeground(QColor("#aaa"));
        requestsTable->setItem(0, 0, ph);
        requestsTable->setSpan(0, 0, 1, 6);
    }
}

void RequestManagementForm::onApproveClicked()
{
    int row = requestsTable->currentRow();
	if (row < 0 || row >= requestIds.size()) // Check if a valid row is selected
    {
        QMessageBox::warning(this, "No Selection", "Please select a request first.");
        return;
    }

    QString reqId = requestIds[row];
    QString patient = requestsTable->item(row, 1)->text();
    QString hospital = requestsTable->item(row, 2)->text();
    QString bloodGroup = requestsTable->item(row, 3)->text();
    int     units = requestsTable->item(row, 4)->text().toInt();

    //ctually persist the status change to the file
    FileManager::updateRequestStatus(reqId, "Approved");

    //decrement inventory
    updateInventory(bloodGroup, units);

	//generate a simple text certificate for the approved request
    QDir().mkpath("Database");
    QString certPath = "Database/certificate_" + reqId + ".txt";
    QFile cert(certPath);
    if (cert.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&cert);
        out << "=== BLOOD BANK APPROVAL CERTIFICATE ===\n";
        out << "Request ID:  " << reqId << "\n";
        out << "Patient:     " << patient << "\n";
        out << "Hospital:    " << hospital << "\n";
        out << "Blood Group: " << bloodGroup << "\n";
        out << "Units:       " << units << "\n";
        out << "Date:        " << QDateTime::currentDateTime().toString() << "\n";
        out << "Status:      APPROVED\n";
        out << "========================================\n";
        cert.close();
    }

    QMessageBox::information(this, "Approved",
        "Request " + reqId + " approved.\nInventory updated.\nCertificate saved.");
    loadRequests(); // Refresh — approved row disappears from Pending list
}
//handle rejection of a blood request
void RequestManagementForm::onRejectClicked()
{
    int row = requestsTable->currentRow();
    if (row < 0 || row >= requestIds.size())
    {
        QMessageBox::warning(this, "No Selection", "Please select a request first.");
        return;
    }

    QString reqId = requestIds[row];

    //actually persist the rejection to the file
    FileManager::updateRequestStatus(reqId, "Rejected");

    QMessageBox::information(this, "Rejected", "Request " + reqId + " has been rejected.");
    loadRequests(); //refresh
}

void RequestManagementForm::onRefreshClicked()
{
    loadRequests();
}
//helper function to update the blood inventory when a request is approved
void RequestManagementForm::updateInventory(const QString& bloodGroup, int units)
{
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);

    bool ok = inventory.useBag(bloodGroup, units);
    if (ok) 
    {
        inventory.save(FileManager::INVENTORY_FILE);
    }
    else 
    {
        QMessageBox::warning(this, "Inventory Warning",
            "Not enough " + bloodGroup + " stock to fulfil this request.\n"
            "Request approved but inventory not decremented — please restock.");
    }
}