#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>


// LoginForm.h 
// Part : Esha Qaisar

// Features:
//   1. Username + Password fields
//   2. CAPTCHA verification (random 5-char code)
//   3. Live Password Strength bar (on Signup side)
//   4. Routes to correct dashboard based on user role
//
// Advanced Features:
//   - Interactive CAPTCHA (random string, user must type it)
//   - Password field with echo mode = Password (dots shown)
//
// Qt Concepts:
//   - QLineEdit::setEchoMode(QLineEdit::Password) hides password
//   - QProgressBar used for password strength indicator


class LoginForm : public QWidget
{
    Q_OBJECT

public:
    //  "Admin" or "User"  determines which dashboard opens on success
    explicit LoginForm(const QString& mode = "User", QWidget* parent = nullptr);
    ~LoginForm();

private slots:
    void onLoginClicked();    // Validate credentials and route to dashboard
    void onSignupClicked();   // Open SignupForm
    void onBackClicked();     // Go back to LandingPage
    void refreshCaptcha();    // Generate a new CAPTCHA string
    void onPasswordChanged(const QString& text); // Update strength bar live

private:
    QString mode;           // "Admin" or "User"
    QString currentCaptcha; // The randomly generated CAPTCHA string

    // Widgets 
    QLabel* lblTitle;
    QLineEdit* txtUsername;
    QLineEdit* txtPassword;
    QLabel* lblCaptchaDisplay; // Shows the CAPTCHA image/text to type
    QLineEdit* txtCaptchaInput;   // Where user types the CAPTCHA
    QPushButton* btnRefreshCaptcha; // Generate new CAPTCHA
    QProgressBar* barPasswordStrength; // Live strength indicator
    QLabel* lblStrengthText;
    QPushButton* btnLogin;
    QPushButton* btnSignup;
    QPushButton* btnBack;
    QLabel* lblStatus; // Shows error messages ("Wrong password", etc.)

    // Helpers
    void setupUI();
    void applyStyle();
    QString generateCaptcha(int length = 5); // Returns random alphanumeric string
    void navigateToDashboard(const QString& role, const QString& username);
};

#endif // LOGINFORM_H
