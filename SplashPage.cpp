#include "SplashPage.h"//for the class definition
#include "Views/LandingPage.h"//for navigating to the landing page after clicking "Get Started"
#include <QPainter>//for drawing the background image
#include <QFont>//for setting custom fonts

SplashPage::SplashPage(QWidget* parent) : QWidget(parent)
{
    setupUI();
    applyStyle();
}

SplashPage::~SplashPage() {}
//this function is responsible for drawing the background image on the splash page. It uses QPainter to draw a pixmap that fills the entire widget area. The image is loaded from the resource file (":/splash.jpg") and scaled to fit the widget's dimensions. The event parameter is not used in this implementation, but it is required by the paintEvent signature.
void SplashPage::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap bg(":/splash.jpg");
    painter.drawPixmap(0, 0, width(), height(), bg);
    (void)event;
}
//this function sets up the user interface of the splash page. It creates a title label and a "Get Started" button, and arranges them vertically using a QVBoxLayout. The title label is centered and styled with a larger font size and a bold weight. The "Get Started" button is styled with a red background, white text, rounded corners, and changes color on hover and press. The button is connected to the onGetStartedClicked slot, which will handle the navigation to the landing page when clicked.
void SplashPage::setupUI()
{
    setWindowTitle("Blood Bank System");
    setMinimumSize(500, 700);
	//title label with a blood drop emoji and the name of the system
    lblTitle = new QLabel("🩸 Blood Bank System", this);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setObjectName("lblTitle");

	//get started button that will take the user to the landing page when clicked
    btnGetStarted = new QPushButton("Get Started →", this);
    btnGetStarted->setObjectName("btnGetStarted");
    btnGetStarted->setMinimumHeight(50);
    btnGetStarted->setMaximumWidth(200);
	//connect the button's clicked signal to the slot that will handle the navigation
    connect(btnGetStarted, &QPushButton::clicked, this, &SplashPage::onGetStartedClicked);
	//layout to arrange the title and button vertically with some spacing and margins
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
//this function applies the styles to the splash page using a stylesheet. It sets the overall font and color for the widget, and then defines specific styles for the title label and the "Get Started" button. The title label is styled with a larger font size, bold weight, and a dark red color. The button is styled with a red background, white text, rounded corners, and changes color on hover and press to provide visual feedback to the user.
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
//this slot is called when the "Get Started" button is clicked. It creates a new instance of the LandingPage class, shows it, and then closes the current splash page. This allows the user to transition smoothly from the splash screen to the main landing page of the application.
void SplashPage::onGetStartedClicked()
{
    LandingPage* landing = new LandingPage();
    landing->show();
    this->close();
}