#include "DonorDashboard.h"
#include "LandingPage.h"
#include "../Utilities/FileManager.h"
#include "../Models/Donor.h"
#include <QTableWidgetItem>   
#include <QHeaderView>        
#include <QAbstractItemView>  
#include <QColor>             
#include <QFont>              



DonorDashboard::DonorDashboard(const QString& donorUsername, QWidget* parent)
    : QWidget(parent), donorUsername(donorUsername)
{
    setupUI();
    loadDonorProfile(); // Populate labels from file
    applyStyle();
}
DonorDashboard::~DonorDashboard() {}

void DonorDashboard::setupUI() {
    setWindowTitle("Donor Dashboard");
    setMinimumSize(600, 500);

    lblTitle = new QLabel("🩸 Donor Dashboard", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

    lblName = new QLabel("Name: —", this);
    lblBloodGroup = new QLabel("Blood Group: —", this);
    lblEligibility = new QLabel("Eligible to Donate: —", this);
    lblLastDonation = new QLabel("Last Donation: —", this);

    // Donation history table
    tblHistory = new QTableWidget(0, 2, this); // 0 rows, 2 columns
    tblHistory->setHorizontalHeaderLabels({ "Date", "Units Donated" });
    tblHistory->horizontalHeader()->setStretchLastSection(true);
    tblHistory->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only

    btnLogout = new QPushButton("🚪 Logout", this);
    connect(btnLogout, &QPushButton::clicked, this, &DonorDashboard::onLogout);

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

// ---- Load the donor's data from donors.txt and display it ----
void DonorDashboard::loadDonorProfile() {
    // Find the donor whose name matches the username (simplified match)
    QList<Donor> donors = FileManager::loadDonors();
    for (const Donor& d : donors) {
        if (d.getName().toLower().contains(donorUsername.toLower()) ||
            d.getContact() == donorUsername) {
            // Found — populate labels using ENCAPSULATED getters
            lblName->setText("Name: " + d.getName());
            lblBloodGroup->setText("Blood Group: " + d.getBloodGroup());
            lblEligibility->setText(
                "Eligible to Donate: " + QString(d.isEligible() ? "✅ Yes" : "❌ No (cooldown active)"));
            lblLastDonation->setText(
                "Last Donation: " + (d.getLastDonationDate().isValid()
                    ? d.getLastDonationDate().toString("dd-MM-yyyy")
                    : "No previous donation"));

            // Populate history table
            QStringList history = d.getDonationHistory();
            tblHistory->setRowCount(history.size());
            for (int i = 0; i < history.size(); i++) {
                tblHistory->setItem(i, 0, new QTableWidgetItem(history[i]));
                tblHistory->setItem(i, 1, new QTableWidgetItem("1 unit"));
            }
            return;
        }
    }
    lblName->setText("Profile not found. Please contact Admin.");
}

void DonorDashboard::onLogout() {
    FileManager::logActivity("Donor logged out: " + donorUsername);
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}

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
