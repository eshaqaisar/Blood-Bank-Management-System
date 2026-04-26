#ifndef SIGNUPFORM_H
#define SIGNUPFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QProgressBar>
#include <QVBoxLayout>

// SignupForm.h 
// Part: Esha Qaisar

//  User registration form. Collects:
//  Name, Age, Contact, Blood Group (QComboBox),
//  City, Password, Role (Admin/Donor/Patient)
//  Creates both a User (for login) AND a Donor/Patient record.
//  Advanced Feature: Live Password Strength QProgressBar


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
    QLineEdit* txtName;
    QLineEdit* txtAge;
    QLineEdit* txtContact;
    QLineEdit* txtCity;
    QLineEdit* txtUsername;
    QLineEdit* txtPassword;
    QLineEdit* txtConfirmPassword;
    QComboBox* cmbBloodGroup;
    QComboBox* cmbRole;         // Donor or Patient
    QProgressBar* barStrength;
    QLabel* lblStrengthHint;
    QPushButton* btnRegister;
    QPushButton* btnBack;
    QLabel* lblStatus;

    void setupUI();
    void applyStyle();
};

#endif // SIGNUPFORM_H
