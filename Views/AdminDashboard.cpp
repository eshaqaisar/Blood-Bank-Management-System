#include "AdminDashboard.h"
#include "DonorListForm.h"
#include "InventoryDashboard.h"
#include "RequestManagementForm.h"
#include "LandingPage.h"
#include "../Utilities/FileManager.h"
#include "../Models/BloodBag.h"
#include <QApplication>   // REQUIRED: provides the qApp global pointer
#include <QFileDialog>
#include <QFileInfo>       // REQUIRED: for QFileInfo used in FileManager
#include <QMessageBox>



// AdminDashboard.cpp 
// Part: Esha Qaisar


AdminDashboard::AdminDashboard(const QString& adminUsername, QWidget* parent)
    : QWidget(parent), adminUsername(adminUsername)
{
    setupUI();
    applyLightMode(); // Start in light mode
}

AdminDashboard::~AdminDashboard() {}

void AdminDashboard::setupUI() {
    setWindowTitle("Admin Dashboard - Blood Bank System");
    setMinimumSize(950, 650);

    // Top Bar 
    lblWelcome = new QLabel("👋 Welcome, Admin: " + adminUsername, this);
    lblWelcome->setObjectName("lblWelcome");

    lblAlert = new QLabel("", this); // Empty until low stock detected
    lblAlert->setObjectName("lblAlert");
    lblAlert->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QHBoxLayout* topBar = new QHBoxLayout();
    topBar->addWidget(lblWelcome);
    topBar->addStretch();
    topBar->addWidget(lblAlert);

    //  Navigation Sidebar Buttons 
    btnDonors = new QPushButton("👥  Donor List", this);
    btnInventory = new QPushButton("🩸  Blood Inventory", this);
    btnRequests = new QPushButton("📋  Requests", this);
    btnToggleDark = new QPushButton("🌙  Dark Mode", this);
    btnExportCSV = new QPushButton("📊  Export CSV", this);
    btnLogout = new QPushButton("🚪  Logout", this);

    // Each button switches the stacked widget page
    connect(btnDonors, &QPushButton::clicked, this, &AdminDashboard::showDonorList);
    connect(btnInventory, &QPushButton::clicked, this, &AdminDashboard::showInventory);
    connect(btnRequests, &QPushButton::clicked, this, &AdminDashboard::showRequestManagement);
    connect(btnToggleDark, &QPushButton::clicked, this, &AdminDashboard::toggleDarkMode);
    connect(btnExportCSV, &QPushButton::clicked, this, &AdminDashboard::exportCSV);
    connect(btnLogout, &QPushButton::clicked, this, &AdminDashboard::onLogout);

    // Left Sidebar Layout 
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

    QWidget* sidebarWidget = new QWidget(this);
    sidebarWidget->setLayout(sidebar);
    sidebarWidget->setFixedWidth(180);
    sidebarWidget->setObjectName("sidebar");

    // QStackedWidget: Content Area 
    // Each page is a separate form widget added as a "page"
    donorListPage = new DonorListForm(this);
    inventoryPage = new InventoryDashboard(this);
    requestsPage = new RequestManagementForm(this);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(donorListPage);   // Page 0
    stackedWidget->addWidget(inventoryPage);   // Page 1
    stackedWidget->addWidget(requestsPage);    // Page 2
    stackedWidget->setCurrentIndex(0);         // Show Donor List first

    // Main Layout: sidebar | content area 
    QHBoxLayout* mainRow = new QHBoxLayout();
    mainRow->addWidget(sidebarWidget);
    mainRow->addWidget(stackedWidget);

    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->addLayout(topBar);
    rootLayout->addLayout(mainRow);
    setLayout(rootLayout);

    // Check for low stock and show alert if needed
    BloodInventory inv;
    inv.load(FileManager::INVENTORY_FILE);
    QStringList criticalGroups;
    for (const QString& g : { "O-", "AB-", "B-", "A-" }) {
        if (inv.isLowStock(g))
            criticalGroups << g;
    }
    if (!criticalGroups.isEmpty()) {
        lblAlert->setText("⚠️ LOW STOCK: " + criticalGroups.join(", ") + " — Order immediately!");
        lblAlert->setStyleSheet("color: red; font-weight: bold;");
    }
}

// Switch to page 0: Donor List 
void AdminDashboard::showDonorList() {
    stackedWidget->setCurrentIndex(0);
}

// Switch to page 1: Inventory 
void AdminDashboard::showInventory() {
    stackedWidget->setCurrentIndex(1);
}

// Switch to page 2: Requests 
void AdminDashboard::showRequestManagement() {
    stackedWidget->setCurrentIndex(2);
}

//  Advanced Feature: Dark Mode / Light Mode Toggle 
// Reads a QSS file and applies it to the whole application.
void AdminDashboard::toggleDarkMode() {
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
    // Read light QSS file and apply to entire app
    QFile file("Resources/style_light.qss");
    if (file.open(QIODevice::ReadOnly)) {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }
    else
    {
        // Fallback inline style if file not found
        qApp->setStyleSheet("QWidget { background: #f5f5f5; color: #2c3e50; }");
    }
}

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
        // Fallback inline dark style
        qApp->setStyleSheet(
            "QWidget { background: #2c3e50; color: #ecf0f1; }"
            "QPushButton { background: #34495e; color: white; border-radius: 5px; padding: 8px; }"
            "QPushButton:hover { background: #c0392b; }"
            "QLineEdit, QTableWidget { background: #34495e; color: #ecf0f1; border: 1px solid #555; }"
        );
    }
}

//  Advanced Feature: One-Click CSV Export 
void AdminDashboard::exportCSV()
{
    // Let admin choose where to save the file
    QString filePath = QFileDialog::getSaveFileName(
        this, "Export Donors to CSV", "donors_export.csv", "CSV Files (*.csv)"
    );
    if (filePath.isEmpty()) return; // User cancelled

    if (FileManager::exportDonorsToCSV(filePath)) {
        QMessageBox::information(this, "Success",
            "Donor list exported to:\n" + filePath);
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to export CSV file.");
    }
}

void AdminDashboard::onLogout() {
    FileManager::logActivity("Admin logged out: " + adminUsername);
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}
