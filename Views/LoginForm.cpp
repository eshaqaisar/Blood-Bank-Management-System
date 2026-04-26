#include "LoginForm.h"
#include "LandingPage.h"
#include "SignupForm.h"
#include "AdminDashboard.h"
#include "DonorDashboard.h"
#include "PatientDashboard.h"
#include "../Utilities/FileManager.h"
#include "../Models/User.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QFont>


// LoginForm.cpp  
// Part: Esha Qaisar


LoginForm::LoginForm(const QString& mode, QWidget* parent)
    : QWidget(parent), mode(mode)
{
    setupUI();
    applyStyle();
    refreshCaptcha(); // Generate first CAPTCHA on startup
}

LoginForm::~LoginForm() {}

void LoginForm::setupUI()
{
    setWindowTitle("Login - Blood Bank System");
    setMinimumSize(420, 520);

    lblTitle = new QLabel((mode == "Admin") ? "🔐 Admin Login" : "👤 User Login", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

    // Username 
    QLabel* lblUser = new QLabel("Username:", this);
    txtUsername = new QLineEdit(this);
    txtUsername->setPlaceholderText("Enter your username...");

    // Password
    QLabel* lblPass = new QLabel("Password:", this);
    txtPassword = new QLineEdit(this);
    txtPassword->setPlaceholderText("Enter password...");
    txtPassword->setEchoMode(QLineEdit::Password); // Shows dots instead of text

    //  Password Strength Bar
    lblStrengthText = new QLabel("Password Strength:", this);
    barPasswordStrength = new QProgressBar(this);
    barPasswordStrength->setRange(0, 4);
    barPasswordStrength->setValue(0);
    barPasswordStrength->setTextVisible(false); // Hide the "0/4" text

    // Update bar every time password changes
    connect(txtPassword, &QLineEdit::textChanged, this, &LoginForm::onPasswordChanged);

    // CAPTCHA 
    QLabel* lblCaptchaLabel = new QLabel("Enter CAPTCHA:", this);
    lblCaptchaDisplay = new QLabel("------", this);
    lblCaptchaDisplay->setObjectName("lblCaptcha");
    lblCaptchaDisplay->setAlignment(Qt::AlignCenter);

    btnRefreshCaptcha = new QPushButton("🔄 New", this);
    btnRefreshCaptcha->setMaximumWidth(70);
    connect(btnRefreshCaptcha, &QPushButton::clicked, this, &LoginForm::refreshCaptcha);

    txtCaptchaInput = new QLineEdit(this);
    txtCaptchaInput->setPlaceholderText("Type CAPTCHA here...");
    txtCaptchaInput->setMaxLength(5);

    // CAPTCHA row: label + refresh button side by side
    QHBoxLayout* captchaRow = new QHBoxLayout();
    captchaRow->addWidget(lblCaptchaDisplay);
    captchaRow->addWidget(btnRefreshCaptcha);

    // Status message
    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");

    // Buttons 
    btnLogin = new QPushButton("Login", this);
    btnSignup = new QPushButton("Create Account", this);
    btnBack = new QPushButton("← Back", this);

    connect(btnLogin, &QPushButton::clicked, this, &LoginForm::onLoginClicked);
    connect(btnSignup, &QPushButton::clicked, this, &LoginForm::onSignupClicked);
    connect(btnBack, &QPushButton::clicked, this, &LoginForm::onBackClicked);

    // Main Layout 
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(50, 40, 50, 40);
    layout->addWidget(lblTitle);
    layout->addSpacing(10);
    layout->addWidget(lblUser);
    layout->addWidget(txtUsername);
    layout->addWidget(lblPass);
    layout->addWidget(txtPassword);
    layout->addWidget(lblStrengthText);
    layout->addWidget(barPasswordStrength);
    layout->addWidget(lblCaptchaLabel);
    layout->addLayout(captchaRow);
    layout->addWidget(txtCaptchaInput);
    layout->addWidget(lblStatus);
    layout->addSpacing(10);
    layout->addWidget(btnLogin);
    layout->addWidget(btnSignup);
    layout->addWidget(btnBack);
    setLayout(layout);
}

//Generate a random alphanumeric CAPTCHA 
// This uses Qt's random number generator to pick characters.
QString LoginForm::generateCaptcha(int length)
{
    const QString chars = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"; // No confusing I/O/1/0
    QString result;
    for (int i = 0; i < length; i++)
    {
        // Pick a random index from the chars string
        int idx = QRandomGenerator::global()->bounded(chars.length());
        result += chars[idx];
    }
    return result;
}

//  Generate and display a new CAPTCHA 
void LoginForm::refreshCaptcha() {
    currentCaptcha = generateCaptcha(5);
    // Show in a styled way with spaces between characters for readability
    lblCaptchaDisplay->setText(currentCaptcha.split("").join(" "));
    txtCaptchaInput->clear();
}

// Update password strength bar as user types 
void LoginForm::onPasswordChanged(const QString& text)
{
    int strength = User::passwordStrength(text); // 0-4 from User class
    barPasswordStrength->setValue(strength);

    // Change bar color based on strength level
    QString color;
    if (strength <= 1) color = "#e74c3c"; // Red = weak
    else if (strength == 2) color = "#e67e22"; // Orange = medium
    else if (strength == 3) color = "#f1c40f"; // Yellow = good
    else                    color = "#27ae60"; // Green = strong

    barPasswordStrength->setStyleSheet(
        "QProgressBar::chunk { background-color: " + color + "; border-radius: 4px; }"
    );
}

//  Login button clicked
void LoginForm::onLoginClicked() {
    lblStatus->setText(""); // Clear previous errors

    // Step 1: Check CAPTCHA
    if (txtCaptchaInput->text().trimmed().toUpper() != currentCaptcha) {
        lblStatus->setText("❌ Wrong CAPTCHA. Try again.");
        lblStatus->setStyleSheet("color: red;");
        refreshCaptcha(); // Make them do it again
        return;
    }

    // Step 2: Check empty fields
    QString username = txtUsername->text().trimmed();
    QString password = txtPassword->text();
    if (username.isEmpty() || password.isEmpty())
    {
        lblStatus->setText("❌ Username and password cannot be empty.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }

    // Step 3: Find user in file and authenticate
    User user = FileManager::findUser(username);
    if (user.getUsername() == "NOT_FOUND")
    {
        lblStatus->setText("❌ Username not found.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }

    if (!user.authenticate(password))
    {
        lblStatus->setText("❌ Incorrect password.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }

    // Step 4: Check role matches the login mode
    if (mode == "Admin" && user.getRole() != "Admin")
    {
        lblStatus->setText("❌ Not an Admin account.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }

    // Step 5: All checks passed — navigate to correct dashboard
    lblStatus->setText("✅ Login successful!");
    lblStatus->setStyleSheet("color: green;");
    FileManager::logActivity("User logged in: " + username);
    navigateToDashboard(user.getRole(), username);
}

//Route to correct dashboard based on role 
void LoginForm::navigateToDashboard(const QString& role, const QString& username) {
    this->hide();
    if (role == "Admin") {
        AdminDashboard* dash = new AdminDashboard(username);
        dash->show();
    }
    else if (role == "Donor") {
        DonorDashboard* dash = new DonorDashboard(username);
        dash->show();
    }
    else if (role == "Patient") {
        PatientDashboard* dash = new PatientDashboard(username);
        dash->show();
    }
}

// Signup button
void LoginForm::onSignupClicked()
{
    SignupForm* signup = new SignupForm();
    signup->show();
    this->hide();
}

// Back button 
void LoginForm::onBackClicked()
{
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}

void LoginForm::applyStyle()
{
    setStyleSheet(R"(
        QWidget { background-color: #fff; font-family: Arial; font-size: 13px; }
        #lblTitle { font-size: 20px; font-weight: bold; color: #2c3e50; }
        #lblCaptcha {
            font-size: 22px; font-weight: bold; letter-spacing: 4px;
            background: #ecf0f1; border-radius: 6px; padding: 8px;
            color: #2c3e50;
        }
        #lblStatus { font-size: 12px; font-weight: bold; }
        QLineEdit {
            padding: 8px; border: 1px solid #bdc3c7;
            border-radius: 6px; font-size: 13px;
        }
        QLineEdit:focus { border: 2px solid #c0392b; }
        QPushButton {
            background-color: #c0392b; color: white;
            border-radius: 6px; padding: 10px; font-size: 14px; font-weight: bold;
        }
        QPushButton:hover { background-color: #e74c3c; }
        QProgressBar {
            border: 1px solid #bdc3c7; border-radius: 4px; height: 12px;
            background: #ecf0f1;
        }
    )");
}
