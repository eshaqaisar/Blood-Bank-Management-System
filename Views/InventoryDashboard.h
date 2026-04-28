#ifndef INVENTORYDASHBOARD_H
#define INVENTORYDASHBOARD_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>

class InventoryDashboard : public QWidget
{
    Q_OBJECT

public:
    InventoryDashboard(QWidget *parent = nullptr);
    void BloodInventory();
    
private:
    void setupUI();
    void applyStyle();
    void loadInventory();
    void refreshDisplay();
    void updateTable();

    QTableWidget *inventoryTable;
    QLabel *summaryLabel;
    QPushButton *refreshButton;
    QPushButton *removeExpiredButton;
    QPushButton *exportCSVButton;
};

#endif // INVENTORYDASHBOARD_H
