#include "LandingPage.h"//header file for the LandingPage class
#include "LoginForm.h"//for the login form that will be opened when the user clicks the login buttons
#include <QApplication>//for qApp used in style sheet application
#include <QFont>//for customizing font styles in the UI, such as making the title bold and larger


LandingPage::LandingPage(QWidget* parent) : QWidget(parent) {
    setupUI();
    applyStyle();
}

LandingPage::~LandingPage() {}

//set up the user interface elements for the landing page, including the title, subtitle, and login buttons for admin and users. The layout is organized to provide a clean and welcoming experience for users when they first open the application. The title includes a blood drop emoji for visual appeal, and the buttons are styled to be prominent and inviting for users to click and proceed to the login forms.
void LandingPage::setupUI() {
    setWindowTitle("Blood Bank System - Welcome");
    setMinimumSize(500, 400);

	//title label for the landing page, styled to be larger and bold, with a blood drop emoji for visual appeal. It is centered at the top of the page to clearly indicate the purpose of the application and create an inviting first impression for users.
    lblTitle = new QLabel("🩸 Centralized Blood Bank System", this);
    QFont titleFont("Arial", 20, QFont::Bold);
    lblTitle->setFont(titleFont);
    lblTitle->setAlignment(Qt::AlignCenter);
	lblTitle->setObjectName("lblTitle"); //set an object name for styling purposes in the style sheet

	//subtitle label to provide a brief description of the application, styled to be smaller and less prominent than the title. It is centered below the title to complement the main heading and give users a quick understanding of the application's mission.
    lblSubtitle = new QLabel("Connecting Donors with Patients Since 2025", this);
    lblSubtitle->setAlignment(Qt::AlignCenter);
    lblSubtitle->setObjectName("lblSubtitle");

	//admin login button, styled to be prominent and inviting for admins to click and proceed to the admin login form. The button includes a lock emoji to visually indicate that it is for admin access, and it is connected to a slot that will open the login form when clicked.
    btnAdminLogin = new QPushButton("🔐  Login as Admin", this);
    btnAdminLogin->setMinimumHeight(50);
    btnAdminLogin->setObjectName("btnAdmin");

	//user login button, styled similarly to the admin button but with a different color scheme to visually differentiate it. The button includes a user emoji to indicate that it is for donors and patients to access their respective login forms, and it is connected to a slot that will open the login form when clicked.
    btnUserLogin = new QPushButton("👤  Login as Donor / Patient", this);
    btnUserLogin->setMinimumHeight(50);
    btnUserLogin->setObjectName("btnUser");

	//connect buttons to their respective slots for handling clicks
    connect(btnAdminLogin, &QPushButton::clicked, this, &LandingPage::onAdminLoginClicked);
    connect(btnUserLogin, &QPushButton::clicked, this, &LandingPage::onUserLoginClicked);

	//layout the landing page using a vertical layout to stack the title, subtitle, and login buttons in a clean and organized manner. The layout includes spacing and margins for better visual separation between elements, creating an inviting and user-friendly experience for users when they first open the application.
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
//apply custom styles to the landing page using a style sheet. This method sets the background color, font styles, and colors for various UI elements such as labels and buttons to create a cohesive and visually appealing design for the landing page. It uses a combination of inline styles and object names to target specific elements like the title, subtitle, and login buttons for styling.
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

//admin Login button clicked — open the login form for admins. The slot creates a new instance of the LoginForm class, passing "Admin" as an argument to indicate that the login form should expect admin credentials. The landing page is hidden while the login form is open to provide a focused experience for the user during the login process.
void LandingPage::onAdminLoginClicked()
{
	//as the login form is shared between admins and users, we pass a parameter to indicate which type of login is being attempted. The LoginForm will use this parameter to determine how to validate the credentials and which dashboard to show upon successful login.
    LoginForm* loginForm = new LoginForm("Admin");
    loginForm->show();
	this->hide(); //hide the landing page while the login form is open
}

//user Login button clicked — open the login form for users (donors and patients). The slot creates a new instance of the LoginForm class, passing "User" as an argument to indicate that the login form should expect user credentials. The landing page is hidden while the login form is open to provide a focused experience for the user during the login process. The LoginForm will determine whether the user is a donor or patient based on their credentials and direct them to the appropriate dashboard after successful login.
void LandingPage::onUserLoginClicked() {
	//as the login form is shared between admins and users, we pass a parameter to indicate which type of login is being attempted. The LoginForm will use this parameter to determine how to validate the credentials and which dashboard to show upon successful login.
    LoginForm* loginForm = new LoginForm("User");
    loginForm->show();
    this->hide();
}
