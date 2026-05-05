#ifndef DONORREGISTRATIONFORM_H
#define DONORREGISTRATIONFORM_H

#include <QWidget>//base class for all UI elements in Qt
#include <QLineEdit>//for text input fields in the form
#include <QComboBox>//for the dropdown selection of blood groups
#include <QPushButton>//for the action buttons in the form
#include <QLabel>//for displaying labels and status messages
#include <QFormLayout>//for organizing the form layout
#include <QVBoxLayout>//for organizing the overall layout of the form
#include <string>//std::string replaces QString for internal data handling

class DonorRegistrationForm : public QWidget {
    Q_OBJECT

public:
    explicit DonorRegistrationForm(QWidget* parent = nullptr);
    ~DonorRegistrationForm();

private slots:
    void onAddDonorClicked(); //validates input, saves donor and updates inventory
    void onClearClicked();    //resets all input fields

private:
    QLineEdit* txtName;
    QLineEdit* txtAge;
    QLineEdit* txtContact;
    QLineEdit* txtCity;
    QLineEdit* txtWeight;
    QLineEdit* txtUnits;
    QComboBox* cmbBloodGroup;
    QPushButton* btnAdd;
    QPushButton* btnClear;
    QLabel* lblStatus;

    void setupUI();
    void applyStyle();
    void clearFields();
};

#endif // DONORREGISTRATIONFORM_H