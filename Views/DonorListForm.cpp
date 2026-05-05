#include "DonorListForm.h"//forward declaration of DonorListForm class
#include "DonorRegistrationForm.h"//forward declaration of DonorRegistrationForm class
#include "../Utilities/FileManager.h"//forward declaration of FileManager class
#include "../Utilities/Compatibility.h"//forward declaration of CompatibilityChecker class
#include "../Models/Donor.h"//forward declaration of Donor class
#include <QMessageBox>//for showing message boxes
#include <QHeaderView>//for customizing table headers
#include <QTableWidgetItem>//for working with table items
#include <QAbstractItemView>//for setting selection behavior of the table
#include <QColor>//for setting text colors
#include <QString>//used only at Qt UI boundary via fromStdString / toStdString
#include <string>//for std::string used throughout

DonorListForm::DonorListForm(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadDonors();
    applyStyle();
}
DonorListForm::~DonorListForm() {}

void DonorListForm::setupUI() {
    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("Search by name...");
    connect(txtSearch, &QLineEdit::textChanged, this, &DonorListForm::onSearchChanged);

    cmbFilter = new QComboBox(this);
    cmbFilter->addItem("All Groups");
    cmbFilter->addItem("A+");  cmbFilter->addItem("A-");
    cmbFilter->addItem("B+");  cmbFilter->addItem("B-");
    cmbFilter->addItem("AB+"); cmbFilter->addItem("AB-");
    cmbFilter->addItem("O+");  cmbFilter->addItem("O-");
    connect(cmbFilter, &QComboBox::currentTextChanged, this, &DonorListForm::onFilterByBloodGroup);

    tblDonors = new QTableWidget(0, 6, this);
    tblDonors->setHorizontalHeaderLabels(
        { "Name", "Age", "City", "Blood Group", "Eligible", "Last Donation" });
    tblDonors->horizontalHeader()->setStretchLastSection(true);
    tblDonors->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblDonors->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // FIX 1: Do NOT enable sorting here at construction time.
    // Sorting is enabled only after loadDonors() finishes populating the table,
    // so that auto-sort cannot shift a half-built row and leave nullptr cells.
    tblDonors->setSortingEnabled(false);

    lblCount = new QLabel("Donors: 0", this);
    lblCount->setObjectName("lblCount");

    btnAdd = new QPushButton("Add Donor", this);
    btnViewProfile = new QPushButton("View Profile", this);
    btnDelete = new QPushButton("Delete", this);
    btnRefresh = new QPushButton("Refresh", this);

    connect(btnAdd, &QPushButton::clicked, this, &DonorListForm::onAddDonorClicked);
    connect(btnViewProfile, &QPushButton::clicked, this, &DonorListForm::onViewProfileClicked);
    connect(btnDelete, &QPushButton::clicked, this, &DonorListForm::onDeleteClicked);
    connect(btnRefresh, &QPushButton::clicked, this, &DonorListForm::onRefreshClicked);

    QHBoxLayout* topBar = new QHBoxLayout();
    topBar->addWidget(txtSearch);
    topBar->addWidget(new QLabel("Filter:", this));
    topBar->addWidget(cmbFilter);
    topBar->addWidget(lblCount);

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnAdd);
    btnRow->addWidget(btnViewProfile);
    btnRow->addWidget(btnDelete);
    btnRow->addWidget(btnRefresh);
    btnRow->addStretch();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(10);
    layout->addLayout(topBar);
    layout->addWidget(tblDonors);
    layout->addLayout(btnRow);
    setLayout(layout);
}

void DonorListForm::loadDonors() {
    QList<Donor> donors = FileManager::loadDonors();

    // FIX 1 (continued): Disable sorting before inserting any rows.
    // With sorting enabled, calling setItem(row, 0, ...) triggers an immediate
    // re-sort that shifts the new row to a different index. The subsequent
    // setItem(row, 1..5, ...) calls then write into the wrong row, leaving many
    // cells as nullptr. Disabling sorting during population prevents this entirely.
    tblDonors->setSortingEnabled(false);
    tblDonors->setRowCount(0);

    for (const Donor& d : donors) {
        int row = tblDonors->rowCount();
        tblDonors->insertRow(row);

        // Convert std::string to QString only at Qt table item boundary
        tblDonors->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(d.getName())));
        tblDonors->setItem(row, 1, new QTableWidgetItem(QString::number(d.getAge())));
        tblDonors->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(d.getCity())));
        tblDonors->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(d.getBloodGroup())));

        QTableWidgetItem* eligItem = new QTableWidgetItem(d.isEligible() ? "[OK] Yes" : "[X] No");
        eligItem->setForeground(d.isEligible() ? QColor("#27ae60") : QColor("#e74c3c"));
        tblDonors->setItem(row, 4, eligItem);

        QString lastDate = d.getLastDonationDate().isValid()
            ? d.getLastDonationDate().toString("dd-MM-yyyy") : "First time";
        tblDonors->setItem(row, 5, new QTableWidgetItem(lastDate));
    }

    // Re-enable sorting only after every row is fully built.
    // This is safe now because all cells are guaranteed to be non-null.
    tblDonors->setSortingEnabled(true);

    lblCount->setText("Donors: " + QString::number(donors.size()));
}

// Slot: called when search text changes; converts QString to std::string at boundary
void DonorListForm::onSearchChanged(const QString& text) {
    std::string bg = (cmbFilter->currentText() == "All Groups")
        ? "" : cmbFilter->currentText().toStdString();
    filterTable(text.toStdString(), bg);
}

