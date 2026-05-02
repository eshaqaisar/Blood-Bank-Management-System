#include "BloodRequestForm.h"
#include "../Models/BloodRequest.h"
#include <QDateTime>

//constructor: initializes the blood request form and sets up the UI elements
BloodRequestForm::BloodRequestForm(QWidget* parent) : QWidget(parent)
{
	//create labels and input fields for the blood request form
    lblName = new QLabel("Patient Name:");
    lblBloodGroup = new QLabel("Blood Group:");
    lblHospital = new QLabel("Hospital:");
    lblUnits = new QLabel("Units Needed:");


	//input fields for the form
    txtName = new QLineEdit();
    txtHospital = new QLineEdit();
    txtUnits = new QLineEdit();
	//combo box for selecting blood group, with common blood types as options
    cmbBloodGroup = new QComboBox();
	//add common blood groups to the combo box for selection
    cmbBloodGroup->addItem("A+");
    cmbBloodGroup->addItem("A-");
    cmbBloodGroup->addItem("B+");
    cmbBloodGroup->addItem("B-");
    cmbBloodGroup->addItem("O+");
    cmbBloodGroup->addItem("O-");
    cmbBloodGroup->addItem("AB+");
    cmbBloodGroup->addItem("AB-");
	//submit button for the form
    btnSubmit = new QPushButton("Submit Request");
	//layout the form using a QFormLayout for a clean and organized appearance
    QFormLayout* layout = new QFormLayout();
    layout->addRow(lblName, txtName);
    layout->addRow(lblBloodGroup, cmbBloodGroup);
    layout->addRow(lblHospital, txtHospital);
    layout->addRow(lblUnits, txtUnits);
    layout->addRow(btnSubmit);
	//set the layout for the form
    setLayout(layout);
	//set the window title for the blood request form
    setWindowTitle("Blood Request Form");
	//connect the submit button to the slot that handles form submission
    connect(btnSubmit, &QPushButton::clicked, this, &BloodRequestForm::onSubmitClicked);

}
//slot that is called when the submit button is clicked, it collects the data from the form, creates a BloodRequest object, and saves it to a file
void BloodRequestForm::onSubmitClicked()
{
	QString name = txtName->text();//get the patient name from the text field
	QString hospital = txtHospital->text();//get the selected hospital from the text field
	QString blood = cmbBloodGroup->currentText();//get the selected blood group from the combo box
	int units = txtUnits->text().toInt();//collect data from the form fields
	BloodRequest request("R001", name, hospital, blood, units);//create a new blood request object with the collected data (using a hardcoded ID for simplicity)
	request.saveToFile();//save the request to a file for later processing
}