
#pragma once  
#include <QWidget>//base class for all UI elements in Qt
#include <QLineEdit>//for text input fields in the form
#include <QLabel>//for displaying labels next to input fields
#include <QPushButton>//for the submit button in the form
#include <QComboBox>//for the dropdown selection of blood groups
#include <QFormLayout>//for organizing the form layout in a clean and structured way
class BloodRequestForm : public QWidget {
    Q_OBJECT
private:
	QLabel* lblName;//label for patient name input
	QLabel* lblBloodGroup;//label for blood group selection
	QLabel* lblHospital;//label for hospital input
	QLabel* lblUnits;//label for units needed input
	QLineEdit* txtName;//text field for patient name input
	QLineEdit* txtHospital;//text field for hospital input
	QLineEdit* txtUnits;//text field for units needed input
	QComboBox* cmbBloodGroup;//combo box for selecting blood group
	QPushButton* btnSubmit;//button to submit the blood request form
public:
	BloodRequestForm(QWidget* parent = nullptr);//constructor to initialize the form and set up the UI elements
public slots:
	void onSubmitClicked();//slot that is called when the submit button is clicked, it collects the data from the form, creates a BloodRequest object, and saves it to a file
};