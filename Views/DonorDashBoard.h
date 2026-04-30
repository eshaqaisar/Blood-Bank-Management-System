#ifndef DONORDASHBOARD_H
#define DONORDASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>


class DonorDashboard : public QWidget {
    Q_OBJECT

public:
    explicit DonorDashboard(const QString& donorUsername, QWidget* parent = nullptr);
    ~DonorDashboard();

private slots:
    void onLogout();

private:
    QString donorUsername;

    QLabel* lblTitle;
    QLabel* lblName;
    QLabel* lblBloodGroup;
    QLabel* lblEligibility;
    QLabel* lblLastDonation;
    QTableWidget* tblHistory;      // Shows donation history
    QPushButton* btnLogout;

    void setupUI();
    void loadDonorProfile(); // Reads donor data from donors.txt
    void applyStyle();
};

#endif // DONORDASHBOARD_H
