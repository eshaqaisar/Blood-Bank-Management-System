#ifndef DONORREGISTRATIONFORM_H
#define DONORREGISTRATIONFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>




class DonorRegistrationForm : public QWidget {
    Q_OBJECT

public:
    explicit DonorRegistrationForm(QWidget* parent = nullptr);
    ~DonorRegistrationForm();

private slots:
    void onAddDonorClicked();
    void onClearClicked();

private:
    QLineEdit* txtName;
    QLineEdit* txtAge;
    QLineEdit* txtContact;
    QLineEdit* txtCity;
    QLineEdit* txtWeight;
    QLineEdit* txtUnits;        // Units being donated today
    QComboBox* cmbBloodGroup;
    QPushButton* btnAdd;
    QPushButton* btnClear;
    QLabel* lblStatus;

    void setupUI();
    void applyStyle();
    void clearFields();
};

#endif // DONORREGISTRATIONFORM_H
