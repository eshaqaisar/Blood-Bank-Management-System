#ifndef SIGNUPFORM_H
#define SIGNUPFORM_H

#include <QWidget>//base class for all UI elements in Qt
#include <QLineEdit>//for text input fields for name, age, contact, city, username, password, and confirm password
#include <QPushButton>//for the action buttons in the form (e.g., "Register" and "← Back")
#include <QLabel>//for displaying labels next to input fields, the title, and status messages
#include <QComboBox>//for the dropdown selection of blood groups and user roles (Donor or Patient)
#include <QProgressBar>//for the live password strength indicator bar
#include <QVBoxLayout>//for organizing the layout of the form in a vertical manner


class SignupForm : public QWidget
{
    Q_OBJECT

public:
    explicit SignupForm(QWidget* parent = nullptr);
    ~SignupForm();

private slots:
    void onRegisterClicked();
    void onBackClicked();
    void onPasswordChanged(const QString& text);

private:
	QLineEdit* txtName;//for name input, which can be used for personalization and record-keeping purposes (e.g., showing the user's name in the dashboard after login)
	QLineEdit* txtAge;//for age input, which can be used for eligibility checks (e.g., donors must be 18 or older)
	QLineEdit* txtContact;//for contact information input, which can be used for communication purposes (e.g., phone number or email)
	QLineEdit* txtCity;//for city input, which can be used for demographic purposes or to show nearby donation centers for donors
	QLineEdit* txtUsername;//for username input, which will be used for login and must be unique across all users
	QLineEdit* txtPassword;//for password input, with echo mode set to Password to hide the input for security
	QLineEdit* txtConfirmPassword;//for confirming the password input, it should match the txtPassword field for successful registration
	QComboBox* cmbBloodGroup;//for selecting blood group, only relevant if registering as a donor, but can be optional for patients. The form can include logic to hide or disable this field when the user selects "Patient" as their role.
    QComboBox* cmbRole;         //Donor or Patient
	QProgressBar* barStrength;//live strength indicator for the password field
	QLabel* lblStrengthHint;//label to show "Password Strength:" text next to the strength bar
	QPushButton* btnRegister;//button to submit the registration form and create a new user account
	QPushButton* btnBack;//button to go back to the login form
	QLabel* lblStatus;//for showing validation messages or success messages after registration, centered and styled to stand out when showing feedback to the user

    void setupUI();
    void applyStyle();
};

#endif // SIGNUPFORM_H
