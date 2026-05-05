#include "LoginForm.h"//header file for the LoginForm class
#include "LandingPage.h"//for navigating back to landing page when Back button is clicked
#include "SignupForm.h"//for opening signup form when Create Account is clicked
#include "AdminDashboard.h"//for opening admin dashboard on successful admin login
#include "DonorDashboard.h"//for opening donor dashboard on successful donor login
#include "PatientDashboard.h"//for opening patient dashboard on successful patient login
#include "../Utilities/FileManager.h"//for finding users and logging activity
#include "../Models/User.h"//for User::passwordStrength and user authentication
#include <QMessageBox>//for displaying error messages during login
#include <QRandomGenerator>//for generating random CAPTCHA strings
#include <QPainter>//for drawing the background image
#include <QScrollArea>//scroll area so labels never touch inputs
#include <QString>//used throughout for Qt UI elements (mode, captcha, labels)
#include <string>//std::string used for username/password/role passed to dashboards

//constructor for the LoginForm class, which initializes the login form with the specified mode (admin or user), sets up the user interface elements and layouts, applies custom styles, and generates an initial CAPTCHA for the login process
LoginForm::LoginForm(const QString& mode, QWidget* parent)
    : QWidget(parent), mode(mode)
{
    setupUI();
    applyStyle();
    refreshCaptcha();
}

LoginForm::~LoginForm() {}

//ui setup function that initializes the user interface elements and layouts for the login form
void LoginForm::setupUI()
{
    setWindowTitle("Login - Blood Bank System");
    setMinimumWidth(420);
    resize(460, 580);

    lblTitle = new QLabel(
        (mode == "Admin") ? "🔐 Admin Login" : "👤 User Login", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

    QLabel* lblUser = new QLabel("Username:", this);
    lblUser->setObjectName("fieldLabel");
    txtUsername = new QLineEdit(this);
    txtUsername->setPlaceholderText("Enter your username...");

    QLabel* lblPass = new QLabel("Password:", this);
    lblPass->setObjectName("fieldLabel");
    txtPassword = new QLineEdit(this);
    txtPassword->setPlaceholderText("Enter password...");
    txtPassword->setEchoMode(QLineEdit::Password);

    //password strength indicator
    lblStrengthText = new QLabel("Password Strength:", this);
    lblStrengthText->setObjectName("fieldLabel");
    barPasswordStrength = new QProgressBar(this);
    barPasswordStrength->setRange(0, 4);
    barPasswordStrength->setValue(0);
    barPasswordStrength->setTextVisible(false);
    barPasswordStrength->setFixedHeight(12);
    connect(txtPassword, &QLineEdit::textChanged, this, &LoginForm::onPasswordChanged);

    //CAPTCHA
    QLabel* lblCaptchaLabel = new QLabel("Enter CAPTCHA:", this);
    lblCaptchaLabel->setObjectName("fieldLabel");
    lblCaptchaDisplay = new QLabel("------", this);
    lblCaptchaDisplay->setObjectName("lblCaptcha");
    lblCaptchaDisplay->setAlignment(Qt::AlignCenter);

    btnRefreshCaptcha = new QPushButton("🔄 New", this);
    btnRefreshCaptcha->setFixedWidth(80);
    btnRefreshCaptcha->setFixedHeight(42);
    btnRefreshCaptcha->setObjectName("btnSmall");
    connect(btnRefreshCaptcha, &QPushButton::clicked, this, &LoginForm::refreshCaptcha);

    QHBoxLayout* captchaRow = new QHBoxLayout();
    captchaRow->setSpacing(8);
    captchaRow->addWidget(lblCaptchaDisplay, 1);
    captchaRow->addWidget(btnRefreshCaptcha, 0);

    txtCaptchaInput = new QLineEdit(this);
    txtCaptchaInput->setPlaceholderText("Type CAPTCHA here...");
    txtCaptchaInput->setMaxLength(5);

    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");
    lblStatus->setWordWrap(true);

    btnLogin = new QPushButton("Login", this);
    btnSignup = new QPushButton("Create Account", this);
    btnBack = new QPushButton("← Back", this);
    btnBack->setObjectName("btnSecondary");

    connect(btnLogin, &QPushButton::clicked, this, &LoginForm::onLoginClicked);
    connect(btnSignup, &QPushButton::clicked, this, &LoginForm::onSignupClicked);
    connect(btnBack, &QPushButton::clicked, this, &LoginForm::onBackClicked);

    //form layout
    QWidget* formWidget = new QWidget();
    formWidget->setStyleSheet("background: transparent;");
    QVBoxLayout* formLayout = new QVBoxLayout(formWidget);
    formLayout->setContentsMargins(50, 40, 50, 40);
    formLayout->setSpacing(4);

    //helper lambda to add a label and field with consistent spacing
    auto addField = [&](QLabel* lbl, QWidget* field) {
        formLayout->addWidget(lbl);
        formLayout->addSpacing(2);
        formLayout->addWidget(field);
        formLayout->addSpacing(10);
        };

    formLayout->addWidget(lblTitle);
    formLayout->addSpacing(16);

    addField(lblUser, txtUsername);
    addField(lblPass, txtPassword);

    //strength bar block
    formLayout->addWidget(lblStrengthText);
    formLayout->addSpacing(2);
    formLayout->addWidget(barPasswordStrength);
    formLayout->addSpacing(10);

    //CAPTCHA block
    formLayout->addWidget(lblCaptchaLabel);
    formLayout->addSpacing(2);
    formLayout->addLayout(captchaRow);
    formLayout->addSpacing(6);
    formLayout->addWidget(txtCaptchaInput);
    formLayout->addSpacing(10);

    formLayout->addWidget(lblStatus);
    formLayout->addSpacing(12);
    formLayout->addWidget(btnLogin);
    formLayout->addSpacing(4);
    formLayout->addWidget(btnSignup);
    formLayout->addSpacing(4);
    formLayout->addWidget(btnBack);
    formLayout->addSpacing(10);

    //scroll area to ensure form is usable on smaller screens
    QScrollArea* scroll = new QScrollArea(this);
    scroll->setWidget(formWidget);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setStyleSheet("background: transparent; border: none;");
    scroll->viewport()->setStyleSheet("background: transparent;");

    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addWidget(scroll);
    setLayout(rootLayout);
}

//background image
void LoginForm::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap bg(":/background.jpg");
    painter.drawPixmap(0, 0, width(), height(), bg);
    (void)event;
}

