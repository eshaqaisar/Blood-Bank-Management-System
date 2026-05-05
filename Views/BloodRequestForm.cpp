#include "BloodRequestForm.h"//include the header file for the BloodRequestForm class
#include "../Models/BloodRequest.h"//for BloodRequest class
#include "../Utilities/FileManager.h"//for FileManager::saveRequest()
#include <QDateTime>//for generating unique request IDs based on timestamp
#include <QScrollArea>//for wrapping the form in a scrollable area
#include <string>//for std::string used in BloodRequest constructor calls

BloodRequestForm::BloodRequestForm(const QString& username, QWidget* parent)
    : QWidget(parent), loggedInUsername(username)
{
    setWindowTitle("New Blood Request");
    setMinimumWidth(420);
    resize(460, 500);

    //form fields setup
    txtName = new QLineEdit(loggedInUsername, this);
    txtName->setReadOnly(true);
    txtName->setObjectName("readOnlyField");

    cmbBloodGroup = new QComboBox(this);
    cmbBloodGroup->addItem("A+");  cmbBloodGroup->addItem("A-");
    cmbBloodGroup->addItem("B+");  cmbBloodGroup->addItem("B-");
    cmbBloodGroup->addItem("O+");  cmbBloodGroup->addItem("O-");
    cmbBloodGroup->addItem("AB+"); cmbBloodGroup->addItem("AB-");

    txtHospital = new QLineEdit(this);
    txtHospital->setPlaceholderText("Enter hospital name...");

    txtUnits = new QLineEdit(this);
    txtUnits->setPlaceholderText("e.g. 2");

    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");
    lblStatus->setWordWrap(true);

    btnSubmit = new QPushButton("✅  Submit Request", this);
    btnCancel = new QPushButton("← Cancel", this);
    btnCancel->setObjectName("btnSecondary");

    connect(btnSubmit, &QPushButton::clicked, this, &BloodRequestForm::onSubmitClicked);
    connect(btnCancel, &QPushButton::clicked, this, &BloodRequestForm::onCancelClicked);

    QWidget* formWidget = new QWidget();
    QVBoxLayout* formLayout = new QVBoxLayout(formWidget);
    formLayout->setContentsMargins(36, 28, 36, 28);
    formLayout->setSpacing(4);

    auto addField = [&](const QString& labelText, QWidget* field) {
        QLabel* lbl = new QLabel(labelText, formWidget);
        lbl->setObjectName("fieldLabel");
        formLayout->addWidget(lbl);
        formLayout->addSpacing(2);
        formLayout->addWidget(field);
        formLayout->addSpacing(12);
        };

    QLabel* title = new QLabel("🩸 Submit Blood Request", formWidget);
    title->setObjectName("lblTitle");
    title->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(title);
    formLayout->addSpacing(18);

    addField("Patient Name:", txtName);
    addField("Blood Group Needed:", cmbBloodGroup);
    addField("Hospital:", txtHospital);
    addField("Units Required:", txtUnits);

    formLayout->addWidget(lblStatus);
    formLayout->addSpacing(12);
    formLayout->addWidget(btnSubmit);
    formLayout->addSpacing(5);
    formLayout->addWidget(btnCancel);
    formLayout->addSpacing(10);

    QScrollArea* scroll = new QScrollArea(this);
    scroll->setWidget(formWidget);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->addWidget(scroll);
    setLayout(root);

    applyStyle();
}

//slot for handling the submit button click - validates input, creates a BloodRequest object, saves it to file, and shows feedback to the user
void BloodRequestForm::onSubmitClicked()
{
    lblStatus->setText("");

    //convert Qt form values to std::string at the boundary for BloodRequest constructor
    std::string hospital = txtHospital->text().trimmed().toStdString();
    std::string bloodGroup = cmbBloodGroup->currentText().toStdString();
    std::string username = loggedInUsername.toStdString();
    int units = txtUnits->text().toInt();

    if (hospital.empty()) {
        lblStatus->setText("❌ Please enter the hospital name.");
        lblStatus->setStyleSheet("color:#e74c3c;");
        return;
    }
    if (units <= 0) {
        lblStatus->setText("❌ Units must be a positive number.");
        lblStatus->setStyleSheet("color:#e74c3c;");
        return;
    }

    //unique ID: timestamp-based so no two requests share the same ID
    //generate as std::string directly
    std::string requestId = "REQ-" + std::to_string(QDateTime::currentMSecsSinceEpoch());

    //BloodRequest constructor now takes std::string parameters
    BloodRequest request(requestId, username, hospital, bloodGroup, units);

    FileManager::saveRequest(request);

    lblStatus->setText("✅ Request submitted! ID: " + QString::fromStdString(requestId));
    lblStatus->setStyleSheet("color:#27ae60;");

    QMessageBox::information(this, "Success",
        "Blood request submitted successfully.\nRequest ID: " + QString::fromStdString(requestId));

    emit requestSubmitted();
    this->close();
}

void BloodRequestForm::onCancelClicked()
{
    this->close();
}

void BloodRequestForm::applyStyle()
{
    setStyleSheet(R"(
        QWidget      { background-color:#ffffff; color:#2c3e50;
                       font-family:Arial; font-size:13px; }
        QScrollArea  { background:#ffffff; border:none; }
        QScrollBar:vertical {
            background:#f0f0f0; width:8px; border-radius:4px;
        }
        QScrollBar::handle:vertical {
            background:#bdc3c7; border-radius:4px; min-height:20px;
        }
        QScrollBar::handle:vertical:hover { background:#c0392b; }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical { height:0; }
        QLabel        { color:#2c3e50; background:transparent; }
        #fieldLabel   { font-size:13px; font-weight:600; color:#2c3e50; margin-top:4px; }
        #lblTitle     { font-size:18px; font-weight:bold; color:#c0392b; }
        #lblStatus    { font-size:12px; font-weight:bold; }
        QLineEdit {
            background-color:#ffffff; color:#2c3e50;
            padding:9px 12px; border:1px solid #bdc3c7;
            border-radius:7px; font-size:13px; min-height:36px;
        }
        QLineEdit:focus         { border:2px solid #c0392b; background:#fff9f9; }
        #readOnlyField          { background:#ecf0f1; color:#7f8c8d; }
        QComboBox {
            background-color:#ffffff; color:#2c3e50;
            padding:8px 12px; border:1px solid #bdc3c7;
            border-radius:7px; font-size:13px; min-height:36px;
        }
        QComboBox:focus { border:2px solid #c0392b; }
        QComboBox QAbstractItemView {
            background:#ffffff; color:#2c3e50;
            selection-background-color:#fadbd8;
        }
        QPushButton {
            background-color:#c0392b; color:#ffffff;
            border-radius:8px; padding:11px;
            font-size:13px; font-weight:bold; min-height:38px; border:none;
        }
        QPushButton:hover   { background-color:#e74c3c; }
        QPushButton:pressed { background-color:#a93226; }
        #btnSecondary {
            background-color:#ecf0f1; color:#2c3e50;
            border:1px solid #bdc3c7; border-radius:8px; font-weight:normal;
        }
        #btnSecondary:hover { background-color:#dfe6e9; }
    )");
}