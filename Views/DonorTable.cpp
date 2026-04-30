#include "DonorListForm.h"
#include "DonorRegistrationForm.h"
#include "../Utilities/FileManager.h"
#include "../Utilities/Compatibility.h"
#include "../Models/Donor.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>   
#include <QAbstractItemView>  
#include <QColor>             
#include <QFont>              



DonorListForm::DonorListForm(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadDonors();
    applyStyle();
}
DonorListForm::~DonorListForm() {}

void DonorListForm::setupUI() {
    // ---- Search Bar ----
    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("🔍 Search by name...");
    connect(txtSearch, &QLineEdit::textChanged, this, &DonorListForm::onSearchChanged);

    // ---- Blood Group Filter ----
    cmbFilter = new QComboBox(this);
    cmbFilter->addItem("All Groups");
    cmbFilter->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });
    connect(cmbFilter, &QComboBox::currentTextChanged,
        this, &DonorListForm::onFilterByBloodGroup);

    // ---- Donor Table ----
    // Columns: Name | Age | City | Blood Group | Eligible | Last Donation
    tblDonors = new QTableWidget(0, 6, this);
    tblDonors->setHorizontalHeaderLabels(
        { "Name","Age","City","Blood Group","Eligible","Last Donation" });
    tblDonors->horizontalHeader()->setStretchLastSection(true);
    tblDonors->setSelectionBehavior(QAbstractItemView::SelectRows); // Full row selection
    tblDonors->setEditTriggers(QAbstractItemView::NoEditTriggers);   // Read-only
    tblDonors->setSortingEnabled(true); // Click column header to sort

    // ---- Donor Count Label ----
    lblCount = new QLabel("Donors: 0", this);
    lblCount->setObjectName("lblCount");

    // ---- Action Buttons ----
    btnAdd = new QPushButton("➕ Add Donor", this);
    btnViewProfile = new QPushButton("👁 View Profile", this);
    btnDelete = new QPushButton("🗑 Delete", this);

    connect(btnAdd, &QPushButton::clicked, this, &DonorListForm::onAddDonorClicked);
    connect(btnViewProfile, &QPushButton::clicked, this, &DonorListForm::onViewProfileClicked);
    connect(btnDelete, &QPushButton::clicked, this, &DonorListForm::onDeleteClicked);

    // ---- Top Bar: search + filter ----
    QHBoxLayout* topBar = new QHBoxLayout();
    topBar->addWidget(txtSearch);
    topBar->addWidget(new QLabel("Filter:", this));
    topBar->addWidget(cmbFilter);
    topBar->addWidget(lblCount);

    // ---- Button Row ----
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAdd);
    btnRow->addWidget(btnViewProfile);
    btnRow->addWidget(btnDelete);
    btnRow->addStretch();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(10);
    layout->addLayout(topBar);
    layout->addWidget(tblDonors);
    layout->addLayout(btnRow);
    setLayout(layout);
}

// ---- Read all donors from file and populate table ----
void DonorListForm::loadDonors() {
    QList<Donor> donors = FileManager::loadDonors();
    tblDonors->setRowCount(0); // Clear before refilling

    for (const Donor& d : donors) {
        int row = tblDonors->rowCount();
        tblDonors->insertRow(row);

        tblDonors->setItem(row, 0, new QTableWidgetItem(d.getName()));
        tblDonors->setItem(row, 1, new QTableWidgetItem(QString::number(d.getAge())));
        tblDonors->setItem(row, 2, new QTableWidgetItem(d.getCity()));
        tblDonors->setItem(row, 3, new QTableWidgetItem(d.getBloodGroup()));

        // Color-code eligibility
        QTableWidgetItem* eligItem = new QTableWidgetItem(d.isEligible() ? "✅ Yes" : "❌ No");
        eligItem->setForeground(d.isEligible() ? QColor("#27ae60") : QColor("#e74c3c"));
        tblDonors->setItem(row, 4, eligItem);

        QString lastDate = d.getLastDonationDate().isValid()
            ? d.getLastDonationDate().toString("dd-MM-yyyy") : "First time";
        tblDonors->setItem(row, 5, new QTableWidgetItem(lastDate));
    }
    lblCount->setText("Donors: " + QString::number(donors.size()));
}

// ---- Live search: hide rows that don't match ----
void DonorListForm::onSearchChanged(const QString& text) {
    filterTable(text, cmbFilter->currentText());
}

void DonorListForm::onFilterByBloodGroup(const QString& bg) {
    filterTable(txtSearch->text(), bg);
}

// ---- Hide rows that don't match BOTH search text and blood group ----
void DonorListForm::filterTable(const QString& text, const QString& bloodGroup) {
    for (int row = 0; row < tblDonors->rowCount(); row++) {
        bool nameMatch = tblDonors->item(row, 0)->text()
            .contains(text, Qt::CaseInsensitive);
        bool bgMatch = (bloodGroup == "All Groups") ||
            tblDonors->item(row, 3)->text() == bloodGroup;
        // Hide row if either condition fails
        tblDonors->setRowHidden(row, !(nameMatch && bgMatch));
    }
}

// ---- Delete the selected donor ----
void DonorListForm::onDeleteClicked() {
    int row = tblDonors->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a donor to delete.");
        return;
    }
    QString name = tblDonors->item(row, 0)->text();
    auto reply = QMessageBox::question(this, "Confirm Delete",
        "Are you sure you want to delete donor: " + name + "?");
    if (reply == QMessageBox::Yes) {
        FileManager::deleteDonor(name); // Remove from donors.txt
        loadDonors(); // Refresh table
    }
}

// ---- Show detailed donor info + blood compatibility in popup ----
void DonorListForm::onViewProfileClicked() {
    int row = tblDonors->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a donor first.");
        return;
    }
    QString name = tblDonors->item(row, 0)->text();
    // Find full donor from file
    for (const Donor& d : FileManager::loadDonors()) {
        if (d.getName() == name) {
            // Get compatible recipient blood groups using CompatibilityChecker
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

void DonorListForm::onAddDonorClicked() {
    DonorRegistrationForm* form = new DonorRegistrationForm();
    form->show();
    // Refresh table after the form is closed
    connect(form, &QObject::destroyed, this, &DonorListForm::loadDonors);
}

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
