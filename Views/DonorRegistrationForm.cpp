#include "DonorRegistrationForm.h"//header file for the DonorRegistrationForm class
#include "../Utilities/FileManager.h"//for saving donor data to the file and managing inventory updates
#include "../Models/Donor.h"//for the Donor data structure and operations, including creating a new donor object with the provided information and calculating eligibility based on age and weight
#include "../Models/BloodBag.h"//for the BloodBag data structure and operations, including creating a new blood bag object to represent the donated units and adding it to the inventory
#include <QMessageBox>//for showing message boxes to the user (e.g., for input validation errors or success messages)


DonorRegistrationForm::DonorRegistrationForm(QWidget* parent) : QWidget(parent) {
    setupUI();
    applyStyle();
}
DonorRegistrationForm::~DonorRegistrationForm() {}
//set up the user interface elements for the donor registration form, including input fields for the donor's name, age, contact information, city, blood group, weight, and units donating today. The form also includes a status label for displaying validation messages and action buttons for adding the donor and clearing the fields. The layout is organized to provide a clean and user-friendly experience for registering new donors.
void DonorRegistrationForm::setupUI() {
    setWindowTitle("Add New Donor");
    setMinimumWidth(420);

    txtName      = new QLineEdit(this); txtName->setPlaceholderText("Full Name");
    txtAge       = new QLineEdit(this); txtAge->setPlaceholderText("Age (must be 18+)");
    txtContact   = new QLineEdit(this); txtContact->setPlaceholderText("Phone Number");
    txtCity      = new QLineEdit(this); txtCity->setPlaceholderText("City");
    txtWeight    = new QLineEdit(this); txtWeight->setPlaceholderText("Weight in kg (must be 50+)");
    txtUnits     = new QLineEdit(this); txtUnits->setPlaceholderText("Units donating today (e.g. 1)");
	//combo box for selecting blood group, with common blood types as options
    cmbBloodGroup = new QComboBox(this);
    cmbBloodGroup->addItems({"A+","A-","B+","B-","AB+","AB-","O+","O-"});
	//status label for displaying validation messages or success messages after adding a donor. It is centered and styled to stand out when showing feedback to the user.
    lblStatus = new QLabel("", this);
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setObjectName("lblStatus");
	//action buttons for the form: "Add Donor" to submit the form and add the donor to the system, and "Clear Fields" to reset all input fields to their default state. Each button is connected to its respective slot for handling the corresponding action when clicked.
    btnAdd   = new QPushButton("✅  Add Donor", this);
    btnClear = new QPushButton("🗑️  Clear Fields", this);
	//connect buttons to their respective slots for handling clicks
    connect(btnAdd,   &QPushButton::clicked, this, &DonorRegistrationForm::onAddDonorClicked);
    connect(btnClear, &QPushButton::clicked, this, &DonorRegistrationForm::onClearClicked);
	//layout the form using a QFormLayout for a clean and organized appearance, with labels on the left and input fields on the right. The form includes spacing between rows for better readability, and the overall layout includes margins to prevent the content from being too close to the edges of the window. The status label is placed below the form fields to provide immediate feedback to the user after attempting to add a donor.
    QFormLayout* form = new QFormLayout();
    form->setSpacing(10);
    form->addRow("Name:",        txtName);
    form->addRow("Age:",         txtAge);
    form->addRow("Contact:",     txtContact);
    form->addRow("City:",        txtCity);
    form->addRow("Blood Group:", cmbBloodGroup);
    form->addRow("Weight (kg):", txtWeight);
    form->addRow("Units Today:", txtUnits);
	//action buttons row
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAdd);
    btnRow->addWidget(btnClear);
	//overall layout for the form, status label, and buttons
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 25, 30, 25);
    layout->setSpacing(12);
    layout->addWidget(new QLabel("👤 Register New Donor", this));
    layout->addLayout(form);
    layout->addWidget(lblStatus);
    layout->addLayout(btnRow);
    setLayout(layout);
}
//slot that is called when the "Add Donor" button is clicked, it performs input validation on the form fields, creates a new Donor object with the provided information, saves the donor to the file, and updates the blood inventory by adding a new BloodBag object representing the donated units. The slot also provides feedback to the user through the status label, indicating whether the donor was successfully added or if there were any validation errors that need to be addressed.
void DonorRegistrationForm::onAddDonorClicked() {
    lblStatus->setText("");

	//input validation: check that required fields are filled and that age, weight, and units are within acceptable ranges. If any validation fails, display an appropriate error message in the status label and return early to prevent adding an invalid donor.
    QString name    = txtName->text().trimmed();
    int     age     = txtAge->text().toInt();
    double  weight  = txtWeight->text().toDouble();
    int     units   = txtUnits->text().toInt();

	if (name.isEmpty() || txtContact->text().isEmpty())//check that the name and contact fields are not empty, as they are required for identifying and contacting the donor. If either field is empty, show an error message in the status label and return early to prevent adding an incomplete donor record.
    {
        lblStatus->setText("❌ Name and Contact are required.");
        lblStatus->setStyleSheet("color: red;"); return;
    }
	if (age < 18)//check that the donor is at least 18 years old, as this is a common requirement for blood donation eligibility. If the age is below 18, show an error message in the status label and return early to prevent adding an ineligible donor.
    {
        lblStatus->setText("❌ Donor must be at least 18 years old.");
        lblStatus->setStyleSheet("color: red;"); return;
    }
	if (weight < 50.0)//check that the donor weighs at least 50 kg, as this is a common requirement for blood donation eligibility. If the weight is below 50 kg, show an error message in the status label and return early to prevent adding an ineligible donor.
    {
        lblStatus->setText("❌ Donor must weigh at least 50 kg.");
        lblStatus->setStyleSheet("color: red;"); return;
    }
	if (units < 1)//check that the number of units donating today is at least 1, as it does not make sense to add a donor who is not donating any blood. If the units are less than 1, show an error message in the status label and return early to prevent adding a donor with invalid donation information.
    {
        lblStatus->setText("❌ Enter a valid number of units (minimum 1).");
        lblStatus->setStyleSheet("color: red;"); return;
    }

	//create a new Donor object with the provided information, using the current date as the last donation date. The Donor class likely has a constructor that takes these parameters and initializes the donor's information accordingly. This object will be used to save the donor's data to the file and to update the inventory with the donated blood.
    Donor newDonor(
        name, age,
        txtContact->text().trimmed(),
        txtCity->text().trimmed(),
        cmbBloodGroup->currentText(),
        weight,
		QDate::currentDate() //set the last donation date to the current date, as this donor is donating today
    );

	//save the new donor to the file using the FileManager utility, which handles the file operations for storing donor data. This will allow the new donor to be included in the list of donors when it is loaded from the file in other parts of the application.
    FileManager::saveDonor(newDonor);

	//update the blood inventory by adding a new BloodBag object representing the donated units. The BloodBag class likely has a constructor that takes the blood group, number of units, donation date, and donor name as parameters. The inventory is loaded from the file, the new blood bag is added to it, and then the updated inventory is saved back to the file to reflect the new donation.
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    BloodBag bag(cmbBloodGroup->currentText(), units, QDate::currentDate(), name);
    inventory.addBag(bag);
    inventory.save(FileManager::INVENTORY_FILE);
	//provide feedback to the user through the status label, indicating that the donor was successfully added and how many units were added to the inventory. The message is styled in green to indicate success, and the form fields are cleared to allow for adding another donor if needed.
    lblStatus->setText("✅ Donor " + name + " added and " +
                       QString::number(units) + " unit(s) added to inventory.");
    lblStatus->setStyleSheet("color: green;");
	clearFields();//clear the form fields after successfully adding the donor to prepare for the next entry and to prevent accidental duplicate entries. This method resets all input fields to their default state, allowing the user to easily add another donor without having to manually clear the previous information.
}
//slot that is called when the "Clear Fields" button is clicked, it calls the clearFields method to reset all input fields to their default state. This allows the user to quickly clear the form if they want to start over or if they entered incorrect information and want to re-enter it without having to manually delete the text from each field.
void DonorRegistrationForm::clearFields() {
    txtName->clear(); txtAge->clear(); txtContact->clear();
    txtCity->clear(); txtWeight->clear(); txtUnits->clear();
    cmbBloodGroup->setCurrentIndex(0);
}

void DonorRegistrationForm::onClearClicked() { clearFields(); }

void DonorRegistrationForm::applyStyle() {
    setStyleSheet(R"(
        QWidget { background: #fff; font-family: Arial; font-size: 13px; }
        QLabel { color: #2c3e50; font-weight: bold; font-size: 14px; }
        #lblStatus { font-size: 12px; font-weight: bold; }
        QLineEdit { padding: 7px; border: 1px solid #bdc3c7; border-radius: 5px; }
        QLineEdit:focus { border: 2px solid #c0392b; }
        QComboBox { padding: 6px; border: 1px solid #bdc3c7; border-radius: 5px; }
        QPushButton {
            background: #c0392b; color: white;
            border-radius: 6px; padding: 9px; font-size: 13px; font-weight: bold;
        }
        QPushButton:hover { background: #e74c3c; }
    )");
}
