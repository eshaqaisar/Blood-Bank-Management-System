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
#include <string>//std::string replaces QString for filter parameters

class DonorListForm : public QWidget {
    Q_OBJECT

public:
    explicit DonorListForm(QWidget* parent = nullptr);
    ~DonorListForm();

private slots:
    void onSearchChanged(const QString& text);    //filter table as user types
    void onDeleteClicked();                        //delete selected row
    void onAddDonorClicked();                      //open DonorRegistrationForm
    void onViewProfileClicked();                   //show popup with donor details + compatibility
    void onFilterByBloodGroup(const QString& bg);  //filter table by blood group
    void onRefreshClicked();                       //refresh donor list from disk

private:
    QTableWidget* tblDonors;
    QLineEdit* txtSearch;
    QComboBox* cmbFilter;
    QPushButton* btnDelete;
    QPushButton* btnAdd;
    QPushButton* btnViewProfile;
    QPushButton* btnRefresh;   //refresh button added
    QLabel* lblCount;

    void setupUI();
    void loadDonors();
    void applyStyle();
    //filterTable uses std::string params to avoid QString comparison issues
    void filterTable(const std::string& text, const std::string& bloodGroup);
};

#endif // DONORLISTFORM_H