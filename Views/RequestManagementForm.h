#ifndef REQUESTMANAGEMENTFORM_H
#define REQUESTMANAGEMENTFORM_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class RequestManagementForm : public QWidget
{
    Q_OBJECT

public:
    explicit RequestManagementForm(QWidget *parent = nullptr);
    ~RequestManagementForm();
    void loadRequests();

private slots:
    void onApproveClicked();
    void onRejectClicked();

private:
    QTableWidget *requestsTable;
    QPushButton *approveBtn;
    QPushButton *rejectBtn;
    void updateInventory(QString bloodGroup, int units);
};

#endif // REQUESTMANAGEMENTFORM_H