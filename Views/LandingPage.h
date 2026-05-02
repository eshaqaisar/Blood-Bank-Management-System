#ifndef LANDINGPAGE_H
#define LANDINGPAGE_H

#include <QWidget>//base class for all UI elements in Qt
#include <QPushButton>//for the login buttons on the landing page
#include <QLabel>//for the title and subtitle labels on the landing page
#include <QVBoxLayout>//for organizing the layout of the landing page in a vertical manner
#include <QPainter>      // background
#include <QPaintEvent>
//forward declaration to avoid circular dependency with LoginForm, since LandingPage only needs to know that LoginForm exists to open it when buttons are clicked
class LoginForm;

class LandingPage : public QWidget 
{
	Q_OBJECT  //macro to enable Qt's signal and slot mechanism for this class

public:
    explicit LandingPage(QWidget* parent = nullptr);
    ~LandingPage();

protected:
	void paintEvent(QPaintEvent* event) override;// background

private slots:
	//slots for handling button clicks to open the login form in the appropriate mode (admin or user)
	void onAdminLoginClicked(); //slot that is called when the "Login as Admin" button is clicked, it will open the login form in admin mode 
	void onUserLoginClicked();  //slot that is called when the "Login as Donor / Patient" button is clicked, it will open the login form in user mode

private:
	//ui elements
    QPushButton* btnAdminLogin;
    QPushButton* btnUserLogin;
    QLabel* lblTitle;
    QLabel* lblSubtitle;

	//helper functions to set up the UI and apply styles
	void setupUI();   //initializes the user interface elements and layouts for the landing page
	void applyStyle(); //applies custom styles to the landing page, such as colors and fonts, to create a visually appealing and welcoming interface for users when they first open the application
};

#endif // LANDINGPAGE_H
