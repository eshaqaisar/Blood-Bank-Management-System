#ifndef SIGNUPFORM_H
#define SIGNUPFORM_H

#include <QWidget>//base class for all UI elements in Qt
#include <QLineEdit>//for text input fields for name, age, contact, city, username, password, and confirm password
#include <QPushButton>//for the action buttons in the form (e.g., "Register" and "← Back")
#include <QLabel>//for displaying labels next to input fields, the title, and status messages
#include <QComboBox>//for the dropdown selection of blood groups and user roles (Donor or Patient)
#include <QProgressBar>//for the live password strength indicator bar
#include <QVBoxLayout>//for organizing the layout of the form in a vertical manner
#include <string>//std::string replaces QString for all internal data passed to Model/FileManager constructors

class SignupForm : public QWidget
{
    Q_OBJECT

public:
    explicit SignupForm(QWidget* parent = nullptr);
    ~SignupForm();

private slots:
    void onRegisterClicked();              //validates input, saves user/donor/patient, shows feedback
    void onBackClicked();                  //navigates back to the login form
    void onPasswordChanged(const QString& text); //updates strength bar live as user types

private:
    QLineEdit* txtName;             //for name input used in Donor/Patient object construction
    QLineEdit* txtAge;              //for age input, used for eligibility checks (18+ for donors)
    QLineEdit* txtContact;          //for contact information, passed to Donor/Patient constructor
    QLineEdit* txtCity;             //for city input, passed to Donor/Patient constructor
    QLineEdit* txtUsername;         //for username input — must be unique across all users
    QLineEdit* txtPassword;         //for password input with echo mode set to Password
    QLineEdit* txtConfirmPassword;  //for confirming password — must match txtPassword
    QComboBox* cmbBloodGroup;       //for selecting blood group, passed to Donor/Patient constructor
    QComboBox* cmbRole;             //Donor or Patient selection
    QProgressBar* barStrength;         //live password strength indicator bar (0–4)
    QLabel* lblStrengthHint;     //hint text next to the strength bar (e.g. "add uppercase")
    QPushButton* btnRegister;         //submit the registration form and create a new user account
    QPushButton* btnBack;             //navigate back to the login form without registering
    QLabel* lblStatus;           //shows validation messages or success messages after registration

    void setupUI();
    void applyStyle();
};

#endif // SIGNUPFORM_H