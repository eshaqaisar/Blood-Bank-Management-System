 #include "DonorTable.h"
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QLineEdit>
#include <QTableWidgetItem>

DonorTable::DonorTable(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::DonorTableClass)
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &DonorTable::filterTable);

    
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({ "Name", "Age", "Weight", "Blood Group" });
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setShowGrid(true); // Partitions dikhane ke liye

    loadData(); 
}

DonorTable::~DonorTable() { delete ui; }

void DonorTable::loadData() {
    QFile file("donors.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        ui->tableWidget->setRowCount(0);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() >= 4) {
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(fields[0]));
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(fields[1]));
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(fields[2]));
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(fields[3]));
            }
        }
        file.close();
    }
}
void DonorTable::filterTable(const QString& text)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        
        QTableWidgetItem* item = ui->tableWidget->item(i, 0);
        if (item) {
            bool match = item->text().contains(text, Qt::CaseInsensitive);
            ui->tableWidget->setRowHidden(i, !match);
        }
    }
}