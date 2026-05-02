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

// DonorListForm.cpp  |  Domain 1: Muhammad Ali

DonorListForm::DonorListForm(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadDonors();
    applyStyle();
}
DonorListForm::~DonorListForm() {}

void DonorListForm::setupUI() {
    txtSearch = new QLineEdit(this);
    txtSearch->setPlaceholderText("🔍 Search by name...");
    connect(txtSearch, &QLineEdit::textChanged, this, &DonorListForm::onSearchChanged);

    cmbFilter = new QComboBox(this);
    cmbFilter->addItem("All Groups");
    cmbFilter->addItems({ "A+","A-","B+","B-","AB+","AB-","O+","O-" });
    connect(cmbFilter, &QComboBox::currentTextChanged, this, &DonorListForm::onFilterByBloodGroup);

    tblDonors = new QTableWidget(0, 6, this);
    tblDonors->setHorizontalHeaderLabels(
        { "Name","Age","City","Blood Group","Eligible","Last Donation" });
    tblDonors->horizontalHeader()->setStretchLastSection(true);
    tblDonors->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblDonors->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblDonors->setSortingEnabled(true);

    lblCount = new QLabel("Donors: 0", this);
    lblCount->setObjectName("lblCount");

    btnAdd = new QPushButton("➕ Add Donor", this);
    btnViewProfile = new QPushButton("👁 View Profile", this);
    btnDelete = new QPushButton("🗑 Delete", this);

    connect(btnAdd, &QPushButton::clicked, this, &DonorListForm::onAddDonorClicked);
    connect(btnViewProfile, &QPushButton::clicked, this, &DonorListForm::onViewProfileClicked);
    connect(btnDelete, &QPushButton::clicked, this, &DonorListForm::onDeleteClicked);

    QHBoxLayout* topBar = new QHBoxLayout();
    topBar->addWidget(txtSearch);
    topBar->addWidget(new QLabel("Filter:", this));
    topBar->addWidget(cmbFilter);
    topBar->addWidget(lblCount);

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

void DonorListForm::loadDonors() {
    QList<Donor> donors = FileManager::loadDonors();
    tblDonors->setRowCount(0);

    for (const Donor& d : donors) {
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

void DonorListForm::onSearchChanged(const QString& text) {
    filterTable(text, cmbFilter->currentText());
}

void DonorListForm::onFilterByBloodGroup(const QString& bg) {
    filterTable(txtSearch->text(), bg);
}

void DonorListForm::filterTable(const QString& text, const QString& bloodGroup) {
    for (int row = 0; row < tblDonors->rowCount(); row++) {
        bool nameMatch = tblDonors->item(row, 0)->text().contains(text, Qt::CaseInsensitive);
        bool bgMatch = (bloodGroup == "All Groups") ||
            tblDonors->item(row, 3)->text() == bloodGroup;
        tblDonors->setRowHidden(row, !(nameMatch && bgMatch));
    }
}

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
        FileManager::deleteDonor(name);
        loadDonors();
    }
}

void DonorListForm::onViewProfileClicked() {
    int row = tblDonors->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a donor first.");
        return;
    }
    QString name = tblDonors->item(row, 0)->text();
    for (const Donor& d : FileManager::loadDonors()) {
        if (d.getName() == name) {
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
    connect(form, &QObject::destroyed, this, &DonorListForm::loadDonors);
}

void DonorListForm::applyStyle() {
    // ✅ FIX: explicit color:#2c3e50 on QWidget, QLabel, QTableWidget::item
    //         min-height on inputs so they stay usable when window shrinks
    setStyleSheet(R"(
        QWidget {
            background-color: #fdfdfd;
            color: #2c3e50;
            font-family: Arial;
            font-size: 13px;
        }
        QLabel {
            color: #2c3e50;
            background: transparent;
        }
        #lblCount { color: #888888; font-size: 12px; }
        QLineEdit {
            background-color: #ffffff;
            color: #2c3e50;
            padding: 7px 10px;
            border: 1px solid #cccccc;
            border-radius: 5px;
            min-height: 32px;
        }
        QLineEdit:focus { border: 2px solid #c0392b; }
        QComboBox {
            background-color: #ffffff;
            color: #2c3e50;
            padding: 6px 10px;
            border: 1px solid #cccccc;
            border-radius: 5px;
            min-height: 32px;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            color: #2c3e50;
            selection-background-color: #fadbd8;
        }
        QTableWidget {
            border: 1px solid #dddddd;
            gridline-color: #f0f0f0;
            background-color: #ffffff;
            color: #2c3e50;
        }
        QTableWidget::item { color: #2c3e50; padding: 5px 8px; }
        QTableWidget::item:selected { background: #fadbd8; color: #2c3e50; }
        QHeaderView::section {
            background: #c0392b; color: #ffffff;
            padding: 7px; font-weight: bold; border: none;
        }
        QPushButton {
            background: #c0392b;
            color: #ffffff;
            border-radius: 6px;
            padding: 8px 14px;
            font-weight: bold;
            min-height: 34px;
        }
        QPushButton:hover { background: #e74c3c; }
    )");
}