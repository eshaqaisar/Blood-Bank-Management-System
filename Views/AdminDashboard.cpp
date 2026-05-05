#include "AdminDashboard.h"//header file for the AdminDashboard class
#include "DonorListForm.h"//for the donor list management form
#include "InventoryDashboard.h"//for the blood inventory management dashboard
#include "RequestManagementForm.h"//for managing blood requests
#include "LandingPage.h"//for the landing page to return to on logout
#include "../Utilities/FileManager.h"//for handling file operations like CSV export and activity logging
#include "../Models/BloodBag.h"//for blood bag data structures and operations
#include <QApplication>//for qApp used in style sheet application
#include <QFileDialog>//for file dialog used in CSV export
#include <QFileInfo>//for checking file existence and properties
#include <QMessageBox>//for showing message boxes to the user
#include <QString>//used only at Qt UI boundary via QString::fromStdString
#include <string>//for std::string used throughout

// AdminDashboard.cpp
// Part: Esha Qaisar

//constructor: initializes the admin dashboard with the given username and sets up the UI
AdminDashboard::AdminDashboard(const std::string& adminUsername, QWidget* parent)
    : QWidget(parent), adminUsername(adminUsername)
{
    //seed demo data at first launch if database files are empty
    FileManager::seedDemoData();
    setupUI();
    applyLightMode(); //start in light mode by default
}

AdminDashboard::~AdminDashboard() {}

void AdminDashboard::setupUI()
{
    setWindowTitle("Admin Dashboard - Blood Bank System");
    setMinimumSize(950, 650);

    //top bar: welcome message and low stock alert
    //convert std::string to QString only at Qt UI label boundary
    lblWelcome = new QLabel("Welcome, Admin: " + QString::fromStdString(adminUsername), this);
    lblWelcome->setObjectName("lblWelcome");

    lblAlert = new QLabel("", this); //initially empty, will be set if low stock is detected
    lblAlert->setObjectName("lblAlert");
    lblAlert->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout* topBar = new QHBoxLayout();
    topBar->addWidget(lblWelcome);
    topBar->addStretch();
    topBar->addWidget(lblAlert);

    //left sidebar buttons for navigation and actions
    btnDonors = new QPushButton("👥  Donor List", this);
    btnInventory = new QPushButton("🩸  Blood Inventory", this);
    btnRequests = new QPushButton("📋  Requests", this);
    btnToggleDark = new QPushButton("🌙  Dark Mode", this);
    btnExportCSV = new QPushButton("📊  Export CSV", this);
    btnLogout = new QPushButton("🚪  Logout", this);

    connect(btnDonors, &QPushButton::clicked, this, &AdminDashboard::showDonorList);
    connect(btnInventory, &QPushButton::clicked, this, &AdminDashboard::showInventory);
    connect(btnRequests, &QPushButton::clicked, this, &AdminDashboard::showRequestManagement);
    connect(btnToggleDark, &QPushButton::clicked, this, &AdminDashboard::toggleDarkMode);
    connect(btnExportCSV, &QPushButton::clicked, this, &AdminDashboard::exportCSV);
    connect(btnLogout, &QPushButton::clicked, this, &AdminDashboard::onLogout);

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
    sidebar->addStretch();
    sidebar->addWidget(btnLogout);

    QWidget* sidebarWidget = new QWidget(this);
    sidebarWidget->setLayout(sidebar);
    sidebarWidget->setFixedWidth(180);
    sidebarWidget->setObjectName("sidebar");

    //create the main content pages
    donorListPage = new DonorListForm(this);
    inventoryPage = new InventoryDashboard(this);
    requestsPage = new RequestManagementForm(this);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(donorListPage);  //page 0
    stackedWidget->addWidget(inventoryPage);  //page 1
    stackedWidget->addWidget(requestsPage);   //page 2
    stackedWidget->setCurrentIndex(0);        //show donor list first

    QHBoxLayout* mainRow = new QHBoxLayout();
    mainRow->addWidget(sidebarWidget);
    mainRow->addWidget(stackedWidget);

    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->addLayout(topBar);
    rootLayout->addLayout(mainRow);
    setLayout(rootLayout);

    //check for low stock using std::string blood groups — FIX: was using QStringList
    BloodInventory inv;
    inv.load(FileManager::INVENTORY_FILE);
    const char* criticalGroups[] = { "O-", "AB-", "B-", "A-" };
    std::string alertText = "";
    for (int i = 0; i < 4; i++) {
        if (inv.isLowStock(criticalGroups[i])) {
            if (!alertText.empty()) alertText += ", ";
            alertText += criticalGroups[i];
        }
    }
    if (!alertText.empty()) {
        lblAlert->setText("⚠️ LOW STOCK: " + QString::fromStdString(alertText) + " — Order immediately!");
        lblAlert->setStyleSheet("color: red; font-weight: bold;");
    }
}

//switch to page 0: Donor List
void AdminDashboard::showDonorList() { stackedWidget->setCurrentIndex(0); }

//switch to page 1: Inventory
void AdminDashboard::showInventory() { stackedWidget->setCurrentIndex(1); }

//switch to page 2: Requests
void AdminDashboard::showRequestManagement() { stackedWidget->setCurrentIndex(2); }

//advanced feature: dark mode / light mode toggle
void AdminDashboard::toggleDarkMode()
{
    isDarkMode = !isDarkMode;
    if (isDarkMode) {
        applyDarkMode();
        btnToggleDark->setText("☀️  Light Mode");
    }
    else {
        applyLightMode();
        btnToggleDark->setText("🌙  Dark Mode");
    }
}

void AdminDashboard::applyLightMode()
{
    QFile file("Resources/style_light.qss");
    if (file.open(QIODevice::ReadOnly)) {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
    else {
        qApp->setStyleSheet("QWidget { background: #f5f5f5; color: #2c3e50; }");
    }
}

void AdminDashboard::applyDarkMode()
{
    QFile file("Resources/style_dark.qss");
    if (file.open(QIODevice::ReadOnly)) {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
    else {
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
        this, "Export Donors to CSV", "donors_export.csv", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return; //user cancelled

    //convert QString path to std::string at the FileManager boundary
    if (FileManager::exportDonorsToCSV(filePath.toStdString())) {
        QMessageBox::information(this, "Success", "Donor list exported to:\n" + filePath);
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to export CSV file.");
    }
}

//handle logout: log the activity, show the landing page, and close the dashboard
void AdminDashboard::onLogout()
{
    FileManager::logActivity("Admin logged out: " + adminUsername);
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}