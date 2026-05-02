#ifndef DONORLISTFORM_H
#define DONORLISTFORM_H

#include <QWidget>//base class for all UI elements in Qt
#include <QTableWidget>//for displaying the list of donors in a tabular format
#include <QLineEdit>//for the search bar to filter donors by name
#include <QPushButton>//for action buttons like "Add Donor", "View Profile", and "Delete"
#include <QLabel>//for displaying the count of donors currently shown in the table
#include <QComboBox>//for filtering donors by blood group
#include <QVBoxLayout>//for organizing the layout of the form in a vertical manner
#include <QHBoxLayout>//for organizing the layout of the search bar and filter in a horizontal manner


class DonorListForm : public QWidget {
    Q_OBJECT

public:
    explicit DonorListForm(QWidget* parent = nullptr);
    ~DonorListForm();

private slots:
    void onSearchChanged(const QString& text); //filter table as user types
    void onDeleteClicked();                    //delete selected row
    void onAddDonorClicked();                  //open DonorRegistrationForm
    void onViewProfileClicked();               //show popup with donor details + compatibility
    void onFilterByBloodGroup(const QString& bg); //filter table by blood group

private:
    QTableWidget* tblDonors;
    QLineEdit* txtSearch;
    QComboBox* cmbFilter;      //filter by blood group
    QPushButton* btnDelete;
    QPushButton* btnAdd;
    QPushButton* btnViewProfile;
    QLabel* lblCount;       //shows "Showing X donors"

    void setupUI();
    void loadDonors();           //read donors.txt and fill table
    void applyStyle();
    void filterTable(const QString& text, const QString& bloodGroup);
};

#endif // DONORLISTFORM_H
