#ifndef INVENTORYDASHBOARD_H
#define INVENTORYDASHBOARD_H

#include <QWidget>//base class for all UI elements in Qt
#include <QTableWidget>//for displaying the inventory data in a tabular format
#include <QPushButton>//for action buttons like "Refresh", "Remove Expired", and "Export CSV"
#include <QLabel>//for displaying the summary of inventory status, such as low stock alerts
#include <QVBoxLayout>//for organizing the layout of the dashboard in a vertical manner

class InventoryDashboard : public QWidget {
    Q_OBJECT

public:
    explicit InventoryDashboard(QWidget* parent = nullptr);
    ~InventoryDashboard();

private slots:
    void onRefreshClicked();       //slot that is called when the "Refresh" button is clicked, reloads inventory from file
    void onRemoveExpiredClicked(); //slot that removes expired bags from inventory and saves
    void onExportInventoryCSV();   //slot that opens file dialog and exports inventory to CSV

private:
    QTableWidget* tblInventory;
    QPushButton* btnRefresh;
    QPushButton* btnRemoveExpired;
    QPushButton* btnExportCSV;
    QLabel* lblSummary;

    void setupUI();
    void loadInventory();
    void applyStyle();
};

#endif // INVENTORYDASHBOARD_H