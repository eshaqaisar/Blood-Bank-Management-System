#include "RequestManagementForm.h"//header file for the RequestManagementForm class
#include <QFile>//for reading and writing request data from/to the file
#include <QTextStream>//for parsing request data from the file and writing approval certificates
#include <QMessageBox>//for showing message boxes to the user (e.g., for warnings, confirmations, and information)
#include <QDateTime>//for adding timestamps to approval certificates

RequestManagementForm::RequestManagementForm(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
	//title
    QLabel *title = new QLabel("Request Management", this);
    title->setAlignment(Qt::AlignCenter);
	//request table to display pending requests with columns for patient name, hospital, blood group, and units requested. The table allows the admin to select a request for approval or rejection.
    requestsTable = new QTableWidget(this);
    requestsTable->setColumnCount(4);
    requestsTable->setHorizontalHeaderLabels(
        {"Patient Name", "Hospital", "Blood Group", "Units"});
	//style the table to match the overall design of the admin dashboard
    approveBtn = new QPushButton("Approve", this);
    rejectBtn  = new QPushButton("Reject", this);
	//connect buttons to their respective slots for handling clicks
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(approveBtn);
    btnLayout->addWidget(rejectBtn);
	//overall layout for the request management form
    mainLayout->addWidget(title);
    mainLayout->addWidget(requestsTable);
    mainLayout->addLayout(btnLayout);
	//set the main layout for the form
    setLayout(mainLayout);
	//connect buttons to their respective slots for handling clicks
    connect(approveBtn, &QPushButton::clicked,
            this, &RequestManagementForm::onApproveClicked);
    connect(rejectBtn, &QPushButton::clicked,
            this, &RequestManagementForm::onRejectClicked);
	//load pending requests from the file and populate the table
    loadRequests();
}

RequestManagementForm::~RequestManagementForm() {}
//load pending requests from the file and populate the table. This method reads the requests from the "requests.txt" file, parses each line to extract the request details, and populates the requests table with the relevant information for each pending request. The method ensures that only valid requests with the expected format are loaded into the table.
void RequestManagementForm::loadRequests()
{
    requestsTable->setRowCount(0);
    QFile file("../Database/requests.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))//show an error message if the file cannot be opened and return early to prevent further errors
        return;
	//read the file line by line and populate the requests table. Each line is expected to be in the format: "Patient Name,Hospital,Blood Group,Units". The method splits each line by commas, trims whitespace, and populates the corresponding columns in the table. Only lines with at least 4 parts are considered valid requests and are added to the table.
    QTextStream in(&file);
    int row = 0;
	while (!in.atEnd())//read each line from the file until the end is reached
    {
        QString line = in.readLine();
        QStringList parts = line.split(",");
		if (parts.size() >= 4)//check if the line has at least 4 parts (patient name, hospital, blood group, units) before trying to access them to prevent out-of-bounds errors
        {
            requestsTable->insertRow(row);
			for (int i = 0; i < 4; i++)//populate the table with the request details, trimming any extra whitespace from each part
                requestsTable->setItem(row, i,
                    new QTableWidgetItem(parts[i].trimmed()));
            row++;
        }
    }
    file.close();
}
//slot that is called when the "Approve" button is clicked, it checks if a request is selected in the table, retrieves the request details, updates the inventory accordingly, generates an approval certificate for the patient, and removes the approved request from the table. If no request is selected, it shows a warning message to the user.
void RequestManagementForm::onApproveClicked()
{
    int row = requestsTable->currentRow();
	if (row < 0) //check if a request is selected in the table, if not show a warning message and return early to prevent further errors
    {
        QMessageBox::warning(this, "Warning", "Please select a request first!");
        return;
    }
	//retrieve request details from the selected row in the table, including the patient name, blood group, and units requested. These details are used to update the inventory and generate the approval certificate.
    QString patientName = requestsTable->item(row, 0)->text();
    QString bloodGroup  = requestsTable->item(row, 2)->text();
    int units = requestsTable->item(row, 3)->text().toInt();
	//update the inventory by reducing the available units for the requested blood group. The updateInventory method is a placeholder and should be implemented to modify the inventory data accordingly (e.g., by updating the inventory file or using a BloodInventory class).
    updateInventory(bloodGroup, units);
	//generate an approval certificate for the patient, which is saved as a text file in the "Database" directory with a name based on the patient's name. The certificate includes the patient's name, blood group, units approved, date and time of approval, and a status indicating that the request has been approved.
    QFile cert("../Database/certificate_" + patientName + ".txt");
    if(cert.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&cert);
        out << "=== BLOOD BANK APPROVAL CERTIFICATE ===\n";
        out << "Patient:     " << patientName << "\n";
        out << "Blood Group: " << bloodGroup  << "\n";
        out << "Units:       " << units       << "\n";
        out << "Date:        " << QDateTime::currentDateTime().toString() << "\n";
        out << "Status:      APPROVED\n";
        out << "========================================\n";
        cert.close();
    }

    requestsTable->removeRow(row);
    QMessageBox::information(this, "Success", "Request approved! Certificate generated.");
}
//slot that is called when the "Reject" button is clicked, it checks if a request is selected in the table, and if so, it removes the selected request from the table and shows an information message indicating that the request has been rejected. If no request is selected, it shows a warning message to the user.
void RequestManagementForm::onRejectClicked()
{
    int row = requestsTable->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Warning", "Please select a request first!");
        return;
    }
    requestsTable->removeRow(row);
    QMessageBox::information(this, "Rejected", "Request has been rejected.");
}
//placeholder method to update the inventory after approving a request. This method should be implemented to modify the inventory data accordingly (e.g., by updating the inventory file or using a BloodInventory class) to reflect the reduction in available units for the requested blood group after approval.
void RequestManagementForm::updateInventory(QString bloodGroup, int units)
{
    Q_UNUSED(bloodGroup);
    Q_UNUSED(units);
}