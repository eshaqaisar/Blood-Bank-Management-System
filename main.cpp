#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include "Views/LandingPage.h"
// Main - Entry point of the application
// Part: Esha Qaisar 
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("BloodBankSystem");

    
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    if (appDataDir.isEmpty())
    {
        QMessageBox::critical(nullptr, "Error", "Cannot determine data directory.");
        return 1;
    }
    if (!QDir().mkpath(appDataDir + "/Database") || !QDir().mkpath(appDataDir + "/Resources")) {
        QMessageBox::critical(nullptr, "Error", "Failed to create required directories.");
        return 1;
    }
    QDir::setCurrent(appDataDir);

    
    LandingPage landingPage;
    landingPage.setWindowTitle("Blood Bank Management System");

    
    landingPage.adjustSize();           
    landingPage.setMinimumSize(400, 300); 

    landingPage.show();

    return app.exec();
}