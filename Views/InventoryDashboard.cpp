#include "InventoryDashboard.h"
#include "../Utilities/FileManager.h"
#include "../Models/BloodBag.h"
#include <QProgressBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>   // REQUIRED: cells in tblInventory
#include <QAbstractItemView>  // REQUIRED: NoEditTriggers
#include <QFont>              // REQUIRED: bold font for blood group column
#include <QColor>             // REQUIRED: cell foreground color for expired bags

// ============================================================
// InventoryDashboard.cpp  |  Domain 2: Zara Shah
// ============================================================

InventoryDashboard::InventoryDashboard(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadInventory();
    applyStyle();
}
InventoryDashboard::~InventoryDashboard() {}

void InventoryDashboard::setupUI() {
    // ---- Inventory Table ----
    // Columns: Blood Group | Units Available | Stock Level (Progress) | Expired Bags
    tblInventory = new QTableWidget(0, 4, this);
    tblInventory->setHorizontalHeaderLabels(
        {"Blood Group", "Units Available", "Stock Level", "Expired Bags"});
    tblInventory->horizontalHeader()->setStretchLastSection(true);
    tblInventory->setEditTriggers(QAbstractItemView::NoEditTriggers);

    lblSummary = new QLabel("", this);
    lblSummary->setObjectName("lblSummary");
    lblSummary->setAlignment(Qt::AlignCenter);

    btnRefresh        = new QPushButton("🔄 Refresh",         this);
    btnRemoveExpired  = new QPushButton("🗑 Remove Expired",   this);
    btnExportCSV      = new QPushButton("📊 Export CSV",       this);

    connect(btnRefresh,       &QPushButton::clicked, this, &InventoryDashboard::onRefreshClicked);
    connect(btnRemoveExpired, &QPushButton::clicked, this, &InventoryDashboard::onRemoveExpiredClicked);
    connect(btnExportCSV,     &QPushButton::clicked, this, &InventoryDashboard::onExportInventoryCSV);

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

// ---- Load and display inventory data ----
void InventoryDashboard::loadInventory() {
    // Use BloodInventory (AGGREGATION class) to get data
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    QMap<QString, int> inventoryMap = inventory.getInventoryMap();

    // Count expired bags per blood group
    QMap<QString, int> expiredCount;
    for (const BloodBag& bag : inventory.getExpiredBags()) {
        expiredCount[bag.getBloodGroup()]++;
    }

    tblInventory->setRowCount(0);
    int lowStockCount = 0;

    // One row per blood group
    for (auto it = inventoryMap.begin(); it != inventoryMap.end(); ++it) {
        int row = tblInventory->rowCount();
        tblInventory->insertRow(row);

        QString bg    = it.key();
        int     units = it.value();
        bool    isLow = inventory.isLowStock(bg);
        if (isLow) lowStockCount++;

        // Column 0: Blood group label
        QTableWidgetItem* bgItem = new QTableWidgetItem(bg);
        bgItem->setFont(QFont("Arial", 12, QFont::Bold));
        tblInventory->setItem(row, 0, bgItem);

        // Column 1: Unit count
        tblInventory->setItem(row, 1, new QTableWidgetItem(QString::number(units) + " units"));

        // Column 2: QProgressBar (visual stock indicator)
        // Max = 20 units = "full". Adjust this as needed.
        QProgressBar* bar = new QProgressBar();
        bar->setRange(0, 20);
        bar->setValue(qMin(units, 20));
        bar->setTextVisible(true);
        bar->setFormat(QString::number(units) + " units");

        // Color: green = ok, orange = low, red = critical
        QString color;
        if      (units == 0) color = "#e74c3c"; // Red = out of stock
        else if (isLow)      color = "#f39c12"; // Orange = low
        else                 color = "#27ae60"; // Green = ok
        bar->setStyleSheet(
            "QProgressBar { border: 1px solid #ccc; border-radius: 4px; }"
            "QProgressBar::chunk { background: " + color + "; border-radius: 4px; }");
        tblInventory->setCellWidget(row, 2, bar);

        // Column 3: Expired bag count
        int expired = expiredCount.value(bg, 0);
        QTableWidgetItem* expItem = new QTableWidgetItem(
            expired > 0 ? QString::number(expired) + " ⚠️" : "None");
        if (expired > 0) expItem->setForeground(QColor("#e74c3c"));
        tblInventory->setItem(row, 3, expItem);
    }

    // Update summary
    if (lowStockCount > 0) {
        lblSummary->setText("⚠️  " + QString::number(lowStockCount) +
                            " blood group(s) are LOW STOCK — take action!");
        lblSummary->setStyleSheet("color: #e74c3c; font-weight: bold;");
    } else {
        lblSummary->setText("✅  All blood groups have sufficient stock.");
        lblSummary->setStyleSheet("color: #27ae60; font-weight: bold;");
    }
}

void InventoryDashboard::onRefreshClicked() {
    loadInventory();
}

// ---- Remove all expired bags from inventory.txt ----
void InventoryDashboard::onRemoveExpiredClicked() {
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    inventory.removeExpiredBags();
    inventory.save(FileManager::INVENTORY_FILE);
    loadInventory(); // Refresh the display
    QMessageBox::information(this, "Done", "Expired blood bags have been removed.");
}

// ---- Export Inventory to CSV ----
void InventoryDashboard::onExportInventoryCSV() {
    QString filePath = QFileDialog::getSaveFileName(
        this, "Export Inventory", "inventory_export.csv", "CSV Files (*.csv)");
    if (filePath.isEmpty()) return;

    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    FileManager::exportInventoryToCSV(filePath, inventory.getInventoryMap());
    QMessageBox::information(this, "Exported", "Inventory exported to:\n" + filePath);
}

void InventoryDashboard::applyStyle() {
    setStyleSheet(R"(
        QWidget { background: #fdfdfd; font-family: Arial; font-size: 13px; }
        QLabel { font-size: 14px; font-weight: bold; color: #2c3e50; }
        #lblSummary { font-size: 13px; padding: 6px; border-radius: 5px; }
        QTableWidget { border: 1px solid #ddd; }
        QHeaderView::section { background: #8e44ad; color: white; padding: 7px; font-weight: bold; }
        QPushButton {
            background: #8e44ad; color: white;
            border-radius: 6px; padding: 8px 14px; font-weight: bold;
        }
        QPushButton:hover { background: #9b59b6; }
    )");
}
