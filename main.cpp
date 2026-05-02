#include <QApplication> //main application class for Qt applications
#include <QDir>//for handling directory paths and operations
#include <QCoreApplication> //provides an event loop for console applications, also used for application-wide settings      
#include "Views/LandingPage.h"//for the landing page of the application
//entry point of the application
int main(int argc, char* argv[])
{
	//create the application object, which manages application-wide resources and settings
    QApplication app(argc, argv);
	//set the application name, which can be used for settings and organization
    app.setApplicationName("BloodBankSystem");

	//set the current working directory to the application's directory
    QDir::setCurrent(QCoreApplication::applicationDirPath());
	//create and show the landing page
    LandingPage landingPage;
	//set the window title for the landing page
    landingPage.setWindowTitle("Blood Bank Management System");
	//set a reasonable default size for the landing page
    landingPage.adjustSize();
	//set a minimum size to prevent it from being too small
    landingPage.setMinimumSize(400, 300);
	//center the landing page on the screen
    landingPage.show();
	//start the event loop
    return app.exec();
}