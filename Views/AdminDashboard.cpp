#include "AdminDashboard.h" //header file for the AdminDashboard class
#include "DonorListForm.h"//for the donor list management form
#include "InventoryDashboard.h"//for the blood inventory management dashboard
#include "RequestManagementForm.h"//for managing blood requests
#include "LandingPage.h"//for the landing page to return to on logout
#include "../Utilities/FileManager.h"//for handling file operations like CSV export and activity logging
#include "../Models/BloodBag.h"//for blood bag data structures and operations
#include <QApplication>  //for qApp used in style sheet application
#include <QFileDialog>//for file dialog used in CSV export
#include <QFileInfo>   //for checking file existence and properties      
#include <QMessageBox>//for showing message boxes to the user (e.g., success or error messages)

// AdminDashboard.cpp 
// Part: Esha Qaisar

//constructor: initializes the admin dashboard with the given username and sets up the UI
AdminDashboard::AdminDashboard(const QString& adminUsername, QWidget* parent)
    : QWidget(parent), adminUsername(adminUsername)
{
	//log the admin login activity
    setupUI();
	//start in light mode by default
	applyLightMode(); //start in light mode by default
}
//destructor
AdminDashboard::~AdminDashboard() {}

void AdminDashboard::setupUI()
{
    setWindowTitle("Admin Dashboard - Blood Bank System");
    setMinimumSize(950, 650);

	//top bar:welcome message and low stock alert 
    lblWelcome = new QLabel("Welcome, Admin: " + adminUsername, this);
    lblWelcome->setObjectName("lblWelcome");

	lblAlert = new QLabel("", this); //initially empty, will be set if low stock is detected
	lblAlert->setObjectName("lblAlert");//set alignment to right and vertically centered
	lblAlert->setAlignment(Qt::AlignRight | Qt::AlignVCenter);//top bar layout: welcome message on the left, alert on the right
	//use a horizontal layout for the top bar to place welcome message and alert on opposite sides
    QHBoxLayout* topBar = new QHBoxLayout();//set spacing and margins for the top bar
	topBar->addWidget(lblWelcome);//welcome message on the left
	topBar->addStretch();//pushes the alert label to the right
	topBar->addWidget(lblAlert);//alert on the right

	//left sidebar buttons for navigation and actions 
    btnDonors = new QPushButton("👥  Donor List", this);
    btnInventory = new QPushButton("🩸  Blood Inventory", this);
    btnRequests = new QPushButton("📋  Requests", this);
    btnToggleDark = new QPushButton("🌙  Dark Mode", this);
    btnExportCSV = new QPushButton("📊  Export CSV", this);
    btnLogout = new QPushButton("🚪  Logout", this);

	//connect buttons to their respective slots for handling clicks
    connect(btnDonors, &QPushButton::clicked, this, &AdminDashboard::showDonorList);
    connect(btnInventory, &QPushButton::clicked, this, &AdminDashboard::showInventory);
    connect(btnRequests, &QPushButton::clicked, this, &AdminDashboard::showRequestManagement);
    connect(btnToggleDark, &QPushButton::clicked, this, &AdminDashboard::toggleDarkMode);
    connect(btnExportCSV, &QPushButton::clicked, this, &AdminDashboard::exportCSV);
    connect(btnLogout, &QPushButton::clicked, this, &AdminDashboard::onLogout);

	//style the sidebar buttons with icons and text, and set them to be flat for a modern look 
    QVBoxLayout* sidebar = new QVBoxLayout();
    sidebar->setSpacing(8);
    sidebar->setContentsMargins(10, 10, 10, 10);
    sidebar->addWidget(new QLabel("📌 Navigation", this));
    sidebar->addWidget(btnDonors);
    sidebar->addWidget(btnInventory);
    sidebar->addWidget(btnRequests);
    sidebar->addSpacing(20);
    sidebar->addWidget(btnToggleDark);
    sidebar->addWidget(btnExportCSV);
    sidebar->addStretch(); // Pushes logout to bottom
    sidebar->addWidget(btnLogout);
	//create a sidebar widget to contain the buttons and set a fixed width for it
    QWidget* sidebarWidget = new QWidget(this);
    sidebarWidget->setLayout(sidebar);
    sidebarWidget->setFixedWidth(180);
    sidebarWidget->setObjectName("sidebar");

	//create the main content area with a stacked widget to switch between different pages (donor list, inventory, requests)
    donorListPage = new DonorListForm(this);
    inventoryPage = new InventoryDashboard(this);
    requestsPage = new RequestManagementForm(this);
	//stacked widget to hold the different pages and allow switching between them
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(donorListPage);   //page 0
    stackedWidget->addWidget(inventoryPage);   //page 1
    stackedWidget->addWidget(requestsPage);    //page 2
    stackedWidget->setCurrentIndex(0);         //show donor list first

	//main layout: top bar at the top, sidebar on the left, and main content area on the right 
    QHBoxLayout* mainRow = new QHBoxLayout();
    mainRow->addWidget(sidebarWidget);
    mainRow->addWidget(stackedWidget);
	//root layout to combine top bar and main row
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->addLayout(topBar);
    rootLayout->addLayout(mainRow);
    setLayout(rootLayout);

    //check for low stock and show alert if needed
    BloodInventory inv;
    inv.load(FileManager::INVENTORY_FILE);
    QStringList criticalGroups;
    for (const QString& g : { "O-", "AB-", "B-", "A-" })
    {
		if (inv.isLowStock(g))//if any of the critical blood groups are low in stock, add them to the alert list
            criticalGroups << g;
    }
	if (!criticalGroups.isEmpty())//if there are critical blood groups, show an alert message in red and bold
    {
        lblAlert->setText("⚠️ LOW STOCK: " + criticalGroups.join(", ") + " — Order immediately!");
        lblAlert->setStyleSheet("color: red; font-weight: bold;");
    }
}

