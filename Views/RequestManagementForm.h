#ifndef REQUESTMANAGEMENTFORM_H
#define REQUESTMANAGEMENTFORM_H

#include <QWidget>//base class for all UI elements in Qt
#include <QTableWidget>//for displaying the list of blood requests in a tabular format
#include <QPushButton>//for action buttons like "Approve" and "Reject"
#include <QVBoxLayout>//for organizing the layout of the form in a vertical manner
#include <QHBoxLayout>//for organizing the layout of the action buttons in a horizontal manner
#include <QLabel>//for displaying the title of the form and any status messages

class RequestManagementForm : public QWidget
{
    Q_OBJECT

public:
    explicit RequestManagementForm(QWidget *parent = nullptr);
    ~RequestManagementForm();
    void loadRequests();

private slots:
	void onApproveClicked();//slot that is called when the "Approve" button is clicked, it checks if a request is selected in the table, retrieves the details of the selected request, updates the inventory by deducting the requested units from the appropriate blood group, generates an approval certificate for the patient (which could be saved to a file or displayed in a new window), and then removes the approved request from the table and updates the requests file accordingly. Finally, it shows a message box to inform the user that the request has been approved and the inventory has been updated.
	void onRejectClicked();//slot that is called when the "Reject" button is clicked, it checks if a request is selected in the table, retrieves the details of the selected request, removes the request from the table and updates the requests file accordingly, and optionally shows a message box to inform the user that the request has been rejected. In a complete implementation, this slot could also include functionality to notify the patient of the rejection (e.g., by saving a notification to a file or displaying a message in their dashboard).

private:
	QTableWidget* requestsTable;//table widget to display the list of pending blood requests, with columns for patient name, hospital, blood group, and units requested. The table allows the admin to select a request for approval or rejection.
	QPushButton* approveBtn;//button to approve the selected blood request, which will update the inventory accordingly and generate an approval certificate for the patient. The button is connected to a slot that will handle the click event and perform the necessary actions for approving a request.
	QPushButton* rejectBtn;//button to reject the selected blood request, which will remove the request from the list and optionally notify the patient of the rejection. The button is connected to a slot that will handle the click event and perform the necessary actions for rejecting a request.
	void updateInventory(QString bloodGroup, int units);//helper method to update the inventory when a request is approved, it takes the blood group and the number of units requested as parameters, loads the current inventory data, deducts the requested units from the appropriate blood group, saves the updated inventory back to the file, and refreshes the inventory display if necessary.
};

#endif // REQUESTMANAGEMENTFORM_H