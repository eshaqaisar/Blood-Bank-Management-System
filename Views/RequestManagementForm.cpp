#include "RequestManagementForm.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>

RequestManagementForm::RequestManagementForm(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Request Management", this);
    title->setAlignment(Qt::AlignCenter);

    requestsTable = new QTableWidget(this);
    requestsTable->setColumnCount(4);
    requestsTable->setHorizontalHeaderLabels(
        {"Patient Name", "Hospital", "Blood Group", "Units"});

    approveBtn = new QPushButton("Approve", this);
    rejectBtn  = new QPushButton("Reject", this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(approveBtn);
    btnLayout->addWidget(rejectBtn);

    mainLayout->addWidget(title);
    mainLayout->addWidget(requestsTable);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);

    connect(approveBtn, &QPushButton::clicked,
            this, &RequestManagementForm::onApproveClicked);
    connect(rejectBtn, &QPushButton::clicked,
            this, &RequestManagementForm::onRejectClicked);

    loadRequests();
}

RequestManagementForm::~RequestManagementForm() {}

void RequestManagementForm::loadRequests()
{
    requestsTable->setRowCount(0);
    QFile file("../Database/requests.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    int row = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if(parts.size() >= 4) {
            requestsTable->insertRow(row);
            for(int i = 0; i < 4; i++)
                requestsTable->setItem(row, i,
                    new QTableWidgetItem(parts[i].trimmed()));
            row++;
        }
    }
    file.close();
}

void RequestManagementForm::onApproveClicked()
{
    int row = requestsTable->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Warning", "Please select a request first!");
        return;
    }

    QString patientName = requestsTable->item(row, 0)->text();
    QString bloodGroup  = requestsTable->item(row, 2)->text();
    int units = requestsTable->item(row, 3)->text().toInt();

    updateInventory(bloodGroup, units);

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

void RequestManagementForm::updateInventory(QString bloodGroup, int units)
{
    Q_UNUSED(bloodGroup);
    Q_UNUSED(units);
}