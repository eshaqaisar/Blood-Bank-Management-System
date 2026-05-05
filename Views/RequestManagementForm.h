#ifndef REQUESTMANAGEMENTFORM_H
#define REQUESTMANAGEMENTFORM_H

#include <QWidget>//for QWidget base class
#include <QTableWidget>//for QTableWidget used to display requests
#include <QPushButton>//for QPushButton used for action buttons
#include <QVBoxLayout>//for QVBoxLayout used in the main layout
#include <QHBoxLayout>//for QHBoxLayout used for button arrangement
#include <QLabel>//for QLabel used for the title
#include <QComboBox>//for blood group filter dropdown
#include <string>//std::string replaces QString for request IDs
// QStringList replaced with a fixed-size plain array of std::string
#define MAX_REQUESTS 200

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
    //FIX: blood group filter crash — safe slot that calls loadRequests with current filter
    void onFilterChanged();

private:
    QTableWidget* requestsTable;
    QPushButton* approveBtn;
    QPushButton* rejectBtn;
    QPushButton* refreshBtn;
    QComboBox* cmbFilter;   //blood group filter dropdown added for admin

    //QStringList requestIds replaced with fixed-size plain array + count
    std::string requestIds[MAX_REQUESTS];
    int         requestCount; //how many IDs are currently tracked

    //updateInventory removed: approve now calls FileManager::updateRequestStatus
    //which handles inventory deduction atomically and returns false on failure
};

#endif // REQUESTMANAGEMENTFORM_H