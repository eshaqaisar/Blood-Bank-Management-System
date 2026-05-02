#include "BloodRequestForm.h"//include the header file for the BloodRequestForm class
#include "../Models/BloodRequest.h"//for BloodRequest class
#include "../Utilities/FileManager.h"//for FileManager::saveRequest()
#include <QDateTime>//for QDateTime used in generating unique request IDs


BloodRequestForm::BloodRequestForm(const QString& username, QWidget* parent)
    : QWidget(parent), loggedInUsername(username)
{
	//basic window setup
    setWindowTitle("New Blood Request");
    setMinimumWidth(420);
	//create form fields and labels
    lblName = new QLabel("Patient Name:");
    lblBloodGroup = new QLabel("Blood Group Needed:");
    lblHospital = new QLabel("Hospital:");
    lblUnits = new QLabel("Units Required:");

    //pre-fill with the logged-in username and make it read-only
    txtName = new QLineEdit(loggedInUsername);
    txtName->setReadOnly(true);
    txtName->setStyleSheet("background: #f0f0f0; color: #555;");
	//placeholders for the other fields
    txtHospital = new QLineEdit();
    txtHospital->setPlaceholderText("Enter hospital name...");
	//only allow numbers for units
    txtUnits = new QLineEdit();
    txtUnits->setPlaceholderText("e.g. 2");
	//set input mask to allow only digits
    cmbBloodGroup = new QComboBox();
    cmbBloodGroup->addItems({ "A+", "A-", "B+", "B-", "O+", "O-", "AB+", "AB-" });
	//status label for showing validation errors or success messages
    lblStatus = new QLabel("");
    lblStatus->setAlignment(Qt::AlignCenter);
	//submit and cancel buttons
    btnSubmit = new QPushButton("✅ Submit Request");
    btnCancel = new QPushButton("← Cancel");
	//arrange the form fields in a neat layout
    QFormLayout* form = new QFormLayout();
    form->setSpacing(12);
    form->addRow(lblName, txtName);
    form->addRow(lblBloodGroup, cmbBloodGroup);
    form->addRow(lblHospital, txtHospital);
    form->addRow(lblUnits, txtUnits);
	//main layout for the form
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 25, 30, 25);
    layout->setSpacing(10);
    layout->addWidget(new QLabel("🩸 Submit Blood Request", this));
    layout->addLayout(form);
    layout->addWidget(lblStatus);
    layout->addWidget(btnSubmit);
    layout->addWidget(btnCancel);
    setLayout(layout);
	//simple styling for a cleaner look
    setStyleSheet(R"(
        QWidget { background: #fff; font-family: Arial; font-size: 13px; }
        QLabel  { color: #2c3e50; }
        QLineEdit, QComboBox {
            padding: 7px; border: 1px solid #bdc3c7; border-radius: 5px;
        }
        QLineEdit:focus { border: 2px solid #c0392b; }
        QPushButton {
            background: #c0392b; color: white;
            border-radius: 6px; padding: 9px; font-size: 13px; font-weight: bold;
        }
        QPushButton:hover { background: #e74c3c; }
    )");

    connect(btnSubmit, &QPushButton::clicked, this, &BloodRequestForm::onSubmitClicked);
    connect(btnCancel, &QPushButton::clicked, this, &BloodRequestForm::onCancelClicked);
}

void BloodRequestForm::onSubmitClicked()
{
    lblStatus->setText("");

    //validate all fields
    QString hospital = txtHospital->text().trimmed();
    int     units = txtUnits->text().toInt();

    if (hospital.isEmpty()) {
        lblStatus->setText("❌ Please enter the hospital name.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }
    if (units <= 0) {
        lblStatus->setText("❌ Units must be a positive number.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }

    //generate a unique request ID using timestamp so no two requests share "R001"
    QString requestId = "REQ-" + QString::number(QDateTime::currentMSecsSinceEpoch());

    BloodRequest request(
        requestId,
        loggedInUsername,               // patient name = logged-in username
        hospital,
        cmbBloodGroup->currentText(),
        units
    );

    //use FileManager::saveRequest() instead of the old hardcoded saveToFile()
    FileManager::saveRequest(request);

    lblStatus->setText("✅ Request submitted successfully!");
    lblStatus->setStyleSheet("color: green;");

    QMessageBox::information(this, "Success",
        "Your blood request has been submitted.\nRequest ID: " + requestId);

    emit requestSubmitted(); //notify PatientDashboard to refresh its table
    this->close();
}

void BloodRequestForm::onCancelClicked()
{
    this->close();
}