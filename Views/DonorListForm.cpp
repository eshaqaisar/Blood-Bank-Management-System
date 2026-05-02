#include "DonorListForm.h"//header file for the DonorListForm class
#include "DonorRegistrationForm.h"//for the form to add new donors
#include "../Utilities/FileManager.h"//for loading and deleting donors from the file
#include "../Utilities/Compatibility.h"//for checking blood group compatibility when viewing donor profiles
#include "../Models/Donor.h"//for the Donor data structure and operations
#include <QMessageBox>//for showing message boxes to the user (e.g., warnings, confirmations, and information)
#include <QHeaderView>//for customizing the appearance of the table headers
#include <QTableWidgetItem>   //for populating the donor table with items and customizing their appearance (e.g., color-coding eligibility status)
#include <QAbstractItemView>  //for setting the table to be read-only and customizing selection behavior
#include <QColor>   //for setting colors in the UI, such as color-coding eligibility status in the donor table          
#include <QFont>    //for customizing font styles in the UI, such as making the title bold and larger


DonorListForm::DonorListForm(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadDonors();
    applyStyle();
}
DonorListForm::~DonorListForm() {}
//set up the user interface elements for the donor list form, including the search bar, blood group filter, donor table, and action buttons. The layout is organized to provide a clean and user-friendly experience for managing the list of donors.
void DonorListForm::setupUI() {
	//set the window title and a reasonable default size for the donor list form, along with a minimum size to prevent it from being too small
    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("🔍 Search by name...");
    connect(txtSearch, &QLineEdit::textChanged, this, &DonorListForm::onSearchChanged);
	//combo box for filtering donors by blood group, with an option for "All Groups" to show all donors regardless of blood type. The combo box is connected to a slot that updates the donor table based on the selected blood group filter.
    cmbFilter = new QComboBox(this);
    cmbFilter->addItem("All Groups");
    cmbFilter->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });
    connect(cmbFilter, &QComboBox::currentTextChanged,
        this, &DonorListForm::onFilterByBloodGroup);
	//donor table to display the list of donors, with columns for name, age, city, blood group, eligibility status, and last donation date. The table is set to be read-only and allows for sorting by clicking on the column headers. The eligibility status is color-coded for better visual distinction (green for eligible, red for not eligible).
    tblDonors = new QTableWidget(0, 6, this);
    tblDonors->setHorizontalHeaderLabels(
        { "Name","Age","City","Blood Group","Eligible","Last Donation" });
    tblDonors->horizontalHeader()->setStretchLastSection(true);
    tblDonors->setSelectionBehavior(QAbstractItemView::SelectRows); 
    tblDonors->setEditTriggers(QAbstractItemView::NoEditTriggers);   
    tblDonors->setSortingEnabled(true); 
	//label to display the count of donors currently shown in the table, which updates dynamically as donors are added, deleted, or filtered. This provides immediate feedback to the user about the number of donors in the list.
    lblCount = new QLabel("Donors: 0", this);
    lblCount->setObjectName("lblCount");

	//action buttons for managing donors: "Add Donor" to open the donor registration form, "View Profile" to see detailed information about the selected donor, and "Delete" to remove the selected donor from the list. Each button is connected to its respective slot for handling the corresponding action when clicked.
    btnAdd = new QPushButton("➕ Add Donor", this);
    btnViewProfile = new QPushButton("👁 View Profile", this);
    btnDelete = new QPushButton("🗑 Delete", this);
	//connect buttons to their respective slots for handling clicks
    connect(btnAdd, &QPushButton::clicked, this, &DonorListForm::onAddDonorClicked);
    connect(btnViewProfile, &QPushButton::clicked, this, &DonorListForm::onViewProfileClicked);
    connect(btnDelete, &QPushButton::clicked, this, &DonorListForm::onDeleteClicked);
	//layout the form using a combination of horizontal and vertical layouts to organize the search bar, filter, donor table, and action buttons in a clean and user-friendly manner. The top bar contains the search field, filter dropdown, and donor count label, while the bottom row contains the action buttons. The overall layout includes spacing and margins for better visual separation between elements.
    QHBoxLayout* topBar = new QHBoxLayout();
    topBar->addWidget(txtSearch);
    topBar->addWidget(new QLabel("Filter:", this));
    topBar->addWidget(cmbFilter);
    topBar->addWidget(lblCount);

	//action buttons row
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAdd);
    btnRow->addWidget(btnViewProfile);
    btnRow->addWidget(btnDelete);
    btnRow->addStretch();
	//root layout to combine top bar, donor table, and button row
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(10);
    layout->addLayout(topBar);
    layout->addWidget(tblDonors);
    layout->addLayout(btnRow);
    setLayout(layout);
}
//load the list of donors from the file and populate the donor table. This function reads the donor data using the FileManager utility, clears any existing rows in the table, and then iterates through the list of donors to add them to the table. Each donor's information is displayed in a row, with color-coded eligibility status and formatted last donation date. After loading the donors, it updates the donor count label to reflect the total number of donors currently shown in the table.
void DonorListForm::loadDonors() {
    QList<Donor> donors = FileManager::loadDonors();
    tblDonors->setRowCount(0); 

    for (const Donor& d : donors)
    {
        int row = tblDonors->rowCount();
        tblDonors->insertRow(row);

        tblDonors->setItem(row, 0, new QTableWidgetItem(d.getName()));
        tblDonors->setItem(row, 1, new QTableWidgetItem(QString::number(d.getAge())));
        tblDonors->setItem(row, 2, new QTableWidgetItem(d.getCity()));
        tblDonors->setItem(row, 3, new QTableWidgetItem(d.getBloodGroup()));

        QTableWidgetItem* eligItem = new QTableWidgetItem(d.isEligible() ? "✅ Yes" : "❌ No");
        eligItem->setForeground(d.isEligible() ? QColor("#27ae60") : QColor("#e74c3c"));
        tblDonors->setItem(row, 4, eligItem);

        QString lastDate = d.getLastDonationDate().isValid()
            ? d.getLastDonationDate().toString("dd-MM-yyyy") : "First time";
        tblDonors->setItem(row, 5, new QTableWidgetItem(lastDate));
    }
    lblCount->setText("Donors: " + QString::number(donors.size()));
}