// Slot: called when blood group filter changes
void DonorListForm::onFilterByBloodGroup(const QString& bg) {
    std::string bgStr = (bg == "All Groups") ? "" : bg.toStdString();
    filterTable(txtSearch->text().toStdString(), bgStr);
}

// Refresh slot: reload donors from disk and repopulate table
void DonorListForm::onRefreshClicked() {
    loadDonors();
}

// filterTable uses std::string params -- no Qt QString comparison needed here
void DonorListForm::filterTable(const std::string& text, const std::string& bloodGroup) {
    for (int row = 0; row < tblDonors->rowCount(); row++) {

        // FIX 2: Guard against nullptr before calling ->text().
        // If Bug 1 had corrupted the table (or any future insertion issue occurs),
        // dereferencing a nullptr QTableWidgetItem* causes an immediate segfault.
        // This is exactly what caused the crash on every search/filter action.
        QTableWidgetItem* nameItem = tblDonors->item(row, 0);
        QTableWidgetItem* bgItem = tblDonors->item(row, 3);
        if (!nameItem || !bgItem) {
            tblDonors->setRowHidden(row, true);
            continue;
        }

        std::string rowName = nameItem->text().toLower().toStdString();
        std::string rowBG = bgItem->text().toStdString();

        // Case-insensitive name search using std::string::find
        std::string lowerText = text;
        for (char& c : lowerText) if (c >= 'A' && c <= 'Z') c += 32;

        bool nameMatch = text.empty() || (rowName.find(lowerText) != std::string::npos);
        bool bgMatch = bloodGroup.empty() || (rowBG == bloodGroup);

        tblDonors->setRowHidden(row, !(nameMatch && bgMatch));
    }
}

void DonorListForm::onDeleteClicked() {
    int row = tblDonors->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a donor to delete.");
        return;
    }
    // Get name as std::string at the boundary
    std::string name = tblDonors->item(row, 0)->text().toStdString();
    auto reply = QMessageBox::question(this, "Confirm Delete",
        "Are you sure you want to delete donor: " + QString::fromStdString(name) + "?");
    if (reply == QMessageBox::Yes) {
        FileManager::deleteDonor(name); // deleteDonor takes std::string
        loadDonors();
    }
}

void DonorListForm::onViewProfileClicked() {
    int row = tblDonors->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a donor first.");
        return;
    }
    std::string name = tblDonors->item(row, 0)->text().toStdString();
    QList<Donor> donors = FileManager::loadDonors();
    for (const Donor& d : donors) {
        if (d.getName() == name) {
            // getCompatibleRecipients returns CompatibleList (replaces QStringList)
            CompatibleList recipients = CompatibilityChecker::getCompatibleRecipients(d.getBloodGroup());

            // Build the recipients string from the plain array without QStringList::join
            std::string recipStr = "";
            for (int i = 0; i < recipients.count; i++) {
                if (i > 0) recipStr += ", ";
                recipStr += recipients.groups[i];
            }

            QString info =
                "Name:        " + QString::fromStdString(d.getName()) + "\n"
                "Age:         " + QString::number(d.getAge()) + "\n"
                "Contact:     " + QString::fromStdString(d.getContact()) + "\n"
                "City:        " + QString::fromStdString(d.getCity()) + "\n"
                "Blood Group: " + QString::fromStdString(d.getBloodGroup()) + "\n"
                "Weight:      " + QString::number(d.getWeight()) + " kg\n"
                "Eligible:    " + (d.isEligible() ? "Yes [OK]" : "No [X]") + "\n\n"
                "Can donate to: " + QString::fromStdString(recipStr);

            QMessageBox::information(this, "Donor Profile -- " + QString::fromStdString(name), info);
            return;
        }
    }
}

void DonorListForm::onAddDonorClicked() {
    DonorRegistrationForm* form = new DonorRegistrationForm();
    form->show();
    // Auto-refresh the table when the registration form closes
    connect(form, &QObject::destroyed, this, &DonorListForm::loadDonors);
}

void DonorListForm::applyStyle() {
    setStyleSheet(R"(
        QWidget {
            background-color: #fdfdfd;
            color: #2c3e50;
            font-family: Arial;
            font-size: 13px;
        }
        QLabel { color: #2c3e50; background: transparent; }
        #lblCount { color: #888888; font-size: 12px; }
        QLineEdit {
            background-color: #ffffff; color: #2c3e50;
            padding: 7px 10px; border: 1px solid #cccccc;
            border-radius: 5px; min-height: 32px;
        }
        QLineEdit:focus { border: 2px solid #c0392b; }
        QComboBox {
            background-color: #ffffff; color: #2c3e50;
            padding: 6px 10px; border: 1px solid #cccccc;
            border-radius: 5px; min-height: 32px;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff; color: #2c3e50;
            selection-background-color: #fadbd8;
        }
        QTableWidget {
            border: 1px solid #dddddd; gridline-color: #f0f0f0;
            background-color: #ffffff; color: #2c3e50;
        }
        QTableWidget::item { color: #2c3e50; padding: 5px 8px; }
        QTableWidget::item:selected { background: #fadbd8; color: #2c3e50; }
        QHeaderView::section {
            background: #c0392b; color: #ffffff;
            padding: 7px; font-weight: bold; border: none;
        }
        QPushButton {
            background: #c0392b; color: #ffffff;
            border-radius: 6px; padding: 8px 14px;
            font-weight: bold; min-height: 34px;
        }
        QPushButton:hover { background: #e74c3c; }
    )");
}