//CAPTCHA generation function that creates a random string of the specified length using a predefined set of characters (excluding easily confused ones) to enhance security during the login process
QString LoginForm::generateCaptcha(int length)
{
    const QString chars = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    QString result;
    for (int i = 0; i < length; i++)
        result += chars[QRandomGenerator::global()->bounded(chars.length())];
    return result;
}

void LoginForm::refreshCaptcha() {
    currentCaptcha = generateCaptcha(5);
    //split("").join("  ") adds spacing between characters for readability
    lblCaptchaDisplay->setText(currentCaptcha.split("").join("  "));
    txtCaptchaInput->clear();
}

//password strength evaluation: uses User::passwordStrength which now takes std::string
void LoginForm::onPasswordChanged(const QString& text)
{
    //convert QString to std::string at the User::passwordStrength boundary
    int strength = User::passwordStrength(text.toStdString());
    barPasswordStrength->setValue(strength);

    QString color;
    if (strength <= 1)     color = "#e74c3c";
    else if (strength == 2) color = "#e67e22";
    else if (strength == 3) color = "#f1c40f";
    else                    color = "#27ae60";

    barPasswordStrength->setStyleSheet(
        "QProgressBar::chunk { background-color: " + color + "; border-radius: 4px; }");
}

//login button click handler: validates CAPTCHA, authenticates user, routes to dashboard
void LoginForm::onLoginClicked() {
    lblStatus->setText("");

    //CAPTCHA check stays in Qt-land (currentCaptcha is QString)
    if (txtCaptchaInput->text().trimmed().toUpper() != currentCaptcha) {
        lblStatus->setText("❌ Wrong CAPTCHA. Try again.");
        lblStatus->setStyleSheet("color: #e74c3c;");
        refreshCaptcha();
        return;
    }

    //convert Qt input to std::string at the FileManager/User boundary
    std::string username = txtUsername->text().trimmed().toStdString();
    std::string password = txtPassword->text().toStdString();

    if (username.empty() || password.empty()) {
        lblStatus->setText("❌ Username and password cannot be empty.");
        lblStatus->setStyleSheet("color: #e74c3c;");
        return;
    }

    //FileManager::findUser now takes std::string
    User user = FileManager::findUser(username);
    if (user.getUsername() == "NOT_FOUND") {
        lblStatus->setText("❌ Username not found.");
        lblStatus->setStyleSheet("color: #e74c3c;");
        return;
    }
    //authenticate takes std::string
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
    //logActivity takes std::string
    FileManager::logActivity("User logged in: " + username);
    //pass std::string role and username to navigateToDashboard
    navigateToDashboard(user.getRole(), username);
}

//navigateToDashboard: takes std::string role and username.
//All three dashboard constructors now take std::string, so no conversion needed here.
void LoginForm::navigateToDashboard(const std::string& role, const std::string& username) {
    this->hide();
    if (role == "Admin") {
        AdminDashboard* d = new AdminDashboard(username);
        d->show();
    }
    else if (role == "Donor") {
        DonorDashboard* d = new DonorDashboard(username);
        d->show();
    }
    else if (role == "Patient") {
        PatientDashboard* d = new PatientDashboard(username);
        d->show();
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

//style application function that applies custom styles to the login form
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
        #fieldLabel {
            font-size: 13px;
            font-weight: 600;
            color: #2c3e50;
            background: transparent;
            margin-top: 4px;
        }
        #lblTitle {
            font-size: 20px;
            font-weight: bold;
            color: #8B0000;
            background: transparent;
        }
        #lblCaptcha {
            font-size: 20px;
            font-weight: bold;
            letter-spacing: 6px;
            background: rgba(255,255,255,200);
            border-radius: 6px;
            padding: 10px 6px;
            color: #2c3e50;
            border: 1px solid #bdc3c7;
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
            min-height: 36px;
        }
        QLineEdit:focus {
            border: 2px solid #8B0000;
            background-color: rgba(255,249,249,220);
        }
        QProgressBar {
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            background: rgba(255,255,255,150);
            height: 12px;
        }
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
        QPushButton:hover   { background-color: #c0392b; }
        QPushButton:pressed { background-color: #6B0000; }
        #btnSmall {
            background-color: rgba(100,100,100,180);
            color: #ffffff;
            border-radius: 6px;
            padding: 6px 8px;
            font-size: 12px;
            font-weight: bold;
            min-height: 0;
            border: none;
        }
        #btnSmall:hover { background-color: rgba(120,120,120,200); }
        #btnSecondary {
            background-color: rgba(236,240,241,200);
            color: #2c3e50;
            border: 1px solid #bdc3c7;
            border-radius: 6px;
            font-weight: normal;
        }
        #btnSecondary:hover { background-color: rgba(220,230,231,220); }
        QScrollBar:vertical {
            background: rgba(200,200,200,80);
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: rgba(150,150,150,180);
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover { background: rgba(139,0,0,180); }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical { height: 0; }
    )");
}