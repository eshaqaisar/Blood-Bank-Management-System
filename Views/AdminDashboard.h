#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>//base class for all UI elements in Qt
#include <QStackedWidget>//for managing multiple pages in the dashboard
#include <QPushButton>//for clickable buttons in the navigation and actions
#include <QLabel>//for displaying text such as welcome message and alerts
#include <QHBoxLayout>//for horizontal layout of the top bar and main content area
#include <QVBoxLayout>//for vertical layout of the sidebar and overall dashboard
#include <string>//std::string replaces QString for adminUsername member variable

//forward declarations
class DonorListForm;
class InventoryDashboard;
class RequestManagementForm;

//admin dashboard class that provides an interface for managing donors, inventory, and requests in the blood bank system.
class AdminDashboard : public QWidget
{
    Q_OBJECT

public:
    //constructor now takes std::string; converts to QString only at UI boundary
    explicit AdminDashboard(const std::string& adminUsername, QWidget* parent = nullptr);
    ~AdminDashboard();

private slots:
    void showDonorList();         //switch to page 0
    void showInventory();         //switch to page 1
    void showRequestManagement(); //switch to page 2
    void toggleDarkMode();        //swap between dark/light QSS
    void exportCSV();             //export donors to CSV file
    void onLogout();              //return to LandingPage

private:
    std::string adminUsername;    //std::string replaces QString
    bool        isDarkMode = false;

    //ui elements
    QStackedWidget* stackedWidget;

    //sidebar buttons for navigation and actions
    QPushButton* btnDonors;
    QPushButton* btnInventory;
    QPushButton* btnRequests;
    QPushButton* btnToggleDark;
    QPushButton* btnExportCSV;
    QPushButton* btnLogout;

    //labels for welcome message and low stock alert
    QLabel* lblWelcome;
    QLabel* lblAlert;

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