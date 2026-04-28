#ifndef DONORDASHBOARD_H
#define DONORDASHBOARD_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class DonorDashBoardClass; } // Designer name match
QT_END_NAMESPACE

class DonorDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit DonorDashboard(QWidget* parent = nullptr);
    ~DonorDashboard();

private slots:
    
    void on_pushButton_clicked();

private:
    Ui::DonorDashBoardClass* ui;
};

#endif 