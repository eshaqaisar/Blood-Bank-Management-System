#include "SignupForm.h"//for the class definition and UI element declarations
#include "LoginForm.h"//to navigate back to the login form after successful registration or when the user clicks the "← Back" button
#include "../Utilities/FileManager.h"//for checking if a username already exists and for saving the new user, donor, or patient data to files after successful registration
#include "../Models/User.h"//for creating a new User object with the provided username, password, and role, and for using the static method to evaluate password strength
#include "../Models/Donor.h"//for creating a new Donor object with the provided name, age, contact, city, blood group, and default donation count when registering as a donor
#include "../Models/Patient.h"//for creating a new Patient object with the provided name, age, contact, city, blood group, default required units, and hospital when registering as a patient
#include <QMessageBox>//for showing a message box to the user after successful registration, confirming that their account has been created and they can now login
#include <QFormLayout>//for organizing the input fields and their labels in a clean, structured manner within the form


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
	//labels with emojis to make the form more visually appealing and engaging for users, while still keeping the text clear and concise. The title label includes a pencil emoji to indicate that this is a form for creating a new account, and the register button includes a checkmark emoji to signify successful registration. The back button includes a left arrow emoji to indicate navigation back to the login form. These small visual cues can enhance the user experience and make the interface more inviting.
    QLabel* lblTitle = new QLabel("📝 Create New Account", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");
	//add emojis to labels and buttons for a more engaging and friendly UI, while keeping the text clear and concise. Emojis can help convey the purpose of the form and actions in a visually appealing way, making the interface more inviting for users.
    txtName = new QLineEdit(this); txtName->setPlaceholderText("Full Name");
    txtAge = new QLineEdit(this); txtAge->setPlaceholderText("Age");
    txtContact = new QLineEdit(this); txtContact->setPlaceholderText("Phone Number");
    txtCity = new QLineEdit(this); txtCity->setPlaceholderText("City");
    txtUsername = new QLineEdit(this); txtUsername->setPlaceholderText("Choose a username");
    txtPassword = new QLineEdit(this); txtPassword->setPlaceholderText("Choose a password");
    txtPassword->setEchoMode(QLineEdit::Password);
    txtConfirmPassword = new QLineEdit(this); txtConfirmPassword->setPlaceholderText("Confirm password");
    txtConfirmPassword->setEchoMode(QLineEdit::Password);
	//the blood group combo box allows users to select their blood type from a predefined list, which is important for both donors and patients in the blood bank system. This ensures that the data collected is consistent and valid, as users can only choose from the standard blood groups (A+, A-, B+, B-, AB+, AB-, O+, O-). This also simplifies the registration process for users, as they don't have to manually enter their blood group, reducing the chances of typos or invalid entries.
    cmbBloodGroup = new QComboBox(this);
    cmbBloodGroup->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });

    cmbRole = new QComboBox(this);
    cmbRole->addItems({ "Donor", "Patient" });

    QLabel* lblStrLabel = new QLabel("Password Strength:", this);
    barStrength = new QProgressBar(this);
    barStrength->setRange(0, 4);
    barStrength->setValue(0);
    barStrength->setTextVisible(false);
    lblStrengthHint = new QLabel("Enter password to see strength", this);
    lblStrengthHint->setObjectName("lblHint");

    connect(txtPassword, &QLineEdit::textChanged, this, &SignupForm::onPasswordChanged);
	//the status label is used to display validation messages or success messages after the user attempts to register. It is centered and styled to stand out, making it easy for users to see feedback about their registration attempt. For example, if the user leaves required fields empty, enters mismatched passwords, or chooses a username that already exists, the status label will show an appropriate error message in red. If the registration is successful, it will show a success message in green. This immediate feedback helps guide users through the registration process and improves the overall user experience.
    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");
	
    btnRegister = new QPushButton("✅  Register", this);
    btnBack = new QPushButton("← Back to Login", this);
    connect(btnRegister, &QPushButton::clicked, this, &SignupForm::onRegisterClicked);
    connect(btnBack, &QPushButton::clicked, this, &SignupForm::onBackClicked);
	//the form layout organizes the input fields and their labels in a clean, structured manner. Each row consists of a label (e.g., "Full Name:") and its corresponding input field (e.g., txtName). This layout makes it easy for users to understand which information they need to enter in each field, improving the overall usability of the form. The spacing between rows is set to 10 pixels to ensure that the form doesn't look too cramped, while still keeping related fields visually grouped together.
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
	//the main layout of the form is a vertical box layout that organizes the title, form, status label, and buttons in a vertical manner. The contents margins are set to 40 pixels on the left and right, and 30 pixels on the top and bottom, to create a visually balanced and spacious layout. The title is placed at the top, followed by the form with input fields, then the status label for feedback messages, and finally the action buttons at the bottom. This layout ensures that the form is easy to navigate and visually appealing for users.
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
//for the password strength indicator, we use the static method User::passwordStrength to evaluate the strength of the entered password. The strength is represented as an integer value from 0 to 4, which corresponds to different levels of password strength (e.g., very weak, weak, fair, good, strong). Based on the strength value, we update the progress bar's value and color to visually indicate the strength level to the user. Additionally, we display a hint message next to the progress bar that provides feedback on how to improve the password strength (e.g., "add uppercase", "add a number", "add a symbol"). This real-time feedback helps users create stronger passwords and enhances the security of their accounts.
void SignupForm::onPasswordChanged(const QString& text)
{
    int strength = User::passwordStrength(text);
    barStrength->setValue(strength);

    QStringList hints = {
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
//when the user clicks the "Register" button, we perform several validation checks on the input fields. We check if the name and username fields are not empty, if the password and confirm password fields match, if the password meets a minimum length requirement, and if the chosen username is not already taken by another user. If any of these validations fail, we display an appropriate error message in the status label with red color to indicate the issue. If all validations pass, we create a new User object with the provided username, password, and role (Donor or Patient), and save it using the FileManager. We also create a new Donor or Patient object based on the selected role and save it accordingly. Finally, we display a success message in green color in the status label and show a message box confirming that the account has been created successfully, prompting the user to login. After that, we navigate back to the login form.
void SignupForm::onRegisterClicked()
{
    lblStatus->setText("");

    if (txtName->text().trimmed().isEmpty() || txtUsername->text().trimmed().isEmpty()) {
        lblStatus->setText("❌ Name and Username are required.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (txtPassword->text() != txtConfirmPassword->text()) {
        lblStatus->setText("❌ Passwords do not match.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (txtPassword->text().length() < 6) {
        lblStatus->setText("❌ Password must be at least 6 characters.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }
    if (FileManager::userExists(txtUsername->text().trimmed())) {
        lblStatus->setText("❌ Username already taken. Choose another.");
        lblStatus->setStyleSheet("color: #e74c3c;"); return;
    }

    User newUser(txtUsername->text().trimmed(), txtPassword->text(), cmbRole->currentText());
    FileManager::saveUser(newUser);

    QString name = txtName->text().trimmed();
    int     age = txtAge->text().toInt();
    QString contact = txtContact->text().trimmed();
    QString city = txtCity->text().trimmed();
    QString bg = cmbBloodGroup->currentText();

    if (cmbRole->currentText() == "Donor") 
    {
        Donor donor(name, age, contact, city, bg, 60.0);
        FileManager::saveDonor(donor);
    }
    else 
    {
        Patient patient(name, age, contact, city, bg, 1, "General Hospital");
        FileManager::savePatient(patient);
    }

    lblStatus->setText("✅ Account created successfully!");
    lblStatus->setStyleSheet("color: #27ae60;");
    QMessageBox::information(this, "Success", "Account registered! You can now login.");
    onBackClicked();
}

void SignupForm::onBackClicked()
{
    LoginForm* login = new LoginForm("User");
    login->show();
    this->close();
}

void SignupForm::applyStyle()
{
	//the stylesheet defines the visual appearance of the form and its elements, creating a clean and modern look. The background color is set to white, with dark text for good contrast. Input fields have a light border and rounded corners, with a focus state that highlights the field in red. The register button is styled with a bold red background and white text, changing to a brighter red on hover. The password strength bar changes color based on the strength level, providing visual feedback to the user. Overall, the stylesheet enhances the user experience by making the form visually appealing and easy to use.
    setStyleSheet(R"(
        QWidget {
            background-color: #ffffff;
            color: #2c3e50;
            font-family: Arial;
            font-size: 13px;
        }
        QLabel {
            color: #2c3e50;
            background: transparent;
        }
        #lblTitle {
            font-size: 18px;
            font-weight: bold;
            color: #2c3e50;
        }
        #lblHint   { color: #888888; font-size: 11px; }
        #lblStatus { font-size: 12px; font-weight: bold; }
        QLineEdit {
            background-color: #ffffff;
            color: #2c3e50;
            padding: 8px 12px;
            border: 1px solid #bdc3c7;
            border-radius: 5px;
            font-size: 13px;
            min-height: 34px;
        }
        QLineEdit:focus  { border: 2px solid #c0392b; background-color: #fff9f9; }
        QLineEdit:disabled { background-color: #ecf0f1; color: #95a5a6; }
        QComboBox {
            background-color: #ffffff;
            color: #2c3e50;
            padding: 7px 10px;
            border: 1px solid #bdc3c7;
            border-radius: 5px;
            min-height: 34px;
        }
        QComboBox:focus { border: 2px solid #c0392b; }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            color: #2c3e50;
            selection-background-color: #fadbd8;
        }
        QPushButton {
            background-color: #c0392b;
            color: #ffffff;
            border-radius: 6px;
            padding: 10px;
            font-size: 13px;
            font-weight: bold;
            min-height: 36px;
        }
        QPushButton:hover { background-color: #e74c3c; }
        QProgressBar {
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            height: 12px;
            background: #ecf0f1;
            color: #2c3e50;
        }
    )");
}