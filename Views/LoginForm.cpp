#include "LoginForm.h"//header file for the LoginForm class, which defines the UI and behavior of the login form that is opened when users click the login buttons on the landing page
#include "LandingPage.h"//header file for the LandingPage class, which defines the UI and behavior of the landing page of the application, needed here to navigate back to it when the Back button is clicked
#include "SignupForm.h"//header file for the SignupForm class, which defines the UI and behavior of the signup form that is opened when users click the "Create Account" button on the login form
#include "AdminDashboard.h"//header file for the AdminDashboard class, which defines the UI and behavior of the admin dashboard that is opened when an admin user successfully logs in
#include "DonorDashboard.h"//header file for the DonorDashboard class, which defines the UI and behavior of the donor dashboard that is opened when a donor user successfully logs in
#include "PatientDashboard.h"//header file for the PatientDashboard class, which defines the UI and behavior of the patient dashboard that is opened when a patient user successfully logs in
#include "../Utilities/FileManager.h"//header file for the FileManager class, which provides utility functions for reading and writing user data to files, as well as logging user activities such as logins and signups, used here to authenticate users during login and to log successful logins
#include "../Models/User.h"//header file for the User class, which defines the data structure and methods for user objects in the application, including authentication and password strength evaluation, used here to authenticate users during login and to evaluate password strength as the user types
#include <QMessageBox>//for displaying error messages and status updates to the user during the login process, such as incorrect CAPTCHA, empty fields, or successful login notifications
#include <QRandomGenerator>//for generating random CAPTCHA strings to enhance security during the login process by preventing automated bots from easily accessing user accounts
#include <QFont>//for setting custom fonts for the labels and buttons on the login form to create a visually appealing and user-friendly interface that is consistent with the overall design of the application
#include <QPainter>//for drawing the background image on the login form to create a visually appealing and cohesive design that matches the landing page and enhances the user experience during the login process
#include <QScrollArea>//scroll area so labels never touch inputs

//constructor for the LoginForm class, which initializes the login form with the specified mode (admin or user), sets up the user interface elements and layouts, applies custom styles, and generates an initial CAPTCHA for the login process
LoginForm::LoginForm(const QString& mode, QWidget* parent)
    : QWidget(parent), mode(mode)
{
    setupUI();
    applyStyle();
    refreshCaptcha();
}

LoginForm::~LoginForm() {}
//ui setup function that initializes the user interface elements and layouts for the login form, including the title, input fields for username and password, password strength indicator, CAPTCHA display and input, status label for error messages, and buttons for login, signup, and navigation back to the landing page, organized in a scrollable layout to ensure proper spacing and usability even on smaller screens
void LoginForm::setupUI()
{
    setWindowTitle("Login - Blood Bank System");
	//increased minimum width to prevent layout from breaking on smaller screens; scroll area will handle overflow if resized smaller than this
    setMinimumWidth(420);
    resize(460, 580);

    //widgets
    lblTitle = new QLabel(
        (mode == "Admin") ? "🔐 Admin Login" : "👤 User Login", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");
	//explicit color set on labels so they are visible on any background (prevents white text on light backgrounds)
    QLabel* lblUser = new QLabel("Username:", this);
    lblUser->setObjectName("fieldLabel");
    txtUsername = new QLineEdit(this);
    txtUsername->setPlaceholderText("Enter your username...");
	//setting a fixed height on the input fields to ensure they are always large enough to be easily clickable and to maintain a consistent appearance across different screen sizes and resolutions
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
	//letters spaced out with split("").join("  ") for better readability; background and border added in stylesheet for better visibility on any background
    lblCaptchaDisplay = new QLabel("------", this);
    lblCaptchaDisplay->setObjectName("lblCaptcha");
    lblCaptchaDisplay->setAlignment(Qt::AlignCenter);

    btnRefreshCaptcha = new QPushButton("🔄 New", this);
    //fixed dimensions so the button never overflows the right edge
    btnRefreshCaptcha->setFixedWidth(80);
    btnRefreshCaptcha->setFixedHeight(42);
    btnRefreshCaptcha->setObjectName("btnSmall");
    connect(btnRefreshCaptcha, &QPushButton::clicked, this, &LoginForm::refreshCaptcha);

    //stretch=1 on display label so button stays fixed-size
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
	//we use a QVBoxLayout for the form to ensure that all labels and fields are properly spaced and aligned vertically, and we add spacing manually after each field to create a consistent gap between the label and the next label, since setSpacing() only controls the spacing between items in the layout, not between a label and the next label if they are separated by a field widget
    QVBoxLayout* formLayout = new QVBoxLayout(formWidget);
    formLayout->setContentsMargins(50, 40, 50, 40);
	//formLayout->setSpacing(12); //we will handle spacing manually to ensure consistent gaps between labels, fields, and buttons
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

	//scroll area to ensure form is usable on smaller screens and to prevent layout from breaking if resized too small; also keeps the form centered and looking good on larger screens by preventing labels from stretching all the way across
    QScrollArea* scroll = new QScrollArea(this);
    scroll->setWidget(formWidget);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setStyleSheet("background: transparent; border: none;");
	//setting the viewport's background to transparent ensures that the scroll area itself does not have a background color that could interfere with the visibility of the form's background image, allowing the design to remain cohesive and visually appealing regardless of the scroll position
    scroll->viewport()->setStyleSheet("background: transparent;");
	//root layout
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

//CAPTCHA generation function that creates a random string of the specified length using a predefined set of characters (excluding easily confused ones) to enhance security during the login process by preventing automated bots from easily accessing user accounts, and formats the CAPTCHA string with spaces for better readability on the login form
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
    lblCaptchaDisplay->setText(currentCaptcha.split("").join("  "));
    txtCaptchaInput->clear();
}

//password strength evaluation function that calculates the strength of the entered password using the User::passwordStrength method, updates the value of the password strength progress bar accordingly, and changes the color of the progress bar based on the strength level to provide visual feedback to the user about the security of their chosen password as they type it in
void LoginForm::onPasswordChanged(const QString& text)
{
    int strength = User::passwordStrength(text);
    barPasswordStrength->setValue(strength);

    QString color;
    if (strength <= 1) 
        color = "#e74c3c";
    else if (strength == 2)
        color = "#e67e22";
    else if (strength == 3) 
        color = "#f1c40f";
    else                   
        color = "#27ae60";

    barPasswordStrength->setStyleSheet(
        "QProgressBar::chunk { background-color: " + color + "; border-radius: 4px; }");
}

//login button click handler that validates the entered CAPTCHA, checks for empty username or password fields, authenticates the user using the FileManager and User classes, checks for admin role if in admin mode, and displays appropriate status messages to the user based on the outcome of each validation step, while also logging successful logins and navigating to the appropriate dashboard based on the user's role
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
    if (role == "Admin") { AdminDashboard* d = new AdminDashboard(username);   d->show(); }
    else if (role == "Donor") { DonorDashboard* d = new DonorDashboard(username);   d->show(); }
    else if (role == "Patient") { PatientDashboard* d = new PatientDashboard(username); d->show(); }
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

//style application function that applies custom styles to the login form, such as colors and fonts, to create a visually appealing and user-friendly interface that is consistent with the overall design of the application, including specific styles for the title, input fields, password strength indicator, CAPTCHA display, status messages, and buttons to enhance the overall user experience during the login process
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