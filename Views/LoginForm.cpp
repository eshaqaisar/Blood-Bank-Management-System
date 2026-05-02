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
#include <QPainter> 

// LoginForm.cpp
// Part: Esha Qaisar

LoginForm::LoginForm(const QString& mode, QWidget* parent)
    : QWidget(parent), mode(mode)
{
    setupUI();
    applyStyle();
    refreshCaptcha();
}

LoginForm::~LoginForm() {}

void LoginForm::setupUI()
{
    setWindowTitle("Login - Blood Bank System");
    setMinimumSize(420, 520);

    lblTitle = new QLabel((mode == "Admin") ? "🔐 Admin Login" : "👤 User Login", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

    QLabel* lblUser = new QLabel("Username:", this);
    txtUsername = new QLineEdit(this);
    txtUsername->setPlaceholderText("Enter your username...");

    QLabel* lblPass = new QLabel("Password:", this);
    txtPassword = new QLineEdit(this);
    txtPassword->setPlaceholderText("Enter password...");
    txtPassword->setEchoMode(QLineEdit::Password);

    lblStrengthText = new QLabel("Password Strength:", this);
    barPasswordStrength = new QProgressBar(this);
    barPasswordStrength->setRange(0, 4);
    barPasswordStrength->setValue(0);
    barPasswordStrength->setTextVisible(false);
    connect(txtPassword, &QLineEdit::textChanged, this, &LoginForm::onPasswordChanged);

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

    QHBoxLayout* captchaRow = new QHBoxLayout();
    captchaRow->addWidget(lblCaptchaDisplay);
    captchaRow->addWidget(btnRefreshCaptcha);

    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");

    btnLogin = new QPushButton("Login", this);
    btnSignup = new QPushButton("Create Account", this);
    btnBack = new QPushButton("← Back", this);

    connect(btnLogin, &QPushButton::clicked, this, &LoginForm::onLoginClicked);
    connect(btnSignup, &QPushButton::clicked, this, &LoginForm::onSignupClicked);
    connect(btnBack, &QPushButton::clicked, this, &LoginForm::onBackClicked);

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

QString LoginForm::generateCaptcha(int length)
{
    const QString chars = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    QString result;
    for (int i = 0; i < length; i++) {
        int idx = QRandomGenerator::global()->bounded(chars.length());
        result += chars[idx];
    }
    return result;
}

void LoginForm::refreshCaptcha() {
    currentCaptcha = generateCaptcha(5);
    lblCaptchaDisplay->setText(currentCaptcha.split("").join(" "));
    txtCaptchaInput->clear();
}

void LoginForm::onPasswordChanged(const QString& text)
{
    int strength = User::passwordStrength(text);
    barPasswordStrength->setValue(strength);

    QString color;
    if (strength <= 1) color = "#e74c3c";
    else if (strength == 2) color = "#e67e22";
    else if (strength == 3) color = "#f1c40f";
    else                    color = "#27ae60";

    barPasswordStrength->setStyleSheet(
        "QProgressBar::chunk { background-color: " + color + "; border-radius: 4px; }"
    );
}

void LoginForm::onLoginClicked() {
    lblStatus->setText("");

    if (txtCaptchaInput->text().trimmed().toUpper() != currentCaptcha) {
        lblStatus->setText("❌ Wrong CAPTCHA. Try again.");
        lblStatus->setStyleSheet("color: #e74c3c;");
        refreshCaptcha();
        return;
    }

    QString username = txtUsername->text().trimmed();
    QString password = txtPassword->text();
    if (username.isEmpty() || password.isEmpty()) {
        lblStatus->setText("❌ Username and password cannot be empty.");
        lblStatus->setStyleSheet("color: #e74c3c;");
        return;
    }

    User user = FileManager::findUser(username);
    if (user.getUsername() == "NOT_FOUND") {
        lblStatus->setText("❌ Username not found.");
        lblStatus->setStyleSheet("color: #e74c3c;");
        return;
    }
    if (!user.authenticate(password)) {
        lblStatus->setText("❌ Incorrect password.");
        lblStatus->setStyleSheet("color: #e74c3c;");
        return;
    }
    if (mode == "Admin" && user.getRole() != "Admin") {
        lblStatus->setText("❌ Not an Admin account.");
        lblStatus->setStyleSheet("color: #e74c3c;");
        return;
    }

    lblStatus->setText("✅ Login successful!");
    lblStatus->setStyleSheet("color: #27ae60;");
    FileManager::logActivity("User logged in: " + username);
    navigateToDashboard(user.getRole(), username);
}

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

void LoginForm::onSignupClicked() {
    SignupForm* signup = new SignupForm();
    signup->show();
    this->hide();
}

void LoginForm::onBackClicked() {
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}

void LoginForm::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap bg(":/background.jpg");
    painter.drawPixmap(0, 0, width(), height(), bg);
    (void)event;
}


void LoginForm::applyStyle()
{
    setStyleSheet(R"(
        QWidget {
            color: #2c3e50;
            font-family: Arial;
            font-size: 13px;
        }
        QLabel {
            color: #2c3e50;
            background: transparent;
        }
        #lblTitle {
            font-size: 20px;
            font-weight: bold;
            color: #8B0000;
            background: transparent;
        }
        #lblCaptcha {
            font-size: 22px;
            font-weight: bold;
            letter-spacing: 4px;
            background: rgba(255,255,255,200);
            border-radius: 6px;
            padding: 8px;
            color: #2c3e50;
        }
        #lblStatus { 
            font-size: 12px; 
            font-weight: bold;
            background: transparent;
        }
        QLineEdit {
            background-color: rgba(255,255,255,180);
            color: #2c3e50;
            padding: 9px 12px;
            border: 1px solid #bdc3c7;
            border-radius: 6px;
            font-size: 13px;
            min-height: 34px;
        }
        QLineEdit:focus { border: 2px solid #c0392b; background-color: rgba(255,249,249,220); }
        QPushButton {
            background-color: #8B0000;
            color: #ffffff;
            border-radius: 6px;
            padding: 10px;
            font-size: 13px;
            font-weight: bold;
            min-height: 36px;
            border: none;
        }
        QPushButton:hover { background-color: #c0392b; }
        QPushButton:pressed { background-color: #6B0000; }
        QProgressBar {
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            height: 12px;
            background: rgba(255,255,255,150);
        }
    )");
}
