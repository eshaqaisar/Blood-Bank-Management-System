#include "SplashPage.h"
#include "Views/LandingPage.h"
#include <QPainter>
#include <QFont>

SplashPage::SplashPage(QWidget* parent) : QWidget(parent)
{
    setupUI();
    applyStyle();
}

SplashPage::~SplashPage() {}

void SplashPage::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap bg(":/splash.jpg");
    painter.drawPixmap(0, 0, width(), height(), bg);
    (void)event;
}

void SplashPage::setupUI()
{
    setWindowTitle("Blood Bank System");
    setMinimumSize(500, 700);

    lblTitle = new QLabel("🩸 Blood Bank System", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");


    btnGetStarted = new QPushButton("Get Started →", this);
    btnGetStarted->setObjectName("btnGetStarted");
    btnGetStarted->setMinimumHeight(50);
    btnGetStarted->setMaximumWidth(200);

    connect(btnGetStarted, &QPushButton::clicked, this, &SplashPage::onGetStartedClicked);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(60, 10, 60, 60);
    layout->addStretch(8);      //  image ka area
    layout->addWidget(lblTitle);
    layout->addSpacing(20);
    layout->addWidget(btnGetStarted, 0, Qt::AlignCenter);
    layout->addStretch(0);

    setLayout(layout);
}

void SplashPage::applyStyle()
{
    setStyleSheet(R"(
        QWidget {
            color: #2c3e50;
            font-family: Arial;
        }
      #lblTitle {
        font-size: 28px;
        font-weight: bold;
        color: #8B0000;
         background: transparent;
       }

        #btnGetStarted {
            background-color: #8B0000;
            color: white;
            border-radius: 25px;
            font-size: 15px;
            font-weight: bold;
            border: none;
            padding: 10px 30px;
        }
        #btnGetStarted:hover { background-color: #c0392b; }
        #btnGetStarted:pressed { background-color: #6B0000; }
    )");
}

void SplashPage::onGetStartedClicked()
{
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}