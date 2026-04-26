#include "BloodRequestForm.h"
#include "BloodRequest.h"

BloodRequestForm::BloodRequestForm(QWidget* parent) : QWidget(parent) {
    lblName = new QLabel("Patient Name:");
    lblBloodGroup = new QLabel("Blood Group:");
    lblHospital = new QLabel("Hospital:");
    lblUnits = new QLabel("Units Needed:");



    txtName = new QLineEdit();
    txtHospital = new QLineEdit();
    txtUnits = new QLineEdit();

    cmbBloodGroup = new QComboBox();

    cmbBloodGroup->addItem("A+");
    cmbBloodGroup->addItem("A-");
    cmbBloodGroup->addItem("B+");
    cmbBloodGroup->addItem("B-");
    cmbBloodGroup->addItem("O+");
    cmbBloodGroup->addItem("O-");
    cmbBloodGroup->addItem("AB+");
    cmbBloodGroup->addItem("AB-");

    btnSubmit = new QPushButton("Submit Request");

    QFormLayout* layout = new QFormLayout();
    layout->addRow(lblName, txtName);
    layout->addRow(lblBloodGroup, cmbBloodGroup);
    layout->addRow(lblHospital, txtHospital);
    layout->addRow(lblUnits, txtUnits);
    layout->addRow(btnSubmit);

    setLayout(layout);

    setWindowTitle("Blood Request Form");

    connect(btnSubmit, &QPushButton::clicked, this, &BloodRequestForm::onSubmitClicked);

}
void BloodRequestForm::onSubmitClicked() {
    QString name = txtName->text();
    QString hospital = txtHospital->text();
    QString blood = cmbBloodGroup->currentText();
    int units = txtUnits->text().toInt();
    BloodRequest request("R001", name, hospital, blood, units);
    request.saveToFile();
}