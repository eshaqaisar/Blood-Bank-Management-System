#include "RequestManagementForm.h"//for the class definition
#include "../Utilities/FileManager.h"//for FileManager to load/update requests and inventory
#include "../Models/BloodRequest.h"//for BloodRequest model to represent each request
#include "../Models/BloodBag.h"//for BloodBag model to represent inventory items
#include <QHeaderView>//for QHeaderView to adjust table column sizes
#include <QMessageBox>//for QMessageBox to show dialogs
#include <QDateTime>//for QDateTime to timestamp certificates
#include <QFile>//for QFile to read/write files
#include <QTextStream>//for QTextStream to write certificate content
#include <QDir>//for QDir to create directories if needed
#include <QColor>//for QColor to style table items

RequestManagementForm::RequestManagementForm(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);
	//add a title label with an emoji for visual appeal
    QLabel* title = new QLabel("📋 Blood Request Management", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");

	//set up the table to display requests with appropriate columns and styling
    requestsTable = new QTableWidget(this);
    requestsTable->setColumnCount(6);
    requestsTable->setHorizontalHeaderLabels(
        { "Request ID", "Patient Name", "Hospital", "Blood Group", "Units", "Status" });
    requestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    requestsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    requestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    requestsTable->setAlternatingRowColors(true);
	//add buttons with emojis and consistent styling
    approveBtn = new QPushButton("✅ Approve", this);
    rejectBtn = new QPushButton("❌ Reject", this);
    refreshBtn = new QPushButton("🔄 Refresh", this);
	//style buttons with colors and padding for better UX
    approveBtn->setStyleSheet("background:#27ae60; color:white; border-radius:5px; padding:8px; font-weight:bold;");
    rejectBtn->setStyleSheet("background:#e74c3c; color:white; border-radius:5px; padding:8px; font-weight:bold;");
    refreshBtn->setStyleSheet("background:#2980b9; color:white; border-radius:5px; padding:8px; font-weight:bold;");
	//arrange buttons in a horizontal layout with spacing
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
	//connect button signals to their respective slots
    connect(approveBtn, &QPushButton::clicked, this, &RequestManagementForm::onApproveClicked);
    connect(rejectBtn, &QPushButton::clicked, this, &RequestManagementForm::onRejectClicked);
    connect(refreshBtn, &QPushButton::clicked, this, &RequestManagementForm::onRefreshClicked);

    loadRequests();
}

RequestManagementForm::~RequestManagementForm() {}
//load requests from the file and populate the table, only showing pending requests for admin action
void RequestManagementForm::loadRequests()
{
    requestsTable->setRowCount(0);
    requestIds.clear();

    
    QList<BloodRequest> requests = FileManager::loadRequests();

    for (const BloodRequest& req : requests) {
		//shows only pending requests in the table, approved/rejected ones are hidden since they can't be acted on anymore
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

		//keep track of request IDs in the same order as they appear in the table for later reference when approving/rejecting
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
//approve the selected request, update inventory, and generate a certificate
void RequestManagementForm::onApproveClicked()
{
    int row = requestsTable->currentRow();
    if (row < 0 || row >= requestIds.size()) {
        QMessageBox::warning(this, "No Selection", "Please select a request first.");
        return;
    }

    QString reqId = requestIds[row];
    QString patient = requestsTable->item(row, 1)->text();
    QString hospital = requestsTable->item(row, 2)->text();
    QString bloodGroup = requestsTable->item(row, 3)->text();
    int     units = requestsTable->item(row, 4)->text().toInt();

    //actually persist the status change to the file
    FileManager::updateRequestStatus(reqId, "Approved");

    //decrement inventory (previously updateInventory() was empty)
    updateInventory(bloodGroup, units);

    //certificate saved to "Database/" not "../Database/"
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
    loadRequests(); //refresh , approved row disappears from Pending list
}
//approve the selected request, update inventory, and generate a certificate
void RequestManagementForm::onRejectClicked()
{
    int row = requestsTable->currentRow();
    if (row < 0 || row >= requestIds.size()) 
    {
        QMessageBox::warning(this, "No Selection", "Please select a request first.");
        return;
    }

    QString reqId = requestIds[row];

    //persist the rejection to the file
    FileManager::updateRequestStatus(reqId, "Rejected");

    QMessageBox::information(this, "Rejected", "Request " + reqId + " has been rejected.");
    loadRequests(); //refresh
}

void RequestManagementForm::onRefreshClicked()
{
    loadRequests();
}


void RequestManagementForm::updateInventory(const QString& bloodGroup, int units)
{
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);

    bool ok = inventory.useBag(bloodGroup, units);
    if (ok) {
        inventory.save(FileManager::INVENTORY_FILE);
    }
    else {
        QMessageBox::warning(this, "Inventory Warning",
            "Not enough " + bloodGroup + " stock to fulfil this request.\n"
            "Request approved but inventory not decremented — please restock.");
    }
}