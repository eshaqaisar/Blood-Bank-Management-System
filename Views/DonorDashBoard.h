#ifndef DONORDASHBOARD_H
#define DONORDASHBOARD_H

#include <QWidget>//base class for all UI elements in Qt
#include <QLabel>//for displaying text such as donor information and titles
#include <QPushButton>//for the logout button and refresh button
#include <QTableWidget>//for displaying the donor's donation history in a tabular format
#include <QVBoxLayout>//for organizing the layout of the dashboard in a vertical manner
#include <string>//std::string replaces QString for donorUsername member variable

class DonorDashboard : public QWidget {
    Q_OBJECT

public:
    //constructor takes std::string; converts to QString only at UI boundary
    explicit DonorDashboard(const std::string& donorUsername, QWidget* parent = nullptr);
    ~DonorDashboard();

private slots:
    void onLogout();
    void onRefreshClicked(); //refresh slot added

private:
    std::string donorUsername; //std::string replaces QString

    QLabel* lblTitle;
    QLabel* lblName;
    QLabel* lblBloodGroup;
    QLabel* lblEligibility;
    QLabel* lblLastDonation;
    QTableWidget* tblHistory;
    QPushButton* btnLogout;
    QPushButton* btnRefresh; //refresh button added

    void setupUI();
    void loadDonorProfile();
    void applyStyle();
};

#endif // DONORDASHBOARD_H