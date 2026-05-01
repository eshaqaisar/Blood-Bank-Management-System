#ifndef DONORREGISTRATIONFORM_H
#define DONORREGISTRATIONFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>

// ============================================================
// DonorRegistrationForm.h  |  Domain 1: Muhammad Ali
// ============================================================
//
// Form to add a new donor to the system.
// On submit:
//   1. Validates all fields
//   2. Checks donor eligibility (age, weight)
//   3. Creates a Donor object
//   4. Saves to donors.txt via FileManager
//   5. Adds blood bag to inventory.txt via BloodInventory
// ============================================================

class DonorRegistrationForm : public QWidget {
    Q_OBJECT

public:
    explicit DonorRegistrationForm(QWidget* parent = nullptr);
    ~DonorRegistrationForm();

private slots:
    void onAddDonorClicked();
    void onClearClicked();

private:
    QLineEdit*  txtName;
    QLineEdit*  txtAge;
    QLineEdit*  txtContact;
    QLineEdit*  txtCity;
    QLineEdit*  txtWeight;
    QLineEdit*  txtUnits;        // Units being donated today
    QComboBox*  cmbBloodGroup;
    QPushButton* btnAdd;
    QPushButton* btnClear;
    QLabel*     lblStatus;

    void setupUI();
    void applyStyle();
    void clearFields();
};

#endif // DONORREGISTRATIONFORM_H
