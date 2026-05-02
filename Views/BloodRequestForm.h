#pragma once
#include <QWidget>//for QWidget
#include <QLineEdit>//for QLineEdit
#include <QLabel>//for labels like "Patient Name:", "Hospital:", etc.
#include <QPushButton>//for QPushButton
#include <QComboBox>//for QComboBox used for blood group selection
#include <QFormLayout>//for QFormLayout to neatly arrange labels and fields
#include <QVBoxLayout>//for QVBoxLayout to arrange the form and buttons
#include <QMessageBox>//for QMessageBox to show success or error messages



class BloodRequestForm : public QWidget {
    Q_OBJECT

private:
    QString loggedInUsername;   //store the patient's username to use as patient name

    QLabel* lblName;
    QLabel* lblBloodGroup;
    QLabel* lblHospital;
    QLabel* lblUnits;
    QLabel* lblStatus;

    QLineEdit* txtName;       //pre-filled and read-only with username
    QLineEdit* txtHospital;
    QLineEdit* txtUnits;
    QComboBox* cmbBloodGroup;
    QPushButton* btnSubmit;
    QPushButton* btnCancel;

public:
    //constructor now takes the logged-in username
    explicit BloodRequestForm(const QString& username, QWidget* parent = nullptr);

signals:
    void requestSubmitted(); //emitted after a successful save so dashboard can refresh

public slots:
	void onSubmitClicked();//validates input, saves the request using FileManager, shows a success message, and emits requestSubmitted() signal
	void onCancelClicked();//just closes the form without saving
};