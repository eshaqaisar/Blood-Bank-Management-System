#ifndef INVENTORYDASHBOARD_H
#define INVENTORYDASHBOARD_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

// ============================================================
// InventoryDashboard.h  |  Domain 2: Zara Shah
// ============================================================
//
// Shows all 8 blood groups, their stock levels, and expiry info.
// Features:
//   - QProgressBar per blood group (visual stock indicator)
//   - Color: Green = OK, Yellow = Low, Red = Critical
//   - One-click "Remove Expired" button
//   - One-click Inventory CSV Export
// ============================================================

class InventoryDashboard : public QWidget {
    Q_OBJECT

public:
    explicit InventoryDashboard(QWidget* parent = nullptr);
    ~InventoryDashboard();

private slots:
    void onRefreshClicked();
    void onRemoveExpiredClicked();
    void onExportInventoryCSV();

private:
    QTableWidget* tblInventory; // Main inventory table
    QPushButton*  btnRefresh;
    QPushButton*  btnRemoveExpired;
    QPushButton*  btnExportCSV;
    QLabel*       lblSummary;   // e.g. "3 blood groups are low stock"

    void setupUI();
    void loadInventory();       // Read inventory.txt and fill table
    void applyStyle();
};

#endif // INVENTORYDASHBOARD_H
