#include "InventoryDashboard.h"//header file for InventoryDashboard class
#include "../Utilities/FileManager.h"//header file for FileManager class
#include "../Models/BloodBag.h"//header file for BloodBag class
#include <QProgressBar>//for stock level bars
#include <QFileDialog>//for export dialog
#include <QMessageBox>//for info popups
#include <QHeaderView>//for table header styling
#include <QTableWidgetItem>//for table items
#include <QAbstractItemView>//for disabling table editing
#include <QColor>//for custom colors
#include <QString>//used only at Qt UI boundary via fromStdString
#include <string>//for std::string used throughout

InventoryDashboard::InventoryDashboard(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadInventory();
    applyStyle();
}
InventoryDashboard::~InventoryDashboard() {}

void InventoryDashboard::setupUI() {
    tblInventory = new QTableWidget(0, 4, this);
    tblInventory->setHorizontalHeaderLabels(
        { "Blood Group", "Units Available", "Stock Level", "Expired Bags" });
    tblInventory->horizontalHeader()->setStretchLastSection(true);
    tblInventory->setEditTriggers(QAbstractItemView::NoEditTriggers);

    lblSummary = new QLabel("", this);
    lblSummary->setObjectName("lblSummary");
    lblSummary->setAlignment(Qt::AlignCenter);

    btnRefresh = new QPushButton("🔄 Refresh", this);
    btnRemoveExpired = new QPushButton("🗑 Remove Expired", this);
    btnExportCSV = new QPushButton("📊 Export CSV", this);

    connect(btnRefresh, &QPushButton::clicked, this, &InventoryDashboard::onRefreshClicked);
    connect(btnRemoveExpired, &QPushButton::clicked, this, &InventoryDashboard::onRemoveExpiredClicked);
    connect(btnExportCSV, &QPushButton::clicked, this, &InventoryDashboard::onExportInventoryCSV);

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnRefresh);
    btnRow->addWidget(btnRemoveExpired);
    btnRow->addWidget(btnExportCSV);
    btnRow->addStretch();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(12);
    layout->addWidget(new QLabel("🩸 Blood Inventory Dashboard", this));
    layout->addWidget(lblSummary);
    layout->addWidget(tblInventory);
    layout->addLayout(btnRow);
    setLayout(layout);
}

void InventoryDashboard::loadInventory() {
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    QMap<QString, int> inventoryMap = inventory.getInventoryMap();

    //count expired bags per blood group using std::string comparison
    //QMap key is still QString (from getInventoryMap) but we use int counter with the same keys
    QMap<QString, int> expiredCount;
    for (const BloodBag& bag : inventory.getExpiredBags()) {
        //getBloodGroup() now returns std::string; convert to QString at Qt map boundary
        expiredCount[QString::fromStdString(bag.getBloodGroup())]++;
    }

    tblInventory->setRowCount(0);
    int lowStockCount = 0;

    for (auto it = inventoryMap.begin(); it != inventoryMap.end(); ++it) {
        int row = tblInventory->rowCount();
        tblInventory->insertRow(row);

        QString bg = it.key();
        int     units = it.value();
        //isLowStock takes std::string; convert QString key at the boundary
        bool    isLow = inventory.isLowStock(bg.toStdString());
        if (isLow) lowStockCount++;

        QTableWidgetItem* bgItem = new QTableWidgetItem(bg);
        bgItem->setForeground(QColor("#2c3e50"));
        tblInventory->setItem(row, 0, bgItem);

        QTableWidgetItem* unitItem = new QTableWidgetItem(QString::number(units) + " units");
        unitItem->setForeground(QColor("#2c3e50"));
        tblInventory->setItem(row, 1, unitItem);

        QProgressBar* bar = new QProgressBar();
        bar->setRange(0, 20);
        bar->setValue(qMin(units, 20));
        bar->setTextVisible(true);
        bar->setFormat(QString::number(units) + " units");

        QString color;
        if (units == 0) color = "#e74c3c";
        else if (isLow) color = "#f39c12";
        else            color = "#27ae60";
        bar->setStyleSheet(
            "QProgressBar { border:1px solid #ccc; border-radius:4px;"
            "               background:#ecf0f1; color:#2c3e50; }"
            "QProgressBar::chunk { background:" + color + "; border-radius:4px; }");
        tblInventory->setCellWidget(row, 2, bar);

        int expired = expiredCount.value(bg, 0);
        QTableWidgetItem* expItem = new QTableWidgetItem(
            expired > 0 ? QString::number(expired) + " ⚠️" : "None");
        expItem->setForeground(expired > 0 ? QColor("#e74c3c") : QColor("#27ae60"));
        tblInventory->setItem(row, 3, expItem);
    }

    if (lowStockCount > 0) {
        lblSummary->setText("⚠️  " + QString::number(lowStockCount) +
            " blood group(s) are LOW STOCK — take action!");
        lblSummary->setStyleSheet("color: #e74c3c; font-weight: bold;");
    }
    else {
        lblSummary->setText("✅  All blood groups have sufficient stock.");
        lblSummary->setStyleSheet("color: #27ae60; font-weight: bold;");
    }
}

void InventoryDashboard::onRefreshClicked() { loadInventory(); }

void InventoryDashboard::onRemoveExpiredClicked() {
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    inventory.removeExpiredBags();
    inventory.save(FileManager::INVENTORY_FILE);
    loadInventory();
    QMessageBox::information(this, "Done", "Expired blood bags have been removed.");
}

void InventoryDashboard::onExportInventoryCSV() {
    QString filePath = QFileDialog::getSaveFileName(
        this, "Export Inventory", "inventory_export.csv", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    //exportInventoryToCSV takes std::string path; convert at boundary
    FileManager::exportInventoryToCSV(filePath.toStdString(), inventory.getInventoryMap());
    QMessageBox::information(this, "Exported", "Inventory exported to:\n" + filePath);
}

void InventoryDashboard::applyStyle() {
    setStyleSheet(R"(
        QWidget {
            background-color: #fdfdfd;
            color: #2c3e50;
            font-family: Arial;
            font-size: 13px;
        }
        QLabel {
            font-size: 13px;
            font-weight: bold;
            color: #2c3e50;
            background: transparent;
        }
        #lblSummary { font-size: 13px; padding: 6px; border-radius: 5px; }
        QTableWidget {
            border: 1px solid #dddddd;
            background-color: #ffffff;
            color: #2c3e50;
            gridline-color: #f0f0f0;
        }
        QTableWidget::item { color: #2c3e50; padding: 5px 8px; }
        QTableWidget::item:selected { background: #e8daef; color: #2c3e50; }
        QHeaderView::section {
            background: #8e44ad; color: #ffffff;
            padding: 7px; font-weight: bold; border: none;
        }
        QPushButton {
            background: #8e44ad; color: #ffffff;
            border-radius: 6px; padding: 8px 14px;
            font-weight: bold; min-height: 34px;
        }
        QPushButton:hover { background: #9b59b6; }
    )");
}