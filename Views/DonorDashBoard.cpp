#include "DonorDashboard.h"//header file for the donor dashboard view
#include "LandingPage.h" //header file for the landing page view
#include "../Utilities/FileManager.h"//header file for file management utilities
#include "../Models/Donor.h"//header file for the Donor model
#include <QTableWidgetItem>//header file for table widget items
#include <QHeaderView>//header file for table header view
#include <QAbstractItemView>//header file for abstract item view (for setting edit triggers)
#include <QColor>//header file for color definitions
#include <QFont>//header file for font definitions

DonorDashboard::DonorDashboard(const QString& donorUsername, QWidget* parent)
    : QWidget(parent), donorUsername(donorUsername)
{
    setupUI();
    loadDonorProfile();
    applyStyle();
}
DonorDashboard::~DonorDashboard() {}
//initializes the UI components and layout for the donor dashboard
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

    tblHistory = new QTableWidget(0, 2, this);
    tblHistory->setHorizontalHeaderLabels({ "Date", "Units Donated" });
    tblHistory->horizontalHeader()->setStretchLastSection(true);
    tblHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);

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

void DonorDashboard::loadDonorProfile() {
    QList<Donor> donors = FileManager::loadDonors();
    for (const Donor& d : donors) {
        if (d.getName().toLower().contains(donorUsername.toLower()) ||
            d.getContact() == donorUsername) {

            lblName->setText("Name: " + d.getName());
            lblBloodGroup->setText("Blood Group: " + d.getBloodGroup());
            lblEligibility->setText(
                "Eligible to Donate: " + QString(d.isEligible() ? "✅ Yes" : "❌ No (cooldown active)"));
            lblLastDonation->setText(
                "Last Donation: " + (d.getLastDonationDate().isValid()
                    ? d.getLastDonationDate().toString("dd-MM-yyyy")
                    : "No previous donation"));

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
	//apply a clean and modern style to the donor dashboard using Qt's stylesheet
    setStyleSheet(R"(
        QWidget {
            background-color: #ffffff;
            color: #2c3e50;
            font-family: Arial;
            font-size: 13px;
        }
        QLabel {
            font-size: 13px;
            color: #2c3e50;
            background: transparent;
        }
        #lblTitle {
            font-size: 20px;
            font-weight: bold;
            color: #c0392b;
        }
        QTableWidget {
            border: 1px solid #ddd;
            background-color: #ffffff;
            color: #2c3e50;
            gridline-color: #f0f0f0;
        }
        QTableWidget::item { color: #2c3e50; padding: 5px 8px; }
        QTableWidget::item:selected { background: #fadbd8; color: #2c3e50; }
        QHeaderView::section {
            background: #c0392b; color: #ffffff;
            padding: 7px; font-weight: bold; border: none;
        }
        QPushButton {
            background: #2c3e50;
            color: #ffffff;
            border-radius: 6px;
            padding: 9px;
            font-size: 13px;
            min-height: 36px;
        }
        QPushButton:hover { background: #34495e; }
    )");
}