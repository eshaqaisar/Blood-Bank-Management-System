#ifndef REGISTRATIONFROM_H
#define REGISTRATIONFROM_H

#include <QMainWindow>

class DonorDashboard; 

QT_BEGIN_NAMESPACE
namespace Ui { class RegistrationFromClass; } 
QT_END_NAMESPACE

class RegistrationFrom : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegistrationFrom(QWidget* parent = nullptr);
    ~RegistrationFrom();

private slots:
    void on_AddDonor_clicked(); 



    void on_veiwList_clicked();
    void on_darkModeBtn_clicked();
private:
    Ui::RegistrationFromClass* ui; // Naming match
};

#endif