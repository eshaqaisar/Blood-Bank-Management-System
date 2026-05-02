#ifndef REQUESTMANAGEMENTFORM_H
#define REQUESTMANAGEMENTFORM_H

#include <QWidget>//for using QWidget as the base class for our form
#include <QTableWidget>//for displaying the list of blood requests in a table format
#include <QPushButton>//for the Approve, Reject, and Refresh buttons
#include <QVBoxLayout>//for arranging the title, table, and buttons vertically
#include <QHBoxLayout>//for arranging the buttons in a horizontal row
#include <QLabel>//for the form title
#include <QStringList>//for QStringList used to keep track of request IDs corresponding to table rows


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
	// We need to keep track of the request IDs in the same order as they appear in the table so we can identify which request is being approved/rejected
    QStringList requestIds;

    void updateInventory(const QString& bloodGroup, int units);
};

#endif // REQUESTMANAGEMENTFORM_H