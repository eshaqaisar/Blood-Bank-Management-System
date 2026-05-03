#include <QApplication>//provides the application framework and event loop
#include <QDir>//for setting the working directory to the executable's location
#include <QCoreApplication>//to get the executable's directory path
#include <QFile>//for loading the .qss stylesheet
#include <QMessageBox>//to show an error if the working directory cannot be set
#include "Views/LandingPage.h"//the main landing page after the splash screen
#include "SplashPage.h"//the initial splash screen shown while loading resources

int main(int argc, char* argv[])//entry point of the application
{
    QApplication app(argc, argv);
    app.setApplicationName("BloodBankSystem");

	//set the working directory to the executable's location, ensuring relative paths work correctly
    QString exeDir = QCoreApplication::applicationDirPath();
    if (!QDir::setCurrent(exeDir)) {
        QMessageBox::critical(nullptr, "Startup Error",
            "Cannot set working directory to:\n" + exeDir);
        return 1;
    }
    QDir().mkpath("Database");
    QDir().mkpath("Resources");

    
    //all forms inherit these base styles; individual forms only need to
    //override named widgets (titles, status labels, etc.) via setObjectName().
    QFile qss("Resources/style_light.qss");
    if (qss.open(QIODevice::ReadOnly)) {
        app.setStyleSheet(qss.readAll());
        qss.close();
    }
    else {
        //inline fallback ,guarantees readable colors even if .qss is missing
        app.setStyleSheet(
            "QWidget    { background-color:#f5f5f5; color:#2c3e50;"
            "             font-family:Arial; font-size:13px; }"
            "QLineEdit  { background:#fff; color:#2c3e50;"
            "             border:1px solid #bdc3c7; border-radius:5px;"
            "             padding:7px 10px; min-height:32px; }"
            "QComboBox  { background:#fff; color:#2c3e50;"
            "             border:1px solid #bdc3c7; border-radius:5px;"
            "             padding:6px 10px; min-height:32px; }"
            "QPushButton{ background:#c0392b; color:#fff; border-radius:6px;"
            "             padding:9px 18px; font-weight:bold; }"
            "QPushButton:hover{ background:#e74c3c; }"
            "QLabel     { color:#2c3e50; background:transparent; }"
            "QTableWidget{ background:#fff; color:#2c3e50; gridline-color:#eee; }"
            "QHeaderView::section{ background:#c0392b; color:#fff;"
            "             padding:8px; font-weight:bold; border:none; }"
        );
    }
	//show the splash screen while loading resources, then transition to the landing page
    SplashPage splash;
    splash.show();

  
    return app.exec();
}