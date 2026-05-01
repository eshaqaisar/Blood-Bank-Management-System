#pragma once
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class PatientDashboard : public QWidget {
    Q_OBJECT

private:
    QString currentUsername;   // ✅ ADD THIS
    QLabel* lblTitle;
    QTableWidget* tblRequests;
    QPushButton* btnNewRequest;
    QPushButton* btnRefresh;

public:
    PatientDashboard(const QString& username, QWidget* parent = nullptr); // ✅ FIXED

public slots:
    void onNewRequestClicked();
    void onRefreshClicked();
    void loadRequests();
};