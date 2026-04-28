#include <QApplication>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <QString>
#include <QHeaderView>
#include <QColor>

class InventoryDashboard : public QWidget {
    Q_OBJECT

public:
    InventoryDashboard(QWidget *parent = nullptr);

private slots:
    void refreshData();
    void removeExpiredBags();
    void exportToCSV();

private:
    QTableWidget *tableWidget;
    QPushButton *refreshButton;
    QPushButton *removeExpiredButton;
    QPushButton *exportButton;
    void setupUI();
    void loadInventoryData();
};

InventoryDashboard::InventoryDashboard(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadInventoryData();
}

void InventoryDashboard::setupUI() {
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels({"Blood Group", "Available Units", "Total Units"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    refreshButton = new QPushButton("Refresh", this);
    removeExpiredButton = new QPushButton("Remove Expired Bags", this);
    exportButton = new QPushButton("Export to CSV", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    layout->addWidget(refreshButton);
    layout->addWidget(removeExpiredButton);
    layout->addWidget(exportButton);
    setLayout(layout);

    connect(refreshButton, &QPushButton::clicked, this, &InventoryDashboard::refreshData);
    connect(removeExpiredButton, &QPushButton::clicked, this, &InventoryDashboard::removeExpiredBags);
    connect(exportButton, &QPushButton::clicked, this, &InventoryDashboard::exportToCSV);
}

void InventoryDashboard::loadInventoryData() {
    // Example data loaded, this should be replaced with database fetching logic.
    QString bloodGroups[8] = {"A+", "A-", "B+", "B-", "O+", "O-", "AB+", "AB-"};
    int availableUnits[8] = {5, 7, 2, 0, 10, 6, 1, 3}; // Example available units
    int totalUnits[8] = {50, 40, 20, 20, 80, 60, 30, 25}; // Example total units

    tableWidget->setRowCount(8);
    for (int i = 0; i < 8; ++i) {
        tableWidget->setItem(i, 0, new QTableWidgetItem(bloodGroups[i]));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(availableUnits[i])));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(totalUnits[i])));

        // Low stock alert condition
        if (availableUnits[i] < 5) {
            tableWidget->item(i, 1)->setBackground(QColor("red"));
        }
    }
}

void InventoryDashboard::refreshData() {
    // Refresh logic to fetch new data from the database.
    QMessageBox::information(this, "Refresh", "Data refreshed from the database.");
}

void InventoryDashboard::removeExpiredBags() {
    // Logic to remove expired blood bags
    QMessageBox::information(this, "Remove Expired", "Expired bags removed.");
}

void InventoryDashboard::exportToCSV() {
    // Logic for exporting table data to CSV
    QMessageBox::information(this, "Export", "Data exported to CSV.");
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    InventoryDashboard dashboard;
    dashboard.setWindowTitle("Inventory Dashboard");
    dashboard.resize(400, 300);
    dashboard.show();
    return app.exec();
}