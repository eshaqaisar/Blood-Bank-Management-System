#ifndef REQUESTMANAGEMENTFORM_H
#define REQUESTMANAGEMENTFORM_H

#include <QWidget>//for QWidget base class
#include <QTableWidget>//for QTableWidget used to display requests
#include <QPushButton>//for QPushButton used for action buttons
#include <QVBoxLayout>//for QVBoxLayout used in the main layout
#include <QHBoxLayout>//for QHBoxLayout used for button arrangement
#include <QLabel>//for QLabel used for the title
#include <QStringList>//for QStringList used to track request IDs



class RequestManagementForm : public QWidget
{
    Q_OBJECT

public:
    explicit RequestManagementForm(QWidget* parent = nullptr);
    ~RequestManagementForm();
    void loadRequests();

private slots:
    void onApproveClicked();
    void onRejectClicked();
    void onRefreshClicked();

private:
    QTableWidget* requestsTable;
    QPushButton* approveBtn;
    QPushButton* rejectBtn;
    QPushButton* refreshBtn;
	//we need to keep track of the request IDs in the same order as they appear in the table, since the table doesn't store the ID directly (only visible columns)
    QStringList requestIds;

    void updateInventory(const QString& bloodGroup, int units);
};

#endif // REQUESTMANAGEMENTFORM_H