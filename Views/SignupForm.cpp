#include "SignupForm.h"
#include "LoginForm.h"
#include "../Utilities/FileManager.h"
#include "../Models/User.h"
#include "../Models/Donor.h"
#include "../Models/Patient.h"
#include <QMessageBox>
#include <QFormLayout>

// SignupForm.cpp
// Part: Esha Qaisar

SignupForm::SignupForm(QWidget* parent) : QWidget(parent)
{
    setupUI();
    applyStyle();
}
SignupForm::~SignupForm() {}

void SignupForm::setupUI()
{
    setWindowTitle("Create Account - Blood Bank System");
    setMinimumSize(440, 620);

    QLabel* lblTitle = new QLabel("📝 Create New Account", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

    txtName = new QLineEdit(this); txtName->setPlaceholderText("Full Name");
    txtAge = new QLineEdit(this); txtAge->setPlaceholderText("Age");
    txtContact = new QLineEdit(this); txtContact->setPlaceholderText("Phone Number");
    txtCity = new QLineEdit(this); txtCity->setPlaceholderText("City");
    txtUsername = new QLineEdit(this); txtUsername->setPlaceholderText("Choose a username");
    txtPassword = new QLineEdit(this); txtPassword->setPlaceholderText("Choose a password");
    txtPassword->setEchoMode(QLineEdit::Password);
    txtConfirmPassword = new QLineEdit(this); txtConfirmPassword->setPlaceholderText("Confirm password");
    txtConfirmPassword->setEchoMode(QLineEdit::Password);

    cmbBloodGroup = new QComboBox(this);
    cmbBloodGroup->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });

    cmbRole = new QComboBox(this);
    cmbRole->addItems({ "Donor", "Patient" });

    QLabel* lblStrLabel = new QLabel("Password Strength:", this);
    barStrength = new QProgressBar(this);
    barStrength->setRange(0, 4);
    barStrength->setValue(0);
    barStrength->setTextVisible(false);
    lblStrengthHint = new QLabel("Enter password to see strength", this);
    lblStrengthHint->setObjectName("lblHint");

    connect(txtPassword, &QLineEdit::textChanged, this, &SignupForm::onPasswordChanged);

    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");

    btnRegister = new QPushButton("✅  Register", this);
    btnBack = new QPushButton("← Back to Login", this);
    connect(btnRegister, &QPushButton::clicked, this, &SignupForm::onRegisterClicked);
    connect(btnBack, &QPushButton::clicked, this, &SignupForm::onBackClicked);

    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);
    form->addRow("Full Name:", txtName);
    form->addRow("Age:", txtAge);
    form->addRow("Contact:", txtContact);
    form->addRow("City:", txtCity);
    form->addRow("Blood Group:", cmbBloodGroup);
    form->addRow("Role:", cmbRole);
    form->addRow("Username:", txtUsername);
    form->addRow("Password:", txtPassword);
    form->addRow("Confirm PW:", txtConfirmPassword);
    form->addRow(lblStrLabel, barStrength);
    form->addRow("", lblStrengthHint);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->addWidget(lblTitle);
    layout->addSpacing(10);
    layout->addLayout(form);
    layout->addWidget(lblStatus);
    layout->addSpacing(10);
    layout->addWidget(btnRegister);
    layout->addWidget(btnBack);
    setLayout(layout);
}

void SignupForm::onPasswordChanged(const QString& text)
{
    int strength = User::passwordStrength(text);
    barStrength->setValue(strength);

    QStringList hints = {
        "Very Weak — too short",
        "Weak — add uppercase",
        "Fair — add a number",
        "Good — add a symbol",
        "Strong! 🔒"
    };
    lblStrengthHint->setText(hints[strength]);

    QString color;
    if (strength <= 1) color = "#e74c3c";
    else if (strength == 2) color = "#e67e22";
    else if (strength == 3) color = "#f1c40f";
    else                    color = "#27ae60";

    barStrength->setStyleSheet(
        "QProgressBar::chunk { background-color: " + color + "; border-radius: 4px; }");
}

void SignupForm::onRegisterClicked()
{
    lblStatus->setText("");

    if (txtName->text().trimmed().isEmpty() || txtUsername->text().trimmed().isEmpty()) {
        lblStatus->setText("❌ Name and Username are required.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (txtPassword->text() != txtConfirmPassword->text()) {
        lblStatus->setText("❌ Passwords do not match.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (txtPassword->text().length() < 6) {
        lblStatus->setText("❌ Password must be at least 6 characters.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (FileManager::userExists(txtUsername->text().trimmed())) {
        lblStatus->setText("❌ Username already taken. Choose another.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }

    User newUser(txtUsername->text().trimmed(), txtPassword->text(), cmbRole->currentText());
    FileManager::saveUser(newUser);

    QString name = txtName->text().trimmed();
    int     age = txtAge->text().toInt();
    QString contact = txtContact->text().trimmed();
    QString city = txtCity->text().trimmed();
    QString bg = cmbBloodGroup->currentText();

    if (cmbRole->currentText() == "Donor") {
        Donor donor(name, age, contact, city, bg, 60.0);
        FileManager::saveDonor(donor);
    }
    else {
        Patient patient(name, age, contact, city, bg, 1, "General Hospital");
        FileManager::savePatient(patient);
    }

    lblStatus->setText("✅ Account created successfully!");
    lblStatus->setStyleSheet("color: #27ae60;");
    QMessageBox::information(this, "Success", "Account registered! You can now login.");
    onBackClicked();
}

void SignupForm::onBackClicked()
{
    LoginForm* login = new LoginForm("User");
    login->show();
    this->close();
}

void SignupForm::applyStyle()
{
    // ✅ FIX: color:#2c3e50 added to QWidget — prevents white-text-on-white-bg
    //         min-height on inputs keeps them visible at any window size
    //         QFormLayout labels explicitly colored
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
        }
        #lblTitle {
            font-size: 18px;
            font-weight: bold;
            color: #2c3e50;
        }
        #lblHint   { color: #888888; font-size: 11px; }
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
        QLineEdit:focus  { border: 2px solid #c0392b; background-color: #fff9f9; }
        QLineEdit:disabled { background-color: #ecf0f1; color: #95a5a6; }
        QComboBox {
            background-color: #ffffff;
            color: #2c3e50;
            padding: 7px 10px;
            border: 1px solid #bdc3c7;
            border-radius: 5px;
            min-height: 34px;
        }
        QComboBox:focus { border: 2px solid #c0392b; }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            color: #2c3e50;
            selection-background-color: #fadbd8;
        }
        QPushButton {
            background-color: #c0392b;
            color: #ffffff;
            border-radius: 6px;
            padding: 10px;
            font-size: 13px;
            font-weight: bold;
            min-height: 36px;
        }
        QPushButton:hover { background-color: #e74c3c; }
        QProgressBar {
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            height: 12px;
            background: #ecf0f1;
            color: #2c3e50;
        }
    )");
}