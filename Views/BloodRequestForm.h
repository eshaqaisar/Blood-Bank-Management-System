
#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFormLayout>
class BloodRequestForm : public QWidget {
    Q_OBJECT
private:
    QLabel* lblName;
    QLabel* lblBloodGroup;
    QLabel* lblHospital;
    QLabel* lblUnits;
    QLineEdit* txtName;
    QLineEdit* txtHospital;
    QLineEdit* txtUnits;
    QComboBox* cmbBloodGroup;
    QPushButton* btnSubmit;
public:
    BloodRequestForm(QWidget* parent = nullptr);
public slots:
    void onSubmitClicked();
};