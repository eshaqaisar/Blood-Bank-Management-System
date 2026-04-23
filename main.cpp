#include "BloodBankSystem.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BloodBankSystem window;
    window.show();
    return app.exec();
}
