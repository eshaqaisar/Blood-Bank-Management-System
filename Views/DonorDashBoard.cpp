#include "DonorDashboard.h"//header file for the donor dashboard view
#include "LandingPage.h"//header file for the landing page view
#include "../Utilities/FileManager.h"//header file for file management utilities
#include "../Models/Donor.h"//header file for the Donor model
#include "../Models/BloodBag.h"//header file for BloodInventory used to build donation history
#include <QTableWidgetItem>//header file for table widget items
#include <QHeaderView>//header file for table header view
#include <QAbstractItemView>//header file for abstract item view
#include <QColor>//header file for color definitions
#include <QString>//used only at Qt UI boundary via fromStdString
#include <string>//for std::string used throughout

DonorDashboard::DonorDashboard(const std::string& donorUsername, QWidget* parent)
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

    lblTitle = new QLabel("Donor Dashboard", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

    lblName = new QLabel("Name: --", this);
    lblBloodGroup = new QLabel("Blood Group: --", this);
    lblEligibility = new QLabel("Eligible to Donate: --", this);
    lblLastDonation = new QLabel("Last Donation: --", this);

    //history table: 3 columns -- Date, Units Donated, Blood Group
    tblHistory = new QTableWidget(0, 3, this);
    tblHistory->setHorizontalHeaderLabels({ "Date", "Units Donated", "Blood Group" });
    tblHistory->horizontalHeader()->setStretchLastSection(true);
    tblHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblHistory->setSelectionBehavior(QAbstractItemView::SelectRows);

    btnRefresh = new QPushButton("Refresh", this);
    btnLogout = new QPushButton("Logout", this);

    connect(btnRefresh, &QPushButton::clicked, this, &DonorDashboard::onRefreshClicked);
    connect(btnLogout, &QPushButton::clicked, this, &DonorDashboard::onLogout);

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnRefresh);
    btnRow->addStretch();
    btnRow->addWidget(btnLogout);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->setSpacing(12);
    layout->addWidget(lblTitle);
    layout->addWidget(lblName);
    layout->addWidget(lblBloodGroup);
    layout->addWidget(lblEligibility);
    layout->addWidget(lblLastDonation);
    layout->addWidget(new QLabel("Donation History:", this));
    layout->addWidget(tblHistory);
    layout->addLayout(btnRow);
    setLayout(layout);
}

//loadDonorProfile: reads donor info from donors.txt AND builds donation history
//from inventory.txt by matching bags whose donorName == this donor's name.
//FIX: the old code tried to use getDonationHistoryCount() which was always 0
//     because Donor::fromFileString() never reads history from the file.
//     The correct source of history is BloodInventory -- every bag has a donorName
//     field written when the donor registered. We scan all bags (including expired)
//     to give a complete history, sorted by the bag's collection date.
void DonorDashboard::loadDonorProfile() {

    //--- step 1: load and display donor profile info ---
    bool found = false;
    QList<Donor> donors = FileManager::loadDonors();
    for (const Donor& d : donors) {
        //case-insensitive name match or exact contact match
        std::string dNameLower = d.getName();
        std::string userLower = donorUsername;
        for (char& c : dNameLower) if (c >= 'A' && c <= 'Z') c += 32;
        for (char& c : userLower)  if (c >= 'A' && c <= 'Z') c += 32;

        bool nameMatch = (dNameLower.find(userLower) != std::string::npos);
        bool contactMatch = (d.getContact() == donorUsername);

        if (nameMatch || contactMatch) {
            found = true;
            //convert std::string to QString only at Qt label boundary
            lblName->setText("Name: " + QString::fromStdString(d.getName()));
            lblBloodGroup->setText("Blood Group: " + QString::fromStdString(d.getBloodGroup()));
            lblEligibility->setText(
                "Eligible to Donate: " +
                QString(d.isEligible() ? "Yes" : "No (cooldown active)"));
            lblLastDonation->setText(
                "Last Donation: " + (d.getLastDonationDate().isValid()
                    ? d.getLastDonationDate().toString("dd-MM-yyyy")
                    : QString("No previous donation")));
            break;
        }
    }
    if (!found) {
        lblName->setText("Profile not found. Please contact Admin.");
        tblHistory->setRowCount(0);
        return;
    }

    //--- step 2: build donation history from inventory bags ---
    //Load ALL bags from inventory (including expired ones so full history shows).
    //Each BloodBag stores the donorName of who donated it.
    //We match bags where donorName == this donor's name (case-insensitive).
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    BagArray allBags = inventory.getAllBags(); //BagArray replaces QList<BloodBag>

    //getAllBags() returns all bags including expired ones,
    //so no separate expired fetch needed

    //filter bags that belong to this donor by name match
    tblHistory->setRowCount(0);
    int row = 0;
    for (int bi = 0; bi < allBags.count; bi++) {
        const BloodBag& bag = allBags[bi];
        //case-insensitive match between bag.donorName and this donor's username
        std::string bagDonorLower = bag.getDonorName();
        std::string userLower = donorUsername;
        for (char& c : bagDonorLower) if (c >= 'A' && c <= 'Z') c += 32;
        for (char& c : userLower)     if (c >= 'A' && c <= 'Z') c += 32;

        //also try matching against the actual donor name we found above
        bool matched = (bagDonorLower.find(userLower) != std::string::npos)
            || (userLower.find(bagDonorLower) != std::string::npos);

        if (!matched) continue;

        tblHistory->insertRow(row);

        //column 0: collection date formatted as dd-MM-yyyy
        QString dateStr = bag.getCollectionDate().toString("dd-MM-yyyy");
        tblHistory->setItem(row, 0, new QTableWidgetItem(dateStr));

        //column 1: units donated in this bag
        tblHistory->setItem(row, 1,
            new QTableWidgetItem(QString::number(bag.getUnits()) + " unit(s)"));

        //column 2: blood group of this bag
        tblHistory->setItem(row, 2,
            new QTableWidgetItem(QString::fromStdString(bag.getBloodGroup())));

        //colour expired rows in light red so donor can see which bags have expired
        if (bag.isExpired()) {
            for (int col = 0; col < 3; col++) {
                QTableWidgetItem* item = tblHistory->item(row, col);
                if (item) item->setForeground(QColor("#e74c3c"));
            }
        }
        row++;
    }

    //if no bags found, show a helpful placeholder
    if (tblHistory->rowCount() == 0) {
        tblHistory->insertRow(0);
        QTableWidgetItem* ph = new QTableWidgetItem("No donation history found.");
        ph->setForeground(QColor("#aaa"));
        tblHistory->setItem(0, 0, ph);
        tblHistory->setSpan(0, 0, 1, 3);
    }
}

//refresh slot: reload donor profile and history from disk
void DonorDashboard::onRefreshClicked() {
    loadDonorProfile();
}

void DonorDashboard::onLogout() {
    FileManager::logActivity("Donor logged out: " + donorUsername);
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}

void DonorDashboard::applyStyle() {
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