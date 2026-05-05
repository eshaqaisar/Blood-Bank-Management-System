#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>//base class for all UI elements in Qt
#include <QLineEdit>//for text input fields for username, password, and CAPTCHA input
#include <QPushButton>//for the action buttons in the form
#include <QLabel>//for displaying labels, title, and status messages
#include <QProgressBar>//for the live password strength indicator bar
#include <QVBoxLayout>//for organizing the layout of the form in a vertical manner
#include <QHBoxLayout>//for organizing the layout of the CAPTCHA display and refresh button
#include <QPainter>//for background image
#include <QPaintEvent>//for background image
#include <string>//std::string used for username/password passing to dashboard constructors

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(const QString& mode = "User", QWidget* parent = nullptr);
    ~LoginForm();

protected:
    void paintEvent(QPaintEvent* event) override; //background image

private slots:
    void onLoginClicked();    //validate credentials and route to dashboard
    void onSignupClicked();   //open SignupForm
    void onBackClicked();     //go back to LandingPage
    void refreshCaptcha();    //generate a new CAPTCHA string
    void onPasswordChanged(const QString& text); //update strength bar live

private:
    QString mode;           //"Admin" or "User" — kept as QString (pure Qt UI mode flag)
    QString currentCaptcha; //randomly generated CAPTCHA string — kept as QString (Qt UI only)

    //widgets
    QLabel* lblTitle;
    QLineEdit* txtUsername;
    QLineEdit* txtPassword;
    QLabel* lblCaptchaDisplay;
    QLineEdit* txtCaptchaInput;
    QPushButton* btnRefreshCaptcha;
    QProgressBar* barPasswordStrength;
    QLabel* lblStrengthText;
    QPushButton* btnLogin;
    QPushButton* btnSignup;
    QPushButton* btnBack;
    QLabel* lblStatus;

    //helpers
    void setupUI();
    void applyStyle();
    QString generateCaptcha(int length = 5); //returns random alphanumeric string
    //navigateToDashboard uses std::string to pass username to dashboard constructors
    void navigateToDashboard(const std::string& role, const std::string& username);
};

#endif // LOGINFORM_H