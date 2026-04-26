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

    // Input Fields 
    txtName = new QLineEdit(this); txtName->setPlaceholderText("Full Name");
    txtAge = new QLineEdit(this); txtAge->setPlaceholderText("Age");
    txtContact = new QLineEdit(this); txtContact->setPlaceholderText("Phone Number");
    txtCity = new QLineEdit(this); txtCity->setPlaceholderText("City");
    txtUsername = new QLineEdit(this); txtUsername->setPlaceholderText("Choose a username");
    txtPassword = new QLineEdit(this); txtPassword->setPlaceholderText("Choose a password");
    txtPassword->setEchoMode(QLineEdit::Password);
    txtConfirmPassword = new QLineEdit(this); txtConfirmPassword->setPlaceholderText("Confirm password");
    txtConfirmPassword->setEchoMode(QLineEdit::Password);

    // Blood Group Dropdown
    cmbBloodGroup = new QComboBox(this);
    cmbBloodGroup->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });

    // Role Dropdown 
    cmbRole = new QComboBox(this);
    cmbRole->addItems({ "Donor", "Patient" });

    // Password Strength Bar 
    QLabel* lblStrLabel = new QLabel("Password Strength:", this);
    barStrength = new QProgressBar(this);
    barStrength->setRange(0, 4);
    barStrength->setValue(0);
    barStrength->setTextVisible(false);
    lblStrengthHint = new QLabel("Enter password to see strength", this);
    lblStrengthHint->setObjectName("lblHint");

    // Live update when password changes
    connect(txtPassword, &QLineEdit::textChanged, this, &SignupForm::onPasswordChanged);

    // Status 
    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");

    // Buttons
    btnRegister = new QPushButton("✅  Register", this);
    btnBack = new QPushButton("← Back to Login", this);
    connect(btnRegister, &QPushButton::clicked, this, &SignupForm::onRegisterClicked);
    connect(btnBack, &QPushButton::clicked, this, &SignupForm::onBackClicked);

    // Form Layout (label + field pairs)
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

// SLOT: Update strength bar and hint text 
void SignupForm::onPasswordChanged(const QString& text) 
{
    int strength = User::passwordStrength(text);
    barStrength->setValue(strength);

    QStringList hints = 
    {
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

// SLOT: Register button clicked 
void SignupForm::onRegisterClicked()
{
    lblStatus->setText("");

    //Validation
    if (txtName->text().trimmed().isEmpty() || txtUsername->text().trimmed().isEmpty())
    {
        lblStatus->setText("❌ Name and Username are required.");
        lblStatus->setStyleSheet("color: red;"); return;
    }
    if (txtPassword->text() != txtConfirmPassword->text())
    {
        lblStatus->setText("❌ Passwords do not match.");
        lblStatus->setStyleSheet("color: red;"); return;
    }
    if (txtPassword->text().length() < 6)
    {
        lblStatus->setText("❌ Password must be at least 6 characters.");
        lblStatus->setStyleSheet("color: red;"); return;
    }
    if (FileManager::userExists(txtUsername->text().trimmed())) 
    {
        lblStatus->setText("❌ Username already taken. Choose another.");
        lblStatus->setStyleSheet("color: red;"); return;
    }

    // Create User account (for login) 
    User newUser(
        txtUsername->text().trimmed(),
        txtPassword->text(),
        cmbRole->currentText()  // "Donor" or "Patient"
    );
    FileManager::saveUser(newUser); // Save to users.txt

    // Create Donor or Patient profile (for the app) 
    QString name = txtName->text().trimmed();
    int     age = txtAge->text().toInt();
    QString contact = txtContact->text().trimmed();
    QString city = txtCity->text().trimmed();
    QString bg = cmbBloodGroup->currentText();

    if (cmbRole->currentText() == "Donor") 
    {
        Donor donor(name, age, contact, city, bg, 60.0); // Default 60kg weight
        FileManager::saveDonor(donor); // Save to donors.txt
    }
    else
    {
        Patient patient(name, age, contact, city, bg, 1, "General Hospital");
        FileManager::savePatient(patient); // Save to patients.txt
    }

    lblStatus->setText("✅ Account created successfully!");
    lblStatus->setStyleSheet("color: green;");
    QMessageBox::information(this, "Success", "Account registered! You can now login.");
    onBackClicked();
}

void SignupForm::onBackClicked() 

{
    LoginForm* login = new LoginForm("User");
    login->show();
    this->close();
}

void SignupForm::applyStyle() {
    setStyleSheet(R"(
        QWidget { background-color: #fff; font-family: Arial; font-size: 13px; }
        #lblTitle { font-size: 18px; font-weight: bold; color: #2c3e50; }
        #lblHint  { color: #888; font-size: 11px; }
        #lblStatus { font-size: 12px; font-weight: bold; }
        QLineEdit {
            padding: 7px; border: 1px solid #bdc3c7;
            border-radius: 5px;
        }
        QLineEdit:focus { border: 2px solid #c0392b; }
        QComboBox { padding: 6px; border: 1px solid #bdc3c7; border-radius: 5px; }
        QPushButton {
            background-color: #c0392b; color: white;
            border-radius: 6px; padding: 10px; font-size: 14px; font-weight: bold;
        }
        QPushButton:hover { background-color: #e74c3c; }
        QProgressBar { border: 1px solid #bdc3c7; border-radius: 4px; height: 12px; }
    )");
}
