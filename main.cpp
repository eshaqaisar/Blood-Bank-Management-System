#include <QApplication>
#include <QDir>
#include <QCoreApplication>
#include <QFile>
#include <QMessageBox>
#include "Views/LandingPage.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("BloodBankSystem");

  
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
        // Inline fallback ,guarantees readable colors even if .qss is missing
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

    LandingPage landingPage;
    landingPage.setWindowTitle("Blood Bank Management System");
    landingPage.setMinimumSize(480, 380);
    landingPage.show();

    return app.exec();
}