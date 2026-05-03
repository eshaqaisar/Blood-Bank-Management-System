#ifndef SPLASHPAGE_H
#define SPLASHPAGE_H

#include <QWidget>//for basic widget functionality
#include <QPushButton>//for the "Get Started" button
#include <QLabel>//for the title label
#include <QVBoxLayout>//for arranging the label and button vertically
#include <QPainter>//for custom painting the background image
#include <QPaintEvent>//for handling paint events

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
