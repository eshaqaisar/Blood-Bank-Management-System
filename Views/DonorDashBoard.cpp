#include "DonorDashBoard.h"
#include "ui_DonorDashBoard.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

DonorDashboard::DonorDashboard(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DonorDashBoardClass) // UI class match
{
    ui->setupUi(this);
}

DonorDashboard::~DonorDashboard() {
    delete ui;
}


void DonorDashboard::on_pushButton_clicked() {
    
    QString name = ui->lineEdit->text();      // Name field
    QString address = ui->lineEdit_2->text();   // Address field
    QString email = ui->lineEdit_3->text();     // Email field
    QString phone = ui->lineEdit_4->text();     // Phone field

    
    QFile file("donor.txt");
    
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << " , " << address << " ," << email << " , " << phone << "\n";
        file.close();

        
        QMessageBox::information(this, "Data Saved", " successfully! donor  data save in file.");
        
        
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
    } else {
        QMessageBox::critical(this, "Error", "File not opening!");
    }
}