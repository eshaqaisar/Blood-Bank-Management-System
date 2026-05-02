#ifndef SPLASHPAGE_H
#define SPLASHPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QPaintEvent>

class SplashPage : public QWidget
{
    Q_OBJECT

public:
    explicit SplashPage(QWidget* parent = nullptr);
    ~SplashPage();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onGetStartedClicked();

private:
    QLabel* lblTitle;
    QPushButton* btnGetStarted;

    void setupUI();
    void applyStyle();
};

#endif // SPLASHPAGE_H
