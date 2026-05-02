#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>//base class for all UI elements in Qt
#include <QLineEdit>//for text input fields for username, password, and CAPTCHA input
#include <QPushButton>//for the action buttons in the form (e.g., "Login", "Create Account", and "← Back")
#include <QLabel>//for displaying labels next to input fields, the title, and status messages
#include <QProgressBar>//for the live password strength indicator bar
#include <QVBoxLayout>//for organizing the layout of the form in a vertical manner
#include <QHBoxLayout>//for organizing the layout of the CAPTCHA display and refresh button in a horizontal manner


class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(const QString& mode = "User", QWidget* parent = nullptr);
    ~LoginForm();

private slots:
    void onLoginClicked();    //validate credentials and route to dashboard
    void onSignupClicked();   //open SignupForm
    void onBackClicked();     //go back to LandingPage
    void refreshCaptcha();    //generate a new CAPTCHA string
    void onPasswordChanged(const QString& text); //update strength bar live

private:
    QString mode;           // "Admin" or "User"
    QString currentCaptcha; //randomly generated CAPTCHA string

    //widgets 
    QLabel* lblTitle;
    QLineEdit* txtUsername;
    QLineEdit* txtPassword;
    QLabel* lblCaptchaDisplay; //shows the CAPTCHA image/text to type
    QLineEdit* txtCaptchaInput;   //where user types the CAPTCHA
    QPushButton* btnRefreshCaptcha; //generate new CAPTCHA
    QProgressBar* barPasswordStrength; //live strength indicator
    QLabel* lblStrengthText;
    QPushButton* btnLogin;
    QPushButton* btnSignup;
    QPushButton* btnBack;
    QLabel* lblStatus; //shows error messages ("Wrong password", etc.)

    //helpers
    void setupUI();
    void applyStyle();
    QString generateCaptcha(int length = 5); //returns random alphanumeric string
    void navigateToDashboard(const QString& role, const QString& username);
};

#endif // LOGINFORM_H
