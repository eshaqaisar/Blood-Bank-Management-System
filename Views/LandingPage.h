#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>


// LandingPage.h 
// Part: Esha Qaisar

// The FIRST screen shown when the app launches.
// Has two buttons:
//   1. "Login as Admin"  → opens LoginForm with Admin mode
//   2. "Login as User"   → opens LoginForm with User mode
//
// Qt Concepts Used:
//   - QWidget: base class for all UI elements
//   - SIGNAL/SLOT: button click → navigate to LoginForm
//   - QVBoxLayout: arranges widgets vertically


// Forward declaration to avoid circular includes
class LoginForm;

class LandingPage : public QWidget 
{
    Q_OBJECT  // Required for Qt SIGNAL/SLOT system to work

public:
    explicit LandingPage(QWidget* parent = nullptr);
    ~LandingPage();

private slots:
    // These slots are connected to button clicks
    void onAdminLoginClicked();  // Opens LoginForm in Admin mode
    void onUserLoginClicked();   // Opens LoginForm in User mode

private:
    // UI Widgets 
    QPushButton* btnAdminLogin;
    QPushButton* btnUserLogin;
    QLabel* lblTitle;
    QLabel* lblSubtitle;

    // Setup helper 
    void setupUI();   // Creates and arranges all widgets
    void applyStyle(); // Applies CSS-like stylesheet
};

#endif // LANDINGPAGE_H
