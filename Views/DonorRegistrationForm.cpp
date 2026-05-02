#include "DonorRegistrationForm.h"
#include "../Utilities/FileManager.h"
#include "../Models/Donor.h"
#include "../Models/BloodBag.h"
#include <QMessageBox>

// DonorRegistrationForm.cpp  |  Domain 1: Muhammad Ali

DonorRegistrationForm::DonorRegistrationForm(QWidget* parent) : QWidget(parent) {
    setupUI();
    applyStyle();
}
DonorRegistrationForm::~DonorRegistrationForm() {}

void DonorRegistrationForm::setupUI() {
    setWindowTitle("Add New Donor");
    setMinimumWidth(420);

    txtName = new QLineEdit(this); txtName->setPlaceholderText("Full Name");
    txtAge = new QLineEdit(this); txtAge->setPlaceholderText("Age (must be 18+)");
    txtContact = new QLineEdit(this); txtContact->setPlaceholderText("Phone Number");
    txtCity = new QLineEdit(this); txtCity->setPlaceholderText("City");
    txtWeight = new QLineEdit(this); txtWeight->setPlaceholderText("Weight in kg (must be 50+)");
    txtUnits = new QLineEdit(this); txtUnits->setPlaceholderText("Units donating today (e.g. 1)");

    cmbBloodGroup = new QComboBox(this);
    cmbBloodGroup->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });

    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");

    btnAdd = new QPushButton("✅  Add Donor", this);
    btnClear = new QPushButton("🗑️  Clear Fields", this);

    connect(btnAdd, &QPushButton::clicked, this, &DonorRegistrationForm::onAddDonorClicked);
    connect(btnClear, &QPushButton::clicked, this, &DonorRegistrationForm::onClearClicked);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);
    form->addRow("Name:", txtName);
    form->addRow("Age:", txtAge);
    form->addRow("Contact:", txtContact);
    form->addRow("City:", txtCity);
    form->addRow("Blood Group:", cmbBloodGroup);
    form->addRow("Weight (kg):", txtWeight);
    form->addRow("Units Today:", txtUnits);

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAdd);
    btnRow->addWidget(btnClear);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 25, 30, 25);
    layout->setSpacing(12);
    layout->addWidget(new QLabel("👤 Register New Donor", this));
    layout->addLayout(form);
    layout->addWidget(lblStatus);
    layout->addLayout(btnRow);
    setLayout(layout);
}

void DonorRegistrationForm::onAddDonorClicked() {
    lblStatus->setText("");

    QString name = txtName->text().trimmed();
    int     age = txtAge->text().toInt();
    double  weight = txtWeight->text().toDouble();
    int     units = txtUnits->text().toInt();

    if (name.isEmpty() || txtContact->text().isEmpty()) {
        lblStatus->setText("❌ Name and Contact are required.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (age < 18) {
        lblStatus->setText("❌ Donor must be at least 18 years old.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (weight < 50.0) {
        lblStatus->setText("❌ Donor must weigh at least 50 kg.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (units < 1) {
        lblStatus->setText("❌ Enter a valid number of units (minimum 1).");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }

    Donor newDonor(
        name, age,
        txtContact->text().trimmed(),
        txtCity->text().trimmed(),
        cmbBloodGroup->currentText(),
        weight,
        QDate::currentDate()
    );
    FileManager::saveDonor(newDonor);

    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    BloodBag bag(cmbBloodGroup->currentText(), units, QDate::currentDate(), name);
    inventory.addBag(bag);
    inventory.save(FileManager::INVENTORY_FILE);

    lblStatus->setText("✅ Donor " + name + " added and " +
        QString::number(units) + " unit(s) added to inventory.");
    lblStatus->setStyleSheet("color: #27ae60;");
    clearFields();
}

void DonorRegistrationForm::clearFields() {
    txtName->clear(); txtAge->clear(); txtContact->clear();
    txtCity->clear(); txtWeight->clear(); txtUnits->clear();
    cmbBloodGroup->setCurrentIndex(0);
}

void DonorRegistrationForm::onClearClicked() { clearFields(); }

void DonorRegistrationForm::applyStyle() {
    // ✅ FIX: explicit color:#2c3e50 everywhere — no white-on-white
    //         min-height on all inputs
    setStyleSheet(R"(
        QWidget {
            background-color: #ffffff;
            color: #2c3e50;
            font-family: Arial;
            font-size: 13px;
        }
        QLabel {
            color: #2c3e50;
            background: transparent;
            font-weight: bold;
            font-size: 13px;
        }
        #lblStatus { font-size: 12px; font-weight: bold; }
        QLineEdit {
            background-color: #ffffff;
            color: #2c3e50;
            padding: 8px 12px;
            border: 1px solid #bdc3c7;
            border-radius: 5px;
            font-size: 13px;
            min-height: 34px;
        }
        QLineEdit:focus { border: 2px solid #c0392b; background: #fff9f9; }
        QComboBox {
            background-color: #ffffff;
            color: #2c3e50;
            padding: 7px 10px;
            border: 1px solid #bdc3c7;
            border-radius: 5px;
            min-height: 34px;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            color: #2c3e50;
            selection-background-color: #fadbd8;
        }
        QPushButton {
            background-color: #c0392b;
            color: #ffffff;
            border-radius: 6px;
            padding: 9px;
            font-size: 13px;
            font-weight: bold;
            min-height: 36px;
        }
        QPushButton:hover { background-color: #e74c3c; }
    )");
}