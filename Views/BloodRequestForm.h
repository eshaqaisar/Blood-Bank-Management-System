#pragma once
#include <QWidget>//for QWidget
#include <QLineEdit>//for QLineEdit
#include <QLabel>//for labels
#include <QPushButton>//for QPushButton
#include <QComboBox>//for QComboBox used for blood group selection
#include <QVBoxLayout>//for QVBoxLayout to arrange the form and buttons
#include <QMessageBox>//for QMessageBox to show success or error messages
#include <string>//std::string used internally for validation and FileManager calls

class BloodRequestForm : public QWidget {
    Q_OBJECT

private:
    QString loggedInUsername; //kept as QString because it comes from Qt UI (PatientDashboard)

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

    void applyStyle();

public:
    explicit BloodRequestForm(const QString& username, QWidget* parent = nullptr);

signals:
    void requestSubmitted(); //emitted after a successful save so dashboard can refresh

public slots:
    void onSubmitClicked(); //validates input, saves the request, shows success message
    void onCancelClicked(); //just closes the form without saving
};