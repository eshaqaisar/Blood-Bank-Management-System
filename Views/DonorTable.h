#ifndef DONORLISTFORM_H
#define DONORLISTFORM_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>




class DonorListForm : public QWidget {
    Q_OBJECT

public:
    explicit DonorListForm(QWidget* parent = nullptr);
    ~DonorListForm();

private slots:
    void onSearchChanged(const QString& text); // Filter table as user types
    void onDeleteClicked();                    // Delete selected row
    void onAddDonorClicked();                  // Open DonorRegistrationForm
    void onViewProfileClicked();               // Show popup with donor details + compatibility
    void onFilterByBloodGroup(const QString& bg); // Filter table by blood group

private:
    QTableWidget* tblDonors;
    QLineEdit* txtSearch;
    QComboBox* cmbFilter;      // Filter by blood group
    QPushButton* btnDelete;
    QPushButton* btnAdd;
    QPushButton* btnViewProfile;
    QLabel* lblCount;       // Shows "Showing X donors"

    void setupUI();
    void loadDonors();           // Read donors.txt and fill table
    void applyStyle();
    void filterTable(const QString& text, const QString& bloodGroup);
};

#endif // DONORLISTFORM_H