//slot that is called when the text in the search field changes, it calls the filterTable function to update the donor table based on the current search text and selected blood group filter. This allows for real-time filtering of the donor list as the user types in the search field.
void DonorListForm::onSearchChanged(const QString& text) {
    filterTable(text, cmbFilter->currentText());
}
//slot that is called when the selected blood group in the filter combo box changes, it calls the filterTable function to update the donor table based on the current search text and newly selected blood group filter. This allows for dynamic filtering of the donor list based on both the search text and the selected blood group.
void DonorListForm::onFilterByBloodGroup(const QString& bg) {
    filterTable(txtSearch->text(), bg);
}

//function to filter the donor table based on the search text and selected blood group. It iterates through each row in the donor table and checks if the donor's name contains the search text (case-insensitive) and if the donor's blood group matches the selected filter (or if "All Groups" is selected). If a row does not match both criteria, it is hidden from view. This function allows for efficient filtering of the donor list based on user input.
void DonorListForm::filterTable(const QString& text, const QString& bloodGroup) {
    for (int row = 0; row < tblDonors->rowCount(); row++) {
        bool nameMatch = tblDonors->item(row, 0)->text()
            .contains(text, Qt::CaseInsensitive);
        bool bgMatch = (bloodGroup == "All Groups") ||
            tblDonors->item(row, 3)->text() == bloodGroup;
        
        tblDonors->setRowHidden(row, !(nameMatch && bgMatch));
    }
}

//slot that is called when the "Delete" button is clicked, it checks if a donor is selected in the table, and if so, it prompts the user for confirmation before deleting the selected donor from the file. If the user confirms the deletion, it calls the FileManager to delete the donor by name and then reloads the donor list to reflect the changes in the table.
void DonorListForm::onDeleteClicked() {
    int row = tblDonors->currentRow();
	if (row < 0)//if no row is selected, show a warning message and return early to prevent errors
    {
        QMessageBox::warning(this, "No Selection", "Please select a donor to delete.");
        return;
    }
    QString name = tblDonors->item(row, 0)->text();
    auto reply = QMessageBox::question(this, "Confirm Delete",
        "Are you sure you want to delete donor: " + name + "?");
	if (reply == QMessageBox::Yes) //if the user confirms the deletion, proceed to delete the donor from the file and reload the donor list to update the table
    {
        FileManager::deleteDonor(name); 
        loadDonors(); 
    }
}

//slot that is called when the "View Profile" button is clicked, it checks if a donor is selected in the table, and if so, it retrieves the selected donor's information from the file and displays it in a message box. The profile includes the donor's name, age, contact information, city, blood group, weight, eligibility status, and a list of compatible recipients based on their blood group. This allows the user to quickly view detailed information about a specific donor.
void DonorListForm::onViewProfileClicked() {
    int row = tblDonors->currentRow();
    if (row < 0) 
    {
        QMessageBox::warning(this, "No Selection", "Please select a donor first.");
        return;
    }
    QString name = tblDonors->item(row, 0)->text();
    
    for (const Donor& d : FileManager::loadDonors()) 
    {
		if (d.getName() == name) //find the donor in the list of donors loaded from the file, and if a match is found, retrieve their information and display it in a message box. The message box includes the donor's details and a list of compatible recipients based on their blood group, which is determined using the CompatibilityChecker utility.
        {
			//get compatible recipients based on the donor's blood group using the CompatibilityChecker utility, which returns a list of blood groups that can receive blood from the donor's blood group. This information is included in the profile display to provide insights into who the donor can potentially donate to.
            QStringList recipients = CompatibilityChecker::getCompatibleRecipients(d.getBloodGroup());

            QString info =
                "Name:        " + d.getName() + "\n"
                "Age:         " + QString::number(d.getAge()) + "\n"
                "Contact:     " + d.getContact() + "\n"
                "City:        " + d.getCity() + "\n"
                "Blood Group: " + d.getBloodGroup() + "\n"
                "Weight:      " + QString::number(d.getWeight()) + " kg\n"
                "Eligible:    " + (d.isEligible() ? "Yes ✅" : "No ❌") + "\n\n"
                "Can donate to: " + recipients.join(", ");
            QMessageBox::information(this, "Donor Profile — " + name, info);
            return;
        }
    }
}
//slot that is called when the "Add Donor" button is clicked, it creates and shows a new instance of the DonorRegistrationForm, which allows the user to input information for a new donor. The slot also connects to the destroyed signal of the registration form, so that when the form is closed (either after adding a donor or canceling), it will automatically call the loadDonors function to refresh the donor list in the table and reflect any changes made.
void DonorListForm::onAddDonorClicked() {
    DonorRegistrationForm* form = new DonorRegistrationForm();
    form->show();
    
    connect(form, &QObject::destroyed, this, &DonorListForm::loadDonors);
}
//apply custom styles to the donor list form using a style sheet. This method sets the background color, font styles, and colors for various UI elements such as labels, input fields, combo boxes, tables, and buttons to create a cohesive and visually appealing design for the donor list form. It uses a combination of inline styles and object names to target specific elements like the donor count label and the table headers.
void DonorListForm::applyStyle() {
    setStyleSheet(R"(
        QWidget { background: #fdfdfd; font-family: Arial; font-size: 13px; }
        #lblCount { color: #888; font-size: 12px; }
        QLineEdit { padding: 7px; border: 1px solid #ccc; border-radius: 5px; }
        QLineEdit:focus { border: 2px solid #c0392b; }
        QComboBox { padding: 6px; border: 1px solid #ccc; border-radius: 5px; }
        QTableWidget { border: 1px solid #ddd; gridline-color: #f0f0f0; }
        QHeaderView::section {
            background: #c0392b; color: white; padding: 7px; font-weight: bold;
        }
        QTableWidget::item:selected { background: #fadbd8; color: #2c3e50; }
        QPushButton {
            background: #c0392b; color: white;
            border-radius: 6px; padding: 8px 14px; font-weight: bold;
        }
        QPushButton:hover { background: #e74c3c; }
    )");
}
