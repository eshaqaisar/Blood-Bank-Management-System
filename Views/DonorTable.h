#ifndef DONORTABLE_H
#define DONORTABLE_H

#include <QMainWindow>
#include <QTableWidget>
#include "ui_DonorTable.h"

class DonorTable : public QMainWindow
{
    Q_OBJECT

public:
    DonorTable(QWidget* parent = nullptr);
    ~DonorTable();
    void loadData(); // File se data parhne ke 

private:
    Ui::DonorTableClass* ui;
   
    
private slots:
    void filterTable(const QString& arg1);
};

#endif