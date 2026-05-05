#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>//base class for all UI elements in Qt
#include <QPushButton>//for the login buttons on the landing page
#include <QLabel>//for the title and subtitle labels on the landing page
#include <QVBoxLayout>//for organizing the layout of the landing page in a vertical manner
#include <QPainter>//background
#include <QPaintEvent>
//forward declaration to avoid circular dependency with LoginForm, since LandingPage only needs to know that LoginForm exists to open it when buttons are clicked
class LoginForm;

class LandingPage : public QWidget
{
    Q_OBJECT

public:
    explicit LandingPage(QWidget* parent = nullptr);
    ~LandingPage();

protected:
    void paintEvent(QPaintEvent* event) override; //background

private slots:
    void onAdminLoginClicked(); //slot that opens the login form in admin mode
    void onUserLoginClicked();  //slot that opens the login form in user mode

private:
    QPushButton* btnAdminLogin;
    QPushButton* btnUserLogin;
    QLabel* lblTitle;
    QLabel* lblSubtitle;

    void setupUI();    //initializes the user interface elements and layouts
    void applyStyle(); //applies custom styles to the landing page
};

#endif // LANDINGPAGE_H