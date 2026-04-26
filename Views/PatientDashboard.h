#pragma once
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class PatientDashboard : public QWidget {
    Q_OBJECT

private:
    QLabel* lblTitle;
    QTableWidget* tblRequests;
    QPushButton* btnNewRequest;
    QPushButton* btnRefresh;

public:
    PatientDashboard(QWidget* parent = nullptr);

public slots:
    void onNewRequestClicked();
    void onRefreshClicked();
    void loadRequests();
};