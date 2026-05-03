#include "DonorRegistrationForm.h"//header file for this class
#include "../Utilities/FileManager.h"//for saving/loading donors and inventory
#include "../Models/Donor.h"//for creating new donor objects
#include "../Models/BloodBag.h"//for creating new blood bag entries
#include <QMessageBox>//for showing success/error messages
#include <QScrollArea>//for making the form scrollable on smaller screens


DonorRegistrationForm::DonorRegistrationForm(QWidget* parent) : QWidget(parent)
{
    setupUI();
    applyStyle();
}

DonorRegistrationForm::~DonorRegistrationForm() {}
//sets up the user interface components and layout for the donor registration form
void DonorRegistrationForm::setupUI()
{
    setWindowTitle("Add New Donor");
    setMinimumWidth(420);
    resize(460, 540);

   
    txtName = new QLineEdit(this); txtName->setPlaceholderText("Full Name");
    txtAge = new QLineEdit(this); txtAge->setPlaceholderText("Age (18+)");
    txtContact = new QLineEdit(this); txtContact->setPlaceholderText("Phone Number");
    txtCity = new QLineEdit(this); txtCity->setPlaceholderText("City");
    txtWeight = new QLineEdit(this); txtWeight->setPlaceholderText("Weight in kg (50+)");
    txtUnits = new QLineEdit(this); txtUnits->setPlaceholderText("Units donating today (e.g. 1)");

    cmbBloodGroup = new QComboBox(this);
    cmbBloodGroup->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });

    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");
    lblStatus->setWordWrap(true);

    btnAdd = new QPushButton("✅  Add Donor", this);
    btnClear = new QPushButton("🗑️  Clear Fields", this);
    btnClear->setObjectName("btnSecondary");

    connect(btnAdd, &QPushButton::clicked, this, &DonorRegistrationForm::onAddDonorClicked);
    connect(btnClear, &QPushButton::clicked, this, &DonorRegistrationForm::onClearClicked);

    
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

    QLabel* title = new QLabel("👤 Register New Donor", formWidget);
    title->setObjectName("lblTitle");
    title->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(title);
    formLayout->addSpacing(18);

    addField("Name:", txtName);
    addField("Age:", txtAge);
    addField("Contact:", txtContact);
    addField("City:", txtCity);
    addField("Blood Group:", cmbBloodGroup);
    addField("Weight (kg):", txtWeight);
    addField("Units Today:", txtUnits);

    formLayout->addWidget(lblStatus);
    formLayout->addSpacing(12);

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->setSpacing(10);
    btnRow->addWidget(btnAdd);
    btnRow->addWidget(btnClear);
    formLayout->addLayout(btnRow);
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
}
//handles the logic when the "Add Donor" button is clicked, including validation, saving data, and updating inventory
void DonorRegistrationForm::onAddDonorClicked()
{
    lblStatus->setText("");

    QString name = txtName->text().trimmed();
    int     age = txtAge->text().toInt();
    double  weight = txtWeight->text().toDouble();
    int     units = txtUnits->text().toInt();

    if (name.isEmpty() || txtContact->text().trimmed().isEmpty()) {
        lblStatus->setText("❌ Name and Contact are required.");
        lblStatus->setStyleSheet("color:#e74c3c;"); return;
    }
    if (age < 18) {
        lblStatus->setText("❌ Donor must be at least 18 years old.");
        lblStatus->setStyleSheet("color:#e74c3c;"); return;
    }
    if (weight < 50.0) {
        lblStatus->setText("❌ Donor must weigh at least 50 kg.");
        lblStatus->setStyleSheet("color:#e74c3c;"); return;
    }
    if (units < 1) {
        lblStatus->setText("❌ Enter a valid number of units (minimum 1).");
        lblStatus->setStyleSheet("color:#e74c3c;"); return;
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

    lblStatus->setText("✅ Donor " + name + " added — " +
        QString::number(units) + " unit(s) added to inventory.");
    lblStatus->setStyleSheet("color:#27ae60;");
    clearFields();
    QMessageBox::information(this, "Success",
        "Donor registered and inventory updated successfully.");
}

void DonorRegistrationForm::clearFields()
{
    txtName->clear(); txtAge->clear(); txtContact->clear();
    txtCity->clear(); txtWeight->clear(); txtUnits->clear();
    cmbBloodGroup->setCurrentIndex(0);
    lblStatus->setText("");
}

void DonorRegistrationForm::onClearClicked() { clearFields(); }
//applies a custom stylesheet to the form and its components to create a modern and visually appealing design
void DonorRegistrationForm::applyStyle()
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
        QLineEdit:focus { border:2px solid #c0392b; background:#fff9f9; }

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