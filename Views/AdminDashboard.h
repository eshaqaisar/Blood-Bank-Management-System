#ifndef ADMINDASHBOARD_H // Include guard to prevent multiple definitions
#define ADMINDASHBOARD_H

#include <QWidget>//base class for all UI elements in Qt
#include <QStackedWidget>//for managing multiple pages in the dashboard
#include <QPushButton>//for clickable buttons in the navigation and actions
#include <QLabel>//for displaying text such as welcome message and alerts
#include <QHBoxLayout>//for horizontal layout of the top bar and main content area
#include <QVBoxLayout>//for vertical layout of the sidebar and overall dashboard
#include <QTimer>//for periodic updates, such as refreshing inventory status

//forward declarations
class DonorListForm;
class InventoryDashboard;
class RequestManagementForm;
//admin dashboard class that provides an interface for managing donors, inventory, and requests in the blood bank system. It includes navigation buttons, a welcome message, and an alert for low stock situations. It also has a dark mode toggle and CSV export functionality for the donor list.
class AdminDashboard : public QWidget 
{
    Q_OBJECT

public:
    explicit AdminDashboard(const QString& adminUsername, QWidget* parent = nullptr);
    ~AdminDashboard();

private slots:
    void showDonorList();         //switch to page 0
    void showInventory();         //switch to page 1
    void showRequestManagement(); //switch to page 2
    void toggleDarkMode();        //swap between dark/light QSS
    void exportCSV();             //export donors to CSV file
    void onLogout();              //return to LandingPage

private:
    QString adminUsername;
    bool    isDarkMode = false;   //tracks current theme

	//ui elements
    QStackedWidget* stackedWidget; // The content area

	//sidebar buttons for navigation and actions
    QPushButton* btnDonors;
    QPushButton* btnInventory;
    QPushButton* btnRequests;
    QPushButton* btnToggleDark;
    QPushButton* btnExportCSV;
    QPushButton* btnLogout;

	//labels for welcome message and low stock alert 
    QLabel* lblWelcome;
	QLabel* lblAlert;   //dynamic alert for low stock situations

	//pages for different sections of the dashboard 
    DonorListForm* donorListPage;
    InventoryDashboard* inventoryPage;
    RequestManagementForm* requestsPage;
	//helper functions to set up the UI and apply themes
    void setupUI();
    void applyLightMode();
    void applyDarkMode();
};

#endif // ADMINDASHBOARD_H
