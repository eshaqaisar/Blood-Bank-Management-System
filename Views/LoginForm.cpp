#include "LoginForm.h"//header file for the LoginForm class
#include "LandingPage.h"//for the landing page to return to on back button
#include "SignupForm.h"//for the signup form to open when "Create Account" is clicked
#include "AdminDashboard.h"//for the admin dashboard to open on successful admin login
#include "DonorDashboard.h"//for the donor dashboard to open on successful user login
#include "PatientDashboard.h"//for the patient dashboard to open on successful user login
#include "../Utilities/FileManager.h"//for validating login credentials against stored user data in the file
#include "../Models/User.h"//for the User data structure and operations, including password strength calculation and user role management
#include <QMessageBox>//for showing message boxes to the user (e.g., for login errors or success messages)
#include <QRandomGenerator>//for generating random CAPTCHA strings
#include <QFont>//for customizing font styles in the UI, such as making the title bold and larger


LoginForm::LoginForm(const QString& mode, QWidget* parent)
    : QWidget(parent), mode(mode)
{
    setupUI();
    applyStyle();
	refreshCaptcha(); //generate the initial CAPTCHA when the form is created
}

LoginForm::~LoginForm() {}
//set up the user interface elements for the login form, including the title, input fields for username and password, password strength bar, CAPTCHA display and input, status message label, and action buttons. The layout is organized to provide a clean and user-friendly experience for logging in, with visual feedback on password strength and a CAPTCHA to enhance security.
void LoginForm::setupUI()
{
    setWindowTitle("Login - Blood Bank System");
    setMinimumSize(420, 520);

    lblTitle = new QLabel((mode == "Admin") ? "🔐 Admin Login" : "👤 User Login", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

	//username
    QLabel* lblUser = new QLabel("Username:", this);
    txtUsername = new QLineEdit(this);
    txtUsername->setPlaceholderText("Enter your username...");
	//password
    QLabel* lblPass = new QLabel("Password:", this);
    txtPassword = new QLineEdit(this);
    txtPassword->setPlaceholderText("Enter password...");
	txtPassword->setEchoMode(QLineEdit::Password); //hide password input for security

	//password strength bar
    lblStrengthText = new QLabel("Password Strength:", this);
    barPasswordStrength = new QProgressBar(this);
    barPasswordStrength->setRange(0, 4);
    barPasswordStrength->setValue(0);
	barPasswordStrength->setTextVisible(false); //hide the percentage text for a cleaner look

	//connect password input to strength checker
    connect(txtPassword, &QLineEdit::textChanged, this, &LoginForm::onPasswordChanged);

    //CAPTCHA 
    QLabel* lblCaptchaLabel = new QLabel("Enter CAPTCHA:", this);
    lblCaptchaDisplay = new QLabel("------", this);
    lblCaptchaDisplay->setObjectName("lblCaptcha");
    lblCaptchaDisplay->setAlignment(Qt::AlignCenter);
	//refresh CAPTCHA button next to the display label
    btnRefreshCaptcha = new QPushButton("🔄 New", this);
    btnRefreshCaptcha->setMaximumWidth(70);
    connect(btnRefreshCaptcha, &QPushButton::clicked, this, &LoginForm::refreshCaptcha);
	//CAPTCHA input field
    txtCaptchaInput = new QLineEdit(this);
    txtCaptchaInput->setPlaceholderText("Type CAPTCHA here...");
    txtCaptchaInput->setMaxLength(5);

	//layout for CAPTCHA display and refresh button
    QHBoxLayout* captchaRow = new QHBoxLayout();
    captchaRow->addWidget(lblCaptchaDisplay);
    captchaRow->addWidget(btnRefreshCaptcha);
	//status message label for showing login errors or success messages, centered and styled to stand out when providing feedback to the user
    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");

	//action buttons for the form: "Login" to submit the login credentials and attempt to log in, "Create Account" to open the signup form for new users, and "← Back" to return to the landing page. Each button is connected to its respective slot for handling the corresponding action when clicked.
    btnLogin = new QPushButton("Login", this);
    btnSignup = new QPushButton("Create Account", this);
    btnBack = new QPushButton("← Back", this);
	//connect buttons to their respective slots for handling clicks
    connect(btnLogin, &QPushButton::clicked, this, &LoginForm::onLoginClicked);
    connect(btnSignup, &QPushButton::clicked, this, &LoginForm::onSignupClicked);
    connect(btnBack, &QPushButton::clicked, this, &LoginForm::onBackClicked);

	//layout the login form using a vertical layout to stack the title, input fields, password strength bar, CAPTCHA, status message, and action buttons in a clean and organized manner. The layout includes spacing and margins for better visual separation between elements, creating a user-friendly experience for logging in.
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

//generate a random alphanumeric CAPTCHA 
//this uses Qt's random number generator to pick characters.
QString LoginForm::generateCaptcha(int length)
{
    const QString chars = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"; //no confusing I/O/1/0
    QString result;
    for (int i = 0; i < length; i++)
    {
		//pick a random index and append the character to the result
        int idx = QRandomGenerator::global()->bounded(chars.length());
        result += chars[idx];
    }
    return result;
}

//refresh the CAPTCHA by generating a new one and updating the display label. This method is called when the user clicks the "New" button next to the CAPTCHA display, allowing them to get a different CAPTCHA if they find the current one difficult to read. The new CAPTCHA is also stored in the currentCaptcha variable for later validation when the user attempts to log in.
void LoginForm::refreshCaptcha() {
    currentCaptcha = generateCaptcha(5);
	//display the CAPTCHA with spaces between characters for better readability
    lblCaptchaDisplay->setText(currentCaptcha.split("").join(" "));
    txtCaptchaInput->clear();
}
//slot that is called whenever the password input text changes, it calculates the strength of the entered password using the User class's passwordStrength method and updates the password strength bar accordingly. The color of the bar changes based on the strength level, providing visual feedback to the user about how strong their password is as they type it in. 
void LoginForm::onPasswordChanged(const QString& text)
{
    int strength = User::passwordStrength(text); // 0-4 from User class
    barPasswordStrength->setValue(strength);

	//set color based on strength level: red for weak, orange for medium, yellow for good, and green for strong
    QString color;
    if (strength <= 1) color = "#e74c3c"; // Red = weak
    else if (strength == 2) color = "#e67e22"; // Orange = medium
    else if (strength == 3) color = "#f1c40f"; // Yellow = good
    else                    color = "#27ae60"; // Green = strong

    barPasswordStrength->setStyleSheet(
        "QProgressBar::chunk { background-color: " + color + "; border-radius: 4px; }"
    );
}

//slot that is called when the "Login" button is clicked, it performs a series of checks to validate the login credentials and provide feedback to the user. The checks include validating the CAPTCHA input, ensuring that the username and password fields are not empty, verifying that the username exists in the file, authenticating the password against the stored hash, and confirming that the user's role matches the login mode (Admin or User). If all checks pass, it logs the successful login activity and navigates to the appropriate dashboard based on the user's role.
void LoginForm::onLoginClicked() {
	lblStatus->setText(""); //clear any previous status messages
	//validate CAPTCHA first to prevent brute-force attacks, the user must enter the correct CAPTCHA before any other checks are performed. This adds an extra layer of security to the login process by ensuring that automated bots cannot easily attempt to guess usernames and passwords without also solving the CAPTCHA.
    if (txtCaptchaInput->text().trimmed().toUpper() != currentCaptcha) {
        lblStatus->setText("❌ Wrong CAPTCHA. Try again.");
        lblStatus->setStyleSheet("color: red;");
		refreshCaptcha(); //refresh the CAPTCHA after a failed attempt to prevent repeated guessing of the same CAPTCHA
        return;
    }

	//validate that username and password fields are not empty, as these are required for logging in. If either field is empty, an error message is shown in the status label, and the login process is halted until the user provides the necessary information.
    QString username = txtUsername->text().trimmed();
    QString password = txtPassword->text();
    if (username.isEmpty() || password.isEmpty())
    {
        lblStatus->setText("❌ Username and password cannot be empty.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }

	//check if the username exists in the file and authenticate the password, the FileManager utility is used to find the user by username. If the user is not found, an error message is shown. If the user is found but the password does not match the stored hash, an error message is shown. This ensures that only valid users with correct credentials can log in.
    User user = FileManager::findUser(username);
    if (user.getUsername() == "NOT_FOUND")
    {
        lblStatus->setText("❌ Username not found.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }
	//authenticate the password using the User class's authenticate method, which likely compares the entered password against the stored hash for that user. If authentication fails, an error message is shown in the status label, and the login process is halted.
    if (!user.authenticate(password))
    {
        lblStatus->setText("❌ Incorrect password.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }

	//check if the user's role matches the login mode (Admin or User), if the login form is in Admin mode, only users with the Admin role are allowed to log in. If a user with a different role attempts to log in through the Admin form, an error message is shown. This ensures that only authorized users can access the admin dashboard, while regular users are directed to their respective dashboards based on their roles.
    if (mode == "Admin" && user.getRole() != "Admin")
    {
        lblStatus->setText("❌ Not an Admin account.");
        lblStatus->setStyleSheet("color: red;");
        return;
    }

	//if all checks pass, show a success message, log the login activity, and navigate to the appropriate dashboard based on the user's role. The success message is styled in green to indicate a successful login, and the FileManager utility is used to log the activity for auditing purposes. The navigateToDashboard method is called to open the correct dashboard for the user based on their role (Admin, Donor, or Patient).
    lblStatus->setText("✅ Login successful!");
    lblStatus->setStyleSheet("color: green;");
    FileManager::logActivity("User logged in: " + username);
    navigateToDashboard(user.getRole(), username);
}

//route to correct dashboard based on role 
void LoginForm::navigateToDashboard(const QString& role, const QString& username) {
    this->hide();
    if (role == "Admin") 
    {
        AdminDashboard* dash = new AdminDashboard(username);
        dash->show();
    }
    else if (role == "Donor")
    {
        DonorDashboard* dash = new DonorDashboard(username);
        dash->show();
    }
    else if (role == "Patient")
    {
        PatientDashboard* dash = new PatientDashboard(username);
        dash->show();
    }
}

//signup button
void LoginForm::onSignupClicked()
{
    SignupForm* signup = new SignupForm();
    signup->show();
    this->hide();
}

//back button 
void LoginForm::onBackClicked()
{
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}
//apply custom styles to the login form using a style sheet. This method sets the background color, font styles, and colors for various UI elements such as labels, input fields, buttons, and the password strength bar to create a cohesive and visually appealing design for the login form. It uses a combination of inline styles and object names to target specific elements like the title, CAPTCHA display, and status label for styling.
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
