#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

// AdminDashboard.h 
// Part: Esha Qaisar
// The Admin's central hub. Uses QStackedWidget to switch
// between sub-screens without opening new windows.
// Embedded screens (pages in the stacked widget):
//   Page 0: Donor List / Search
//   Page 1: Inventory Dashboard
//   Page 2: Request Management
//
// Advanced Features:
//   - Dark Mode / Light Mode Toggle (swaps QSS stylesheet)
//   - One-Click CSV Export button
//   - Emergency Alert label (updated by InventoryDashboard)
//
// Qt Concepts:
//   QStackedWidget: holds multiple "pages", shows one at a time.
//   setCurrentIndex(n) switches to page n.


// Forward declarations
class DonorListForm;
class InventoryDashboard;
class RequestManagementForm;

class AdminDashboard : public QWidget 
{
    Q_OBJECT

public:
    explicit AdminDashboard(const QString& adminUsername, QWidget* parent = nullptr);
    ~AdminDashboard();

private slots:
    void showDonorList();         // Switch to page 0
    void showInventory();         // Switch to page 1
    void showRequestManagement(); // Switch to page 2
    void toggleDarkMode();        // Swap between dark/light QSS
    void exportCSV();             // Export donors to CSV file
    void onLogout();              // Return to LandingPage

private:
    QString adminUsername;
    bool    isDarkMode = false;   // Tracks current theme

    //  Layout structure 
    // Left: navigation panel | Right: QStackedWidget content area
    QStackedWidget* stackedWidget; // The content area

    // Navigation Buttons (left sidebar)
    QPushButton* btnDonors;
    QPushButton* btnInventory;
    QPushButton* btnRequests;
    QPushButton* btnToggleDark;
    QPushButton* btnExportCSV;
    QPushButton* btnLogout;

    //  Top bar 
    QLabel* lblWelcome;
    QLabel* lblAlert;   // Shows emergency low stock warning

    // Pages inside QStackedWidget 
    DonorListForm* donorListPage;
    InventoryDashboard* inventoryPage;
    RequestManagementForm* requestsPage;

    void setupUI();
    void applyLightMode();
    void applyDarkMode();
};

#endif // ADMINDASHBOARD_H