//switch to page 0: Donor List 
void AdminDashboard::showDonorList()
{
    stackedWidget->setCurrentIndex(0);
}

//switch to page 1: Inventory 
void AdminDashboard::showInventory()
{
    stackedWidget->setCurrentIndex(1);
}

//switch to page 2: Requests 
void AdminDashboard::showRequestManagement() 
{
    stackedWidget->setCurrentIndex(2);
}

//advanced feature: dark mode / light mode toggle 
//reads a QSS file and applies it to the whole application.
void AdminDashboard::toggleDarkMode()
{
    isDarkMode = !isDarkMode;
	if (isDarkMode) //if dark mode is now enabled, apply dark styles and update button text to indicate light mode is available
    {
        applyDarkMode();
        btnToggleDark->setText("☀️  Light Mode");
    }
	else //if dark mode is now disabled, apply light styles and update button text to indicate dark mode is available
    {
        applyLightMode();
        btnToggleDark->setText("🌙  Dark Mode");
    }
}

void AdminDashboard::applyLightMode() //apply light mode styles, either from a QSS file or fallback to inline styles if the file is not found
{
    //read light QSS file and apply to entire app
    QFile file("Resources/style_light.qss");
    if (file.open(QIODevice::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
	else//if the file cannot be opened, apply a simple inline style as a fallback to ensure the app remains usable
    {
        //fallback inline style if file not found
        qApp->setStyleSheet("QWidget { background: #f5f5f5; color: #2c3e50; }");
    }
}
//apply dark mode styles, either from a QSS file or fallback to inline styles if the file is not found
void AdminDashboard::applyDarkMode()
{
    QFile file("Resources/style_dark.qss");
    if (file.open(QIODevice::ReadOnly)) 
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
    else
    {
        //fallback inline dark style
        qApp->setStyleSheet(
            "QWidget { background: #2c3e50; color: #ecf0f1; }"
            "QPushButton { background: #34495e; color: white; border-radius: 5px; padding: 8px; }"
            "QPushButton:hover { background: #c0392b; }"
            "QLineEdit, QTableWidget { background: #34495e; color: #ecf0f1; border: 1px solid #555; }"
        );
    }
}

//advanced feature: one-click CSV export 
void AdminDashboard::exportCSV()
{
    //let admin choose where to save the file
    QString filePath = QFileDialog::getSaveFileName(
        this, "Export Donors to CSV", "donors_export.csv", "CSV Files (*.csv)"
    );
    if (filePath.isEmpty()) return; //user cancelled

    if (FileManager::exportDonorsToCSV(filePath)) 
    {
        QMessageBox::information(this, "Success",
            "Donor list exported to:\n" + filePath);
    }
	else//if export fails, show an error message
    {
        QMessageBox::critical(this, "Error", "Failed to export CSV file.");
    }
}
//handle logout: log the activity, show the landing page, and close the dashboard
void AdminDashboard::onLogout() 
{
    FileManager::logActivity("Admin logged out: " + adminUsername);
    LandingPage* landing = new LandingPage();
    landing->show();
	this->close();//close the dashboard window
}
