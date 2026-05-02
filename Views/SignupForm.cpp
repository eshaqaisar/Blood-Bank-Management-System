#include "SignupForm.h"//header file for the SignupForm class
#include "LoginForm.h"//for returning to the login form when the back button is clicked
#include "../Utilities/FileManager.h"//for saving new user data to the file and validating input during registration
#include "../Models/User.h"//for the User data structure and operations, including creating a new user object with the provided information and calculating password strength
#include "../Models/Donor.h"//for the Donor data structure and operations, including creating a new donor object if the user registers as a donor
#include "../Models/Patient.h"//for the Patient data structure and operations, including creating a new patient object if the user registers as a patient
#include <QMessageBox>//for showing message boxes to the user (e.g., for input validation errors or success messages)
#include <QFormLayout>//for organizing the input fields in a clean and structured manner


SignupForm::SignupForm(QWidget* parent) : QWidget(parent) 
{
    setupUI();
    applyStyle();
}
SignupForm::~SignupForm() {}
//set up the user interface elements for the signup form, including input fields for name, age, contact information, city, blood group, role (donor or patient), username, password, and confirm password. The form also includes a password strength bar with live feedback on password strength, a status label for displaying validation messages, and action buttons for registering and returning to the login form. The layout is organized to provide a clean and user-friendly experience for creating a new account.
void SignupForm::setupUI()
{
    setWindowTitle("Create Account - Blood Bank System");
    setMinimumSize(440, 620);
	//title label for the signup form, styled to be larger and bold, with a notepad emoji for visual appeal. It is centered at the top of the form to clearly indicate the purpose of the page and create an inviting experience for users when they are creating a new account.
    QLabel* lblTitle = new QLabel("📝 Create New Account", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

	//input fields for user information, with placeholder text to guide the user on what to enter. The password fields are set to hide the input for security, and the confirm password field allows users to verify their password entry before submitting the form.
    txtName = new QLineEdit(this); txtName->setPlaceholderText("Full Name");
    txtAge = new QLineEdit(this); txtAge->setPlaceholderText("Age");
    txtContact = new QLineEdit(this); txtContact->setPlaceholderText("Phone Number");
    txtCity = new QLineEdit(this); txtCity->setPlaceholderText("City");
    txtUsername = new QLineEdit(this); txtUsername->setPlaceholderText("Choose a username");
    txtPassword = new QLineEdit(this); txtPassword->setPlaceholderText("Choose a password");
    txtPassword->setEchoMode(QLineEdit::Password);
    txtConfirmPassword = new QLineEdit(this); txtConfirmPassword->setPlaceholderText("Confirm password");
    txtConfirmPassword->setEchoMode(QLineEdit::Password);

	//blood group dropdown with common blood types as options, allowing users to select their blood group during registration. This information can be used for matching donors and patients in the system.
    cmbBloodGroup = new QComboBox(this);
    cmbBloodGroup->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });

	//role dropdown to select whether the user is registering as a donor or a patient. This information is important for determining the type of profile to create for the user and how they will interact with the system.
    cmbRole = new QComboBox(this);
    cmbRole->addItems({ "Donor", "Patient" });
	//password strength bar with a label and live feedback on password strength as the user types. The strength is calculated based on criteria such as length, use of uppercase letters, numbers, and symbols. The strength bar provides a visual indication of how strong the password is, and the hint label gives specific feedback on how to improve the password if it is weak.
    QLabel* lblStrLabel = new QLabel("Password Strength:", this);
    barStrength = new QProgressBar(this);
    barStrength->setRange(0, 4);
    barStrength->setValue(0);
    barStrength->setTextVisible(false);
    lblStrengthHint = new QLabel("Enter password to see strength", this);
    lblStrengthHint->setObjectName("lblHint");

	//connect password input to strength checker
    connect(txtPassword, &QLineEdit::textChanged, this, &SignupForm::onPasswordChanged);

	//status label for displaying validation messages or success messages after attempting to register. It is centered and styled to stand out when showing feedback to the user.
    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");

    //buttons
    btnRegister = new QPushButton("✅  Register", this);
    btnBack = new QPushButton("← Back to Login", this);
    connect(btnRegister, &QPushButton::clicked, this, &SignupForm::onRegisterClicked);
    connect(btnBack, &QPushButton::clicked, this, &SignupForm::onBackClicked);

    //form layout (label + field pairs)
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

//update strength bar and hint text 
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
    if (strength <= 1) 
        color = "#e74c3c";
    else if (strength == 2) 
        color = "#e67e22";
    else if (strength == 3) 
        color = "#f1c40f";
    else                   
        color = "#27ae60";

    barStrength->setStyleSheet(
        "QProgressBar::chunk { background-color: " + color + "; border-radius: 4px; }");
}

//register button clicked 
void SignupForm::onRegisterClicked()
{
    lblStatus->setText("");

    //validation
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

    //create User account (for login) 
    User newUser(
        txtUsername->text().trimmed(),
        txtPassword->text(),
        cmbRole->currentText()  // "Donor" or "Patient"
    );
    FileManager::saveUser(newUser); //save to users.txt

    //create Donor or Patient profile (for the app) 
    QString name = txtName->text().trimmed();
    int     age = txtAge->text().toInt();
    QString contact = txtContact->text().trimmed();
    QString city = txtCity->text().trimmed();
    QString bg = cmbBloodGroup->currentText();

    if (cmbRole->currentText() == "Donor") 
    {
        Donor donor(name, age, contact, city, bg, 60.0); //default 60kg weight
        FileManager::saveDonor(donor); //save to donors.txt
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
//back button clicked - return to login form
void SignupForm::onBackClicked() 

{
    LoginForm* login = new LoginForm("User");
    login->show();
    this->close();
}
//apply custom styles to the signup form using a style sheet. This method sets the background color, font styles, and colors for various UI elements such as labels, input fields, combo boxes, buttons, and the password strength bar to create a cohesive and visually appealing design for the signup form. It uses a combination of inline styles and object names to target specific elements like the title label, hint label, and status label for styling.
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
