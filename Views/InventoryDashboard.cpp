#include "InventoryDashboard.h"//header file for the InventoryDashboard class
#include "../Utilities/FileManager.h"//for loading inventory data from the file and exporting to CSV
#include "../Models/BloodBag.h"//for the BloodBag data structure and operations, including counting expired bags and checking stock levels
#include <QProgressBar>//for the visual stock level indicators in the inventory table
#include <QFileDialog>//for file dialog used in CSV export
#include <QMessageBox>//for showing message boxes to the user (e.g., success or error messages)
#include <QHeaderView>//for customizing the appearance of the table headers
#include <QTableWidgetItem> //for populating the inventory table with items and customizing their appearance (e.g., color-coding stock levels and expired bag counts)
#include <QAbstractItemView> //for setting the table to be read-only and customizing selection behavior
#include <QFont>  //for customizing font styles in the UI, such as making the title bold and larger
#include <QColor> //for setting colors in the UI, such as color-coding stock levels and expired bag counts      



InventoryDashboard::InventoryDashboard(QWidget* parent) : QWidget(parent) {
    setupUI();
    loadInventory();
    applyStyle();
}
InventoryDashboard::~InventoryDashboard() {}
//set up the user interface elements for the inventory dashboard, including the inventory table, summary label, and action buttons. The layout is organized to provide a clean and user-friendly experience for managing the blood inventory.
void InventoryDashboard::setupUI() {
	//set the window title and a reasonable default size for the inventory dashboard, along with a minimum size to prevent it from being too small
    tblInventory = new QTableWidget(0, 4, this);
    tblInventory->setHorizontalHeaderLabels(
        {"Blood Group", "Units Available", "Stock Level", "Expired Bags"});
    tblInventory->horizontalHeader()->setStretchLastSection(true);
    tblInventory->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//summary label to display alerts about low stock levels, which updates dynamically based on the inventory data. This provides immediate feedback to the user about the status of the blood inventory and encourages timely action when certain blood groups are running low.
    lblSummary = new QLabel("", this);
    lblSummary->setObjectName("lblSummary");
    lblSummary->setAlignment(Qt::AlignCenter);
	//action buttons for managing the inventory: "Refresh" to reload the inventory data from the file and update the display, "Remove Expired" to delete all expired blood bags from the inventory file and refresh the display, and "Export CSV" to export the current inventory data to a CSV file for reporting or analysis. Each button is connected to its respective slot for handling the corresponding action when clicked.
    btnRefresh        = new QPushButton("🔄 Refresh",         this);
    btnRemoveExpired  = new QPushButton("🗑 Remove Expired",   this);
    btnExportCSV      = new QPushButton("📊 Export CSV",       this);
	//connect buttons to their respective slots for handling clicks
    connect(btnRefresh,       &QPushButton::clicked, this, &InventoryDashboard::onRefreshClicked);
    connect(btnRemoveExpired, &QPushButton::clicked, this, &InventoryDashboard::onRemoveExpiredClicked);
    connect(btnExportCSV,     &QPushButton::clicked, this, &InventoryDashboard::onExportInventoryCSV);
	//layout the dashboard using a vertical layout to stack the title, summary label, inventory table, and action buttons in a clean and organized manner. The action buttons are arranged in a horizontal layout below the inventory table for easy access. The overall layout includes spacing and margins for better visual separation between elements.
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addWidget(btnRefresh);
    btnRow->addWidget(btnRemoveExpired);
    btnRow->addWidget(btnExportCSV);
    btnRow->addStretch();
	//overall layout for the dashboard
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(12);
    layout->addWidget(new QLabel("🩸 Blood Inventory Dashboard", this));
    layout->addWidget(lblSummary);
    layout->addWidget(tblInventory);
    layout->addLayout(btnRow);
    setLayout(layout);
}

