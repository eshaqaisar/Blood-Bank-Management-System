#include <QApplication>
#include <QDir>
#include <QCoreApplication>       // ← add this
#include "Views/LandingPage.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("BloodBankSystem");

    // ✅ FIX: Set working directory to the folder containing the .exe
    // This ensures Database/users.txt is found correctly
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    LandingPage landingPage;
    landingPage.setWindowTitle("Blood Bank Management System");
    landingPage.adjustSize();
    landingPage.setMinimumSize(400, 300);
    landingPage.show();

    return app.exec();
}