# 🩸 Centralized Blood Bank & Donor Management System (Qt C++)

**Team 12 | BSCY-2A | Semester 2 | OOP Project**

| Role         | Name             | Roll #   | Domain                                    |
|--------------|------------------|----------|-------------------------------------------|
| Team Lead    | Esha Qaisar      | 25L-2009 | Security, Auth, File I/O, Routing & QSS Styling |
| Member 2     | Noor Fatima      | 25L-2005 | Blood Requests & Compatibility            |
| Member 3     | Zara Shah        | 25L-2006 | Blood Inventory System                    |
| Member 4     | Muhammad Ali     | 25L-2063 | OOP Architecture & Donor Management       |

---

## 📁 Project Structure (Qt Architecture)
BloodBankSystem/
│
├── BloodBankSystem.sln / .pro # Project configuration
├── main.cpp # Esha (Application entry point)
│
├── Models/ # Pure C++ OOP Classes (backend)
│ ├── Person.h / .cpp # Ali
│ ├── Donor.h / .cpp # Ali
│ ├── Patient.h / .cpp # Ali
│ ├── User.h / .cpp # Esha
│ ├── BloodBag.h / .cpp # Zara
│ └── BloodRequest.h / .cpp # Noor
│
├── Views/ # Qt UI Bundles (.h, .cpp, .ui)
│ ├── LandingPage.* # Esha
│ ├── LoginForm.* # Esha
│ ├── SignupForm.* # Esha
│ ├── AdminDashboard.* # Esha
│ ├── DonorDashboard.* # Ali
│ ├── PatientDashboard.* # Noor
│ ├── DonorRegForm.* # Ali
│ ├── DonorListForm.* # Ali
│ ├── InventoryDashboard.* # Zara
│ ├── BloodRequestForm.* # Noor
│ └── RequestMgmtForm.* # Noor
│
├── Utilities/ # Shared helpers
│ ├── FileManager.h / .cpp # Esha
│ └── Compatibility.h / .cpp # Noor
│
├── Resources/ # Qt Assets & Styling (Esha) ⭐ READ THIS
│ ├── assets.qrc # Resource manifest for images/icons
│ ├── style_light.qss # Global Qt Style Sheet (Light)
│ └── style_dark.qss # Global Qt Style Sheet (Dark)
│
└── Database/ # Auto-tracked via .gitkeep
├── users.txt
├── donors.txt
├── patients.txt
├── inventory.txt
├── requests.txt
└── logs.txt

text

---

## 🎨 Using the Qt Style System (QSS)

> ⚠️ **EVERY TEAM MEMBER MUST READ THIS BEFORE USING QT DESIGNER**  
> We are no longer hard-coding colors in C++! All styling is handled via Qt Style Sheets (.qss) managed by **Esha**.  
> This works exactly like CSS in web development.  
> **Do NOT change colors or fonts manually inside Qt Designer.**

### 1 — How to apply styles to your buttons/widgets

To make your UI look right, you only need to give your widgets the correct **objectName** or Qt Property in Qt Designer. The global `style_light.qss` will automatically style them when the app runs.

In Qt Designer, select your button, go to the Property Editor, and set the `objectName`:

| objectName      | Look / Color        | Use for                                   |
|-----------------|---------------------|-------------------------------------------|
| `btnPrimary`    | 🔴 Red               | Submit, Login, Register, Add              |
| `btnSecondary`  | ⬜ Grey              | Cancel, Back, Close                       |
| `btnDanger`     | 🟥 Dark Red          | Delete, Reject                            |
| `btnSuccess`    | 🟢 Green             | Approve, Mark Eligible                    |
| `btnInfo`       | 🔵 Blue              | View Details, Search                      |
| `btnNav`        | 🔴 Transparent/Red   | Left sidebar navigation panel             |

### 2 — Global Widget Styling

Standard widgets are styled globally. Just drag and drop them in Qt Designer, and they will look perfect automatically:

- `QLineEdit` (Text boxes)
- `QComboBox` (Drop-downs like Blood Group)
- `QTableWidget` (Replaces DataGridView for Donor Lists)

### 3 — Dark Mode Toggle (Muhammad Ali)

To switch the entire application between Light and Dark mode, use this C++ code in your toggle button's slot:

```cpp
#include <QFile>
#include <QApplication>

void AdminDashboard::on_btnDarkToggle_clicked() {
    QFile styleFile(":/styles/style_dark.qss"); // Loads from Resources
    if(styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        qApp->setStyleSheet(styleSheet); // Applies to the WHOLE app
    }
}
4 — Inventory Progress Bars (Zara Shah)
Use Qt QProgressBar. To update them via C++ logic:

cpp
// Update UI based on backend inventory count
ui->pbAPlus->setValue(75); // Updates UI to 75%
⚠️ Git Rules (CRITICAL — read before your first push)
Never push to main directly. Create your own branch:
git checkout -b feature/your-name

STAY IN YOUR VIEWS: Never edit a .ui file assigned to someone else. If two people edit the same .ui file, GitHub will create a merge conflict that breaks the XML, destroying the UI screen entirely.

Before pushing, always pull first:
git pull origin main

Ignore the build folders: Ensure you are not pushing the x64, Debug, or Release folders. Our .gitignore handles this, do not bypass it.

🚀 Getting Started (Clone & Run)
bash
git clone https://github.com/YOUR_USERNAME/BloodBankSystem.git
cd BloodBankSystem
Open BloodBankSystem.sln in Visual Studio 2022 (ensure you have the Qt VS Tools Extension installed).

Ensure your platform is set to x64.

Press F5 to build and run.

Note: The Database/ folder is tracked via a .gitkeep file. Empty .txt files will be safely generated on first run by FileManager::load() if they do not exist.

🖥️ Screens & Ownership Bundle
In Qt, every screen requires 3 files. When you own a screen, you own its .h, .cpp, and .ui files.

Screen	Bundle Name	Owner
Landing Page	LandingPage.*	Esha
Login	LoginForm.*	Esha
Signup	SignupForm.*	Esha
Admin Dashboard	AdminDashboard.*	Esha
Donor Dashboard	DonorDashboard.*	Ali
Donor Registration	DonorRegForm.*	Ali
Donor List / Search	DonorListForm.*	Ali
Inventory Dashboard	InventoryDashboard.*	Zara
Patient Dashboard	PatientDashboard.*	Noor
Blood Request Form	BloodRequestForm.*	Noor
Request Management	RequestMgmtForm.*	Noor
Happy coding! 🚑💉