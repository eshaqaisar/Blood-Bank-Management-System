#include "DonorDashboard.h"//header file for the DonorDashboard class
#include "LandingPage.h"//for the landing page to return to on logout
#include "../Utilities/FileManager.h"//for loading donor data and logging activities
#include "../Models/Donor.h"//for the Donor data structure and operations
#include <QTableWidgetItem>   //for populating the donation history table with items
#include <QHeaderView>        //for customizing the appearance of the table headers
#include <QAbstractItemView>  //for setting the table to be read-only and customizing selection behavior
#include <QColor>             //for setting colors in the UI, such as eligibility status indicators
#include <QFont>              //for customizing font styles in the UI, such as making the title bold and larger



DonorDashboard::DonorDashboard(const QString& donorUsername, QWidget* parent)
    : QWidget(parent), donorUsername(donorUsername)
{
	setupUI();//set up the user interface elements
    loadDonorProfile(); //populate labels from file
	applyStyle();//apply custom styles to the dashboard
}
//destructor
DonorDashboard::~DonorDashboard() {}

void DonorDashboard::setupUI() 
{
	setWindowTitle("Donor Dashboard");//set a reasonable default size for the dashboard and a minimum size to prevent it from being too small
    setMinimumSize(600, 500);
	//title label for the dashboard, styled to be larger and bold, with a blood drop emoji for visual appeal. It is centered at the top of the dashboard to clearly indicate the purpose of the page.
    lblTitle = new QLabel("🩸 Donor Dashboard", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");
	//profile labels to display the donor's information, initialized with placeholders until the actual data is loaded from the file. These will be updated using the ENCAPSULATED getters from the Donor class when loadDonorProfile() is called.
    lblName = new QLabel("Name: —", this);
    lblBloodGroup = new QLabel("Blood Group: —", this);
    lblEligibility = new QLabel("Eligible to Donate: —", this);
    lblLastDonation = new QLabel("Last Donation: —", this);

    //donation history table
    tblHistory = new QTableWidget(0, 2, this); // 0 rows, 2 columns
    tblHistory->setHorizontalHeaderLabels({ "Date", "Units Donated" });
    tblHistory->horizontalHeader()->setStretchLastSection(true);
    tblHistory->setEditTriggers(QAbstractItemView::NoEditTriggers); //read-only
	//style the table to match the overall dashboard design
    btnLogout = new QPushButton("🚪 Logout", this);
    connect(btnLogout, &QPushButton::clicked, this, &DonorDashboard::onLogout);
	//layout the dashboard using a vertical layout to stack the title, profile info, donation history, and logout button in a clean and organized manner. The layout includes spacing and margins for better visual separation between elements.
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->setSpacing(12);
    layout->addWidget(lblTitle);
    layout->addWidget(lblName);
    layout->addWidget(lblBloodGroup);
    layout->addWidget(lblEligibility);
    layout->addWidget(lblLastDonation);
    layout->addWidget(new QLabel("📋 Donation History:", this));
    layout->addWidget(tblHistory);
    layout->addWidget(btnLogout);
    setLayout(layout);
}

//load the donor's data from donors.txt and display it 
void DonorDashboard::loadDonorProfile()
{
    //find the donor whose name matches the username (simplified match)
    QList<Donor> donors = FileManager::loadDonors();
    for (const Donor& d : donors) 
    {
        if (d.getName().toLower().contains(donorUsername.toLower()) ||
            d.getContact() == donorUsername)
        {
			//populate labels with donor information using getters from the Donor class
            lblName->setText("Name: " + d.getName());
            lblBloodGroup->setText("Blood Group: " + d.getBloodGroup());
            lblEligibility->setText(
                "Eligible to Donate: " + QString(d.isEligible() ? "✅ Yes" : "❌ No (cooldown active)"));
            lblLastDonation->setText(
                "Last Donation: " + (d.getLastDonationDate().isValid()
                    ? d.getLastDonationDate().toString("dd-MM-yyyy")
                    : "No previous donation"));

			//populate the donation history table with the donor's donation history, using the getDonationHistory() method from the Donor class to retrieve a list of donation records. Each record is displayed in a new row in the table, showing the date and units donated.
            QStringList history = d.getDonationHistory();
            tblHistory->setRowCount(history.size());
			for (int i = 0; i < history.size(); i++) //for each donation record, add a new row to the table with the date and units donated (assuming 1 unit per donation for simplicity)
            {
                tblHistory->setItem(i, 0, new QTableWidgetItem(history[i]));
                tblHistory->setItem(i, 1, new QTableWidgetItem("1 unit"));
            }
            return;
        }
    }
    lblName->setText("Profile not found. Please contact Admin.");
}
//handle logout: log the activity, show the landing page, and close the dashboard
void DonorDashboard::onLogout() //when the logout button is clicked, this slot is called to handle the logout process. It logs the activity using the FileManager's logActivity method, creates and shows a new instance of the LandingPage to return the user to the main screen, and then closes the current DonorDashboard window to complete the logout process.
{
    FileManager::logActivity("Donor logged out: " + donorUsername);
    LandingPage* landing = new LandingPage();
    landing->show();
	this->close();//close the dashboard window
}
//apply custom styles to the dashboard using a style sheet. This method sets the background color, font styles, and colors for various UI elements to create a cohesive and visually appealing design for the donor dashboard. It uses a combination of inline styles and object names to target specific elements like the title label and the donation history table.
void DonorDashboard::applyStyle() {
    setStyleSheet(R"(
        QWidget { background: #fff; font-family: Arial; font-size: 13px; }
        #lblTitle { font-size: 20px; font-weight: bold; color: #c0392b; }
        QLabel { font-size: 13px; color: #2c3e50; }
        QTableWidget { border: 1px solid #ddd; }
        QPushButton {
            background: #2c3e50; color: white;
            border-radius: 6px; padding: 9px; font-size: 13px;
        }
        QPushButton:hover { background: #34495e; }
    )");
}