//load inventory data from the file and populate the inventory table. This method uses the BloodInventory class (which is an AGGREGATION class) to load the inventory data and count expired bags. It then populates the table with one row per blood group, showing the blood group label, unit count, a visual stock level indicator using a QProgressBar, and the count of expired bags. The method also updates the summary label to alert the user if any blood groups are low on stock.
void InventoryDashboard::loadInventory() {
	//load inventory data from the file using the BloodInventory class, which provides methods to get the inventory map and count expired bags. The inventory data is stored in a QMap where the key is the blood group and the value is the number of units available.
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    QMap<QString, int> inventoryMap = inventory.getInventoryMap();

    //count expired bags per blood group
    QMap<QString, int> expiredCount;
    for (const BloodBag& bag : inventory.getExpiredBags())
    {
        expiredCount[bag.getBloodGroup()]++;
    }
	//clear existing rows in the table before populating with new data
    tblInventory->setRowCount(0);
    int lowStockCount = 0;

    //one row per blood group
    for (auto it = inventoryMap.begin(); it != inventoryMap.end(); ++it) 
    {
        int row = tblInventory->rowCount();
        tblInventory->insertRow(row);

        QString bg    = it.key();
        int     units = it.value();
        bool    isLow = inventory.isLowStock(bg);
        if (isLow) lowStockCount++;

        //column 0: blood group label
        QTableWidgetItem* bgItem = new QTableWidgetItem(bg);
        bgItem->setFont(QFont("Arial", 12, QFont::Bold));
        tblInventory->setItem(row, 0, bgItem);

        //column 1: unit count
        tblInventory->setItem(row, 1, new QTableWidgetItem(QString::number(units) + " units"));

        //column 2: QProgressBar (visual stock indicator)
	   //the progress bar shows the stock level visually, with a range of 0 to 20 units. The color of the bar changes based on the stock level: green for sufficient stock, orange for low stock, and red for out of stock. The text on the bar shows the exact unit count for clarity.
        QProgressBar* bar = new QProgressBar();
        bar->setRange(0, 20);
        bar->setValue(qMin(units, 20));
        bar->setTextVisible(true);
        bar->setFormat(QString::number(units) + " units");

        //color: green = ok, orange = low, red = critical
        QString color;
        if      (units == 0)
            color = "#e74c3c"; // Red = out of stock
        else if (isLow)      
            color = "#f39c12"; // Orange = low
        else                
            color = "#27ae60"; // Green = ok
        bar->setStyleSheet(
            "QProgressBar { border: 1px solid #ccc; border-radius: 4px; }"
            "QProgressBar::chunk { background: " + color + "; border-radius: 4px; }");
        tblInventory->setCellWidget(row, 2, bar);

        //column 3: expired bag count
        int expired = expiredCount.value(bg, 0);
        QTableWidgetItem* expItem = new QTableWidgetItem(
            expired > 0 ? QString::number(expired) + " ⚠️" : "None");
        if (expired > 0) expItem->setForeground(QColor("#e74c3c"));
        tblInventory->setItem(row, 3, expItem);
    }

    //update summary
	if (lowStockCount > 0)//if there are any blood groups that are low on stock, show an alert message in the summary label with the count of low stock groups and style it in red and bold to draw attention to the issue. This encourages the user to take action to replenish the inventory for those blood groups.
    {
        lblSummary->setText("⚠️  " + QString::number(lowStockCount) +
                            " blood group(s) are LOW STOCK — take action!");
        lblSummary->setStyleSheet("color: #e74c3c; font-weight: bold;");
    } else
    {
        lblSummary->setText("✅  All blood groups have sufficient stock.");
        lblSummary->setStyleSheet("color: #27ae60; font-weight: bold;");
    }
}
//slot that is called when the "Refresh" button is clicked, it calls the loadInventory method to reload the inventory data from the file and update the display. This allows the user to see the most up-to-date inventory information after making changes or to refresh the view if they suspect it may be outdated.
void InventoryDashboard::onRefreshClicked() {
    loadInventory();
}

//slot that is called when the "Remove Expired" button is clicked, it creates an instance of the BloodInventory class, loads the inventory data from the file, calls the removeExpiredBags method to delete all expired blood bags from the inventory, saves the updated inventory back to the file, and then calls loadInventory to refresh the display. Finally, it shows a message box to inform the user that the expired blood bags have been removed.
void InventoryDashboard::onRemoveExpiredClicked() {
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    inventory.removeExpiredBags();
    inventory.save(FileManager::INVENTORY_FILE);
    loadInventory(); //refresh the display
    QMessageBox::information(this, "Done", "Expired blood bags have been removed.");
}
//slot that is called when the "Export CSV" button is clicked, it opens a file dialog to let the user choose where to save the CSV file, and if a valid file path is selected, it loads the inventory data from the file and uses the FileManager utility to export the inventory data to a CSV file at the chosen location. Finally, it shows a message box to inform the user that the export was successful and provides the file path of the exported CSV.
void InventoryDashboard::onExportInventoryCSV() {
    QString filePath = QFileDialog::getSaveFileName(
        this, "Export Inventory", "inventory_export.csv", "CSV Files (*.csv)");
	if (filePath.isEmpty()) //user cancelled
        return;
	//load inventory data and export to CSV using the FileManager utility, which handles the file operations for exporting data. The inventory data is retrieved as a map and passed to the exportInventoryToCSV method, which formats it as a CSV file and saves it to the specified location.
    BloodInventory inventory;
    inventory.load(FileManager::INVENTORY_FILE);
    FileManager::exportInventoryToCSV(filePath, inventory.getInventoryMap());
    QMessageBox::information(this, "Exported", "Inventory exported to:\n" + filePath);
}
//apply custom styles to the inventory dashboard using a style sheet. This method sets the background color, font styles, and colors for various UI elements such as labels, tables, progress bars, and buttons to create a cohesive and visually appealing design for the inventory dashboard. It uses a combination of inline styles and object names to target specific elements like the summary label and the table headers.
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
