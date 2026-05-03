#include "LandingPage.h"//header file for the LandingPage class, which defines the UI and behavior of the landing page of the application
#include "LoginForm.h"//header file for the LoginForm class, which defines the UI and behavior of the login form that is opened when users click the login buttons on the landing page
#include <QApplication>//for managing application-wide resources and settings, and for starting the event loop of the application
#include <QFont>//for setting custom fonts for the labels and buttons on the landing page
#include <QPainter>//for drawing the background image on the landing page


LandingPage::LandingPage(QWidget* parent) : QWidget(parent) {
    setupUI();
    applyStyle();
}

LandingPage::~LandingPage() {}
//ui setup function that initializes the user interface elements and layouts for the landing page, including the title, subtitle, and login buttons, and organizes them in a vertical layout with appropriate spacing and margins to create a visually appealing and welcoming interface for users when they first open the application
void LandingPage::setupUI() {
    setWindowTitle("Blood Bank System - Welcome");
    setMinimumSize(500, 400);
    resize(520, 440);   //default open size

    //title
    lblTitle = new QLabel("🩸 Centralized Blood Bank System", this);
    QFont titleFont("Arial", 20, QFont::Bold);
    lblTitle->setFont(titleFont);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");
    lblTitle->setWordWrap(true);

    //subtitle
    lblSubtitle = new QLabel("Connecting Donors with Patients Since 2025", this);
    lblSubtitle->setAlignment(Qt::AlignCenter);
    lblSubtitle->setObjectName("lblSubtitle");

    //admin login button
    btnAdminLogin = new QPushButton("🔐  Login as Admin", this);
    btnAdminLogin->setMinimumHeight(50);
    btnAdminLogin->setObjectName("btnAdmin");

    //user login button
    btnUserLogin = new QPushButton("👤  Login as Donor / Patient", this);
    btnUserLogin->setMinimumHeight(50);
    btnUserLogin->setObjectName("btnUser");

    connect(btnAdminLogin, &QPushButton::clicked, this, &LandingPage::onAdminLoginClicked);
    connect(btnUserLogin, &QPushButton::clicked, this, &LandingPage::onUserLoginClicked);
	//layout setup
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(60, 60, 60, 60);
    layout->addStretch(1);                  //absorbs extra space above
    layout->addWidget(lblTitle);
    layout->addWidget(lblSubtitle);
    layout->addSpacing(30);
    layout->addWidget(btnAdminLogin);
    layout->addWidget(btnUserLogin);
    layout->addStretch(1);                  //absorbs extra space below
    setLayout(layout);
}

//background image
void LandingPage::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap bg(":/background.jpg");
    painter.drawPixmap(0, 0, width(), height(), bg);
    (void)event;
}
//style application function that applies custom styles to the landing page, such as colors and fonts, to create a visually appealing and welcoming interface for users when they first open the application, including specific styles for the title, subtitle, and login buttons to enhance the overall user experience and make the interface more engaging
void LandingPage::applyStyle() {
    setStyleSheet(R"(
        QWidget {
            color: #2c3e50;
            font-family: Arial;
            font-size: 13px;
        }
        #lblTitle {
            color: #8B0000;
            font-size: 22px;
            font-weight: bold;
            background: transparent;
        }
        #lblSubtitle {
            color: #2c3e50;
            font-size: 13px;
            font-weight: bold;
            background: transparent;
        }
        #btnAdmin {
            background-color: #2c3e50;
            color: white;
            border-radius: 8px;
            font-size: 15px;
            font-weight: bold;
            border: none;
        }
        #btnAdmin:hover   { background-color: #34495e; }
        #btnAdmin:pressed { background-color: #1a252f; }
        #btnUser {
            background-color: #8B0000;
            color: white;
            border-radius: 8px;
            font-size: 15px;
            font-weight: bold;
            border: none;
        }
        #btnUser:hover   { background-color: #c0392b; }
        #btnUser:pressed { background-color: #6B0000; }
    )");
}
//slot that is called when the "Login as Admin" button is clicked, it will open the login form in admin mode by creating a new instance of the LoginForm class with the mode set to "Admin", showing the login form, and hiding the landing page to allow the user to focus on the login process without distractions
void LandingPage::onAdminLoginClicked()
{
    LoginForm* loginForm = new LoginForm("Admin");
    loginForm->show();
    this->hide();
}
//slot that is called when the "Login as Donor / Patient" button is clicked, it will open the login form in user mode by creating a new instance of the LoginForm class with the mode set to "User", showing the login form, and hiding the landing page to allow the user to focus on the login process without distractions
void LandingPage::onUserLoginClicked() {
    LoginForm* loginForm = new LoginForm("User");
    loginForm->show();
    this->hide();
}