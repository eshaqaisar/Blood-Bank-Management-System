#ifndef DONORREGISTRATIONFORM_H
#define DONORREGISTRATIONFORM_H

#include <QWidget>//base class for all UI elements in Qt
#include <QLineEdit>//for text input fields in the form
#include <QComboBox>//for the dropdown selection of blood groups
#include <QPushButton>//for the action buttons in the form (e.g., "Add Donor" and "Clear Fields")
#include <QLabel>//for displaying labels next to input fields and for the status label to show validation messages or success messages after adding a donor
#include <QFormLayout>//for organizing the form layout in a clean and structured way
#include <QVBoxLayout>//for organizing the overall layout of the form, including the form fields, status label, and action buttons



class DonorRegistrationForm : public QWidget {
    Q_OBJECT

public:
    explicit DonorRegistrationForm(QWidget* parent = nullptr);
    ~DonorRegistrationForm();

private slots:
	void onAddDonorClicked();//slot that is called when the "Add Donor" button is clicked, it collects the data from the form, validates it, creates a Donor object, saves it to a file, updates the inventory, and provides feedback to the user through the status label
	void onClearClicked();//slot that is called when the "Clear Fields" button is clicked, it calls the clearFields method to reset all input fields to their default state

private:
	QLineEdit* txtName;//text field for the donor's full name input
	QLineEdit* txtAge;//text field for the donor's age input (must be 18 or older for eligibility)
	QLineEdit* txtContact;//text field for the donor's contact information input (e.g., phone number)
	QLineEdit* txtCity;//text field for the donor's city input
	QLineEdit* txtWeight;//text field for the donor's weight input (must be 50 kg or more for eligibility)
	QLineEdit* txtUnits; //text field for the number of units the donor is donating today (must be at least 1)
	QComboBox* cmbBloodGroup;//combo box for selecting the donor's blood group, with common blood types as options
	QPushButton* btnAdd;//button to submit the form and add the donor to the system
	QPushButton* btnClear;//button to clear all input fields in the form
	QLabel* lblStatus;//label for displaying validation messages or success messages after adding a donor, centered and styled to stand out when showing feedback to the user

    void setupUI();
    void applyStyle();
    void clearFields();
};

#endif // DONORREGISTRATIONFORM_H
