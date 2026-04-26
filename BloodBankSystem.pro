########################################################
# BloodBankSystem.pro  —  Qt Project Configuration File
# -------------------------------------------------------
# This file tells Qt Creator / qmake how to build the app.
# If you use Visual Studio 2022 with Qt extension, this
# file is auto-read when you open it via:
#   Qt VS Tools → Open Qt Project File (.pro)
########################################################

# Qt modules required
QT += core gui widgets

# Use C++17 standard (needed for lambdas, range-for, etc.)
CONFIG += c++17

# Target executable name
TARGET   = BloodBankSystem
TEMPLATE = app

# ---- Source Files (.cpp) ----
SOURCES += \
    main.cpp \
    Models/Person.cpp \
    Models/Donor.cpp \
    Models/Patient.cpp \
    Models/User.cpp \
    Models/BloodBag.cpp \
    Models/BloodRequest.cpp \
    Utilities/FileManager.cpp \
    Utilities/Compatibility.cpp \
    Views/LandingPage.cpp \
    Views/LoginForm.cpp \
    Views/SignupForm.cpp \
    Views/AdminDashboard.cpp \
    Views/DonorDashboard.cpp \
    Views/PatientDashboard.cpp \
    Views/DonorRegistrationForm.cpp \
    Views/DonorListForm.cpp \
    Views/InventoryDashboard.cpp \
    Views/RequestManagementForm.cpp \
    Views/BloodRequestForm.cpp

# ---- Header Files (.h) ----
HEADERS += \
    Models/Person.h \
    Models/Donor.h \
    Models/Patient.h \
    Models/User.h \
    Models/BloodBag.h \
    Models/BloodRequest.h \
    Utilities/FileManager.h \
    Utilities/Compatibility.h \
    Views/LandingPage.h \
    Views/LoginForm.h \
    Views/SignupForm.h \
    Views/AdminDashboard.h \
    Views/DonorDashboard.h \
    Views/PatientDashboard.h \
    Views/DonorRegistrationForm.h \
    Views/DonorListForm.h \
    Views/InventoryDashboard.h \
    Views/RequestManagementForm.h \
    Views/BloodRequestForm.h

# ---- Resources (QSS Style Files) ----
# Files listed here get embedded into the executable
# Access via: ":/Resources/style_light.qss"
RESOURCES += \
    Resources/resources.qrc

# Default deployment rules
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
