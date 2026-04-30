#include "RegistrationFrom.h"
#include "ui_RegistrationFrom.h" 
#include "DonorDashBoard.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include"DonorTable.h"
#include <QApplication>

RegistrationFrom::RegistrationFrom(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::RegistrationFromClass) 
{
    ui->setupUi(this);
    if (ui->veiwList) {
        ui->veiwList->show();
        ui->veiwList->raise();
    }
}

RegistrationFrom::~RegistrationFrom() {
    delete ui;
}

void RegistrationFrom::on_AddDonor_clicked() {
    QString name = ui->lineEdit->text();
    int age = ui->spinBox->value();
    int weight = ui->spinBox_2->value();

    if (age >= 18 && weight >= 50) {
        // Eligibility success
        QMessageBox::information(this, "Eligible", "congulations! Dashboard successfully open.");

        this->hide(); 
        DonorDashboard* dash = new DonorDashboard();
        dash->show(); // Dashboard show
    }
    else {
        ui->AddDonor->setText("sorry! you are not eligible .");
        ui->AddDonor->setStyleSheet("color: red;");
    }
}


//  'veiwList' button click 
void RegistrationFrom::on_veiwList_clicked() {
    this->hide();
    // Table window create karein
    DonorTable* tableWindow = new DonorTable();

    //  Window ko show
    tableWindow->show();

    
    // this->hide(); 
}
void
RegistrationFrom::on_darkModeBtn_clicked() {
    static bool isDark = false;

    if (!isDark) {
        qApp->setStyleSheet(
            "QMainWindow { background-color: #121212; }"
            "QLabel { color: #E0E0E0; }"
            "QPushButton { background-color: #333333; color: white; border-radius: 5px; }"
            "QLineEdit, QSpinBox, QComboBox { background-color: #1E1E1E; color: white; }"
        );
        ui->darkModeBtn->setText("Light Mode");
        isDark = true;
    }
    else {
        qApp->setStyleSheet("");
        ui->darkModeBtn->setText("Dark Mode");
        isDark = false;
    }
}