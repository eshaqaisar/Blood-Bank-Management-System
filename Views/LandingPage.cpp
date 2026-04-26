#include "LandingPage.h"
#include "LoginForm.h"
#include <QApplication>
#include <QFont>


// LandingPage.cpp  
// Part : Esha Qaisar


LandingPage::LandingPage(QWidget* parent) : QWidget(parent) {
    setupUI();
    applyStyle();
}

LandingPage::~LandingPage() {}

// Build the UI programmatically 
// All widgets are created, configured, and placed in a layout here.
void LandingPage::setupUI() {
    setWindowTitle("Blood Bank System - Welcome");
    setMinimumSize(500, 400);

    // Title label
    lblTitle = new QLabel("🩸 Centralized Blood Bank System", this);
    QFont titleFont("Arial", 20, QFont::Bold);
    lblTitle->setFont(titleFont);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle"); // Used in stylesheet

    // Subtitle
    lblSubtitle = new QLabel("Connecting Donors with Patients Since 2025", this);
    lblSubtitle->setAlignment(Qt::AlignCenter);
    lblSubtitle->setObjectName("lblSubtitle");

    // Admin Login Button
    btnAdminLogin = new QPushButton("🔐  Login as Admin", this);
    btnAdminLogin->setMinimumHeight(50);
    btnAdminLogin->setObjectName("btnAdmin");

    // User Login Button (Donor or Patient)
    btnUserLogin = new QPushButton("👤  Login as Donor / Patient", this);
    btnUserLogin->setMinimumHeight(50);
    btnUserLogin->setObjectName("btnUser");

    // SIGNAL/SLOT Connections
    // When button is clicked (SIGNAL), call the slot function.
    connect(btnAdminLogin, &QPushButton::clicked, this, &LandingPage::onAdminLoginClicked);
    connect(btnUserLogin, &QPushButton::clicked, this, &LandingPage::onUserLoginClicked);

    //  Layout 
    // QVBoxLayout stacks widgets vertically
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(60, 60, 60, 60);
    layout->addWidget(lblTitle);
    layout->addWidget(lblSubtitle);
    layout->addSpacing(30);
    layout->addWidget(btnAdminLogin);
    layout->addWidget(btnUserLogin);

    setLayout(layout);
}

void LandingPage::applyStyle() {
    setStyleSheet(R"(
        QWidget { background-color: #f5f5f5; }
        #lblTitle { color: #c0392b; font-size: 22px; font-weight: bold; }
        #lblSubtitle { color: #555; font-size: 13px; }
        #btnAdmin {
            background-color: #2c3e50; color: white;
            border-radius: 8px; font-size: 15px; font-weight: bold;
        }
        #btnAdmin:hover { background-color: #34495e; }
        #btnUser {
            background-color: #c0392b; color: white;
            border-radius: 8px; font-size: 15px; font-weight: bold;
        }
        #btnUser:hover { background-color: #e74c3c; }
    )");
}


// Admin Login button clicked 
void LandingPage::onAdminLoginClicked()
{
    // Open login form. Pass "Admin" so it knows which role to expect.
    LoginForm* loginForm = new LoginForm("Admin");
    loginForm->show();
    this->hide(); // Hide landing page while login is open
}

// User Login button clicked 
void LandingPage::onUserLoginClicked() {
    // "User" role — LoginForm will figure out if they're Donor or Patient
    LoginForm* loginForm = new LoginForm("User");
    loginForm->show();
    this->hide();
}
