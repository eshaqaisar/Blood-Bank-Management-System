#ifndef DONORDASHBOARD_H
#define DONORDASHBOARD_H

#include <QWidget>//base class for all UI elements in Qt
#include <QLabel>//for displaying text such as donor information and titles
#include <QPushButton>//for the logout button
#include <QTableWidget>//for displaying the donor's donation history in a tabular format
#include <QVBoxLayout>//for organizing the layout of the dashboard in a vertical manner


class DonorDashboard : public QWidget {
    Q_OBJECT

public:
    explicit DonorDashboard(const QString& donorUsername, QWidget* parent = nullptr);
    ~DonorDashboard();

private slots:
    void onLogout();

private:
    QString donorUsername;

	QLabel* lblTitle;//title label for the dashboard
	QLabel* lblName;//label to display the donor's name
	QLabel* lblBloodGroup;//label to display the donor's blood group
	QLabel* lblEligibility;//label to display whether the donor is currently eligible to donate based on their last donation date and the required waiting period
	QLabel* lblLastDonation;//label to display the date of the donor's last donation
	QTableWidget* tblHistory;//table widget to display the donor's donation history, with columns for date and units donated
	QPushButton* btnLogout;//button to allow the donor to log out and return to the landing page

	void setupUI();//initializes the user interface elements and layouts for the donor dashboard
    void loadDonorProfile(); // Reads donor data from donors.txt
	void applyStyle();//applies custom styles to the dashboard, such as colors and fonts, to create a visually appealing and user-friendly interface
};

#endif // DONORDASHBOARD_H
