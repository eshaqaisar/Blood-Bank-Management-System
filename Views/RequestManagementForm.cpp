#include "RequestManagementForm.h"//for the class definition
#include "../Utilities/FileManager.h"//for FileManager to load/update requests and inventory
#include "../Models/BloodRequest.h"//for BloodRequest model to represent each request
#include "../Models/BloodBag.h"//for BloodInventory to pass into updateRequestStatus
#include <QHeaderView>//for QHeaderView to adjust table column sizes
#include <QMessageBox>//for QMessageBox to show dialogs
#include <QDir>//for QDir to create directories if needed
#include <QColor>//for QColor to style table items
#include <QString>//used only at Qt UI boundary via fromStdString / toStdString
#include <string>//for std::string used throughout

RequestManagementForm::RequestManagementForm(QWidget* parent)
    : QWidget(parent), requestCount(0)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    QLabel* title = new QLabel("📋 Blood Request Management", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50;");

    //blood group filter dropdown — FIX for crash: was passing raw QComboBox text
    //into a QString comparison that could be empty or mismatched.
    //Now uses std::string and passes "" to filterRequestsByBloodGroup when "All" selected.
    cmbFilter = new QComboBox(this);
    cmbFilter->addItem("All Groups");
    cmbFilter->addItem("A+");  cmbFilter->addItem("A-");
    cmbFilter->addItem("B+");  cmbFilter->addItem("B-");
    cmbFilter->addItem("AB+"); cmbFilter->addItem("AB-");
    cmbFilter->addItem("O+");  cmbFilter->addItem("O-");
    connect(cmbFilter, &QComboBox::currentTextChanged, this, &RequestManagementForm::onFilterChanged);

    QHBoxLayout* filterRow = new QHBoxLayout();
    filterRow->addWidget(new QLabel("Filter by Blood Group:", this));
    filterRow->addWidget(cmbFilter);
    filterRow->addStretch();

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

    approveBtn->setStyleSheet("background:#27ae60; color:white; border-radius:5px; padding:8px; font-weight:bold;");
    rejectBtn->setStyleSheet("background:#e74c3c; color:white; border-radius:5px; padding:8px; font-weight:bold;");
    refreshBtn->setStyleSheet("background:#2980b9; color:white; border-radius:5px; padding:8px; font-weight:bold;");

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(approveBtn);
    btnLayout->addWidget(rejectBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(refreshBtn);

    //assemble the main layout
    mainLayout->addWidget(title);
    mainLayout->addLayout(filterRow);
    mainLayout->addWidget(requestsTable);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(approveBtn, &QPushButton::clicked, this, &RequestManagementForm::onApproveClicked);
    connect(rejectBtn, &QPushButton::clicked, this, &RequestManagementForm::onRejectClicked);
    connect(refreshBtn, &QPushButton::clicked, this, &RequestManagementForm::onRefreshClicked);

    loadRequests();
}

RequestManagementForm::~RequestManagementForm() {}

//load requests from the file and populate the table.
//FIX: now uses filterRequestsByBloodGroup() with safe std::string comparison
//     instead of the raw QComboBox text that caused the crash.
void RequestManagementForm::loadRequests()
{
    requestsTable->setRowCount(0);
    requestCount = 0; //reset the plain array counter

    //get filter value; if "All Groups" selected, pass "" to return everything
    std::string filterBG = "";
    if (cmbFilter->currentText() != "All Groups")
        filterBG = cmbFilter->currentText().toStdString();

    //FIX: use safe filter method instead of raw comparison that caused crash
    QList<BloodRequest> requests = FileManager::filterRequestsByBloodGroup(filterBG);

    for (const BloodRequest& req : requests) {
        //shows only pending requests in the table
        if (req.getStatus() != "Pending") continue;

        int row = requestsTable->rowCount();
        requestsTable->insertRow(row);

        //convert std::string to QString only at Qt UI boundary
        requestsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(req.getRequestId())));
        requestsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(req.getPatientName())));
        requestsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(req.getHospitalName())));
        requestsTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(req.getRequiredBloodGroup())));
        requestsTable->setItem(row, 4, new QTableWidgetItem(QString::number(req.getUnitsRequired())));

        QTableWidgetItem* statusItem = new QTableWidgetItem(QString::fromStdString(req.getStatus()));
        statusItem->setForeground(QColor("#e67e22")); //Orange = Pending
        requestsTable->setItem(row, 5, statusItem);

        //track request IDs in the plain array (replaces QStringList)
        if (requestCount < MAX_REQUESTS)
            requestIds[requestCount++] = req.getRequestId();
    }

    if (requestsTable->rowCount() == 0) {
        requestsTable->insertRow(0);
        QTableWidgetItem* ph = new QTableWidgetItem("No pending requests.");
        ph->setForeground(QColor("#aaa"));
        requestsTable->setItem(0, 0, ph);
        requestsTable->setSpan(0, 0, 1, 6);
    }
}

//FIX: onApproveClicked now correctly refuses to approve when stock is insufficient.
//Previously: updateRequestStatus was called unconditionally, then updateInventory()
//            showed a warning AFTER the file was already written as "Approved".
//Now:        FileManager::updateRequestStatus() checks inventory FIRST.
//            It returns false if stock is insufficient, and we show an error without
//            changing the request status at all — request stays Pending.
void RequestManagementForm::onApproveClicked()
{
    int row = requestsTable->currentRow();
    if (row < 0 || row >= requestCount) {
        QMessageBox::warning(this, "No Selection", "Please select a request first.");
        return;
    }

    //get request ID from our plain array (replaces requestIds[row] on QStringList)
    std::string reqId = requestIds[row];

    //load inventory to pass into updateRequestStatus for atomic stock check
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);

    //updateRequestStatus returns false if stock is insufficient — do NOT approve then
    bool success = FileManager::updateRequestStatus(reqId, "Approved", inventory);
    if (!success) {
        //stock was insufficient: request stays Pending, file unchanged
        QMessageBox::warning(this, "Insufficient Stock",
            "Not enough blood units available for this request.\n"
            "The request has NOT been approved. Please restock first.");
        return;
    }

    //stock was sufficient: approved and inventory decremented — generate certificate
    QDir().mkpath("Database");
    QString certPath = "Database/certificate_" + QString::fromStdString(reqId) + ".txt";
    //retrieve full request details for the certificate
    QList<BloodRequest> allReqs = FileManager::loadRequests();
    for (const BloodRequest& r : allReqs) {
        if (r.getRequestId() == reqId) {
            FileManager::generateCertificate(r, certPath.toStdString());
            break;
        }
    }

    QMessageBox::information(this, "Approved",
        "Request " + QString::fromStdString(reqId) + " approved.\n"
        "Inventory updated.\nCertificate saved to Database/.");
    loadRequests(); //refresh — approved row disappears from Pending list
}

void RequestManagementForm::onRejectClicked()
{
    int row = requestsTable->currentRow();
    if (row < 0 || row >= requestCount) {
        QMessageBox::warning(this, "No Selection", "Please select a request first.");
        return;
    }

    std::string reqId = requestIds[row];

    //rejection does not touch inventory; pass a dummy inventory reference
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    FileManager::updateRequestStatus(reqId, "Rejected", inventory);

    QMessageBox::information(this, "Rejected",
        "Request " + QString::fromStdString(reqId) + " has been rejected.");
    loadRequests(); //refresh
}

//refresh button: reload all requests from disk
void RequestManagementForm::onRefreshClicked()
{
    loadRequests();
}

//FIX: blood group filter crash — previously the raw QComboBox text was passed
//into QString comparisons that could crash on empty or unexpected values.
//Now delegates to loadRequests() which safely reads cmbFilter and calls
//filterRequestsByBloodGroup() with a proper std::string.
void RequestManagementForm::onFilterChanged()
{
    loadRequests();
}