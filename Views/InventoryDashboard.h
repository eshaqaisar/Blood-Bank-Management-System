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
	void onRefreshClicked();//slot that is called when the "Refresh" button is clicked, it calls the loadInventory method to reload the inventory data from the file and update the display. This allows the user to see the most up-to-date inventory information after making changes or to refresh the view if they suspect it may be outdated.
	void onRemoveExpiredClicked();//slot that is called when the "Remove Expired" button is clicked, it creates an instance of the BloodInventory class, loads the inventory data from the file, calls the removeExpiredBags method to delete all expired blood bags from the inventory, saves the updated inventory back to the file, and then calls loadInventory to refresh the display. Finally, it shows a message box to inform the user that the expired blood bags have been removed.
	void onExportInventoryCSV();//slot that is called when the "Export CSV" button is clicked, it opens a file dialog to let the user choose where to save the CSV file, and if a valid file path is selected, it loads the inventory data from the file and uses the FileManager utility to export the inventory data to a CSV file at the chosen location. Finally, it shows a message box to inform the user that the export was successful and provides the file path of the exported CSV.

private:
	QTableWidget* tblInventory; //table widget to display the inventory data, with columns for blood group, units available, stock level (using a progress bar), and expired bag count
    QPushButton*  btnRefresh;
    QPushButton*  btnRemoveExpired;
    QPushButton*  btnExportCSV;
    QLabel*       lblSummary;  

    void setupUI();
	void loadInventory();//load inventory data from the file and populate the inventory table. This method uses the BloodInventory class (which is an AGGREGATION class) to load the inventory data and count expired bags. It then populates the table with one row per blood group, showing the blood group label, unit count, a visual stock level indicator using a QProgressBar, and the count of expired bags. The method also updates the summary label to alert the user if any blood groups are low on stock.
    void applyStyle();
};

#endif // INVENTORYDASHBOARD_H
