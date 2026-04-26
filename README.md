# 🩸 Blood Bank Management System
### OOP Project — Team 12 | Qt C++ | Visual Studio 2022

---

## 📁 Project Structure

```
BloodBankSystem/
├── main.cpp                        ← App entry point
├── BloodBankSystem.pro             ← Qt project file (open this in Qt/VS)
│
├── Models/                         ← All OOP classes (backend)
│   ├── Person.h / .cpp             ← Abstract base class (Abstraction)
│   ├── Donor.h  / .cpp             ← Inherits Person (Inheritance)
│   ├── Patient.h / .cpp            ← Inherits Person (Inheritance)
│   ├── User.h  / .cpp              ← Login/Auth class
│   ├── BloodBag.h / .cpp           ← BloodBag + BloodInventory (Aggregation)
│   └── BloodRequest.h / .cpp       ← Blood request lifecycle
│
├── Utilities/                      ← Helper/service classes
│   ├── FileManager.h / .cpp        ← ALL file I/O (Abstraction)
│   └── Compatibility.h / .cpp      ← Blood type matching logic
│
├── Views/                          ← All UI screens
│   ├── LandingPage.h / .cpp        ← First screen shown
│   ├── LoginForm.h / .cpp          ← Login + CAPTCHA
│   ├── SignupForm.h / .cpp         ← Register account
│   ├── AdminDashboard.h / .cpp     ← Admin hub (dark mode, CSV export)
│   ├── DonorDashboard.h / .cpp     ← Donor portal
│   ├── PatientDashboard.h / .cpp   ← Patient portal
│   ├── DonorRegistrationForm.h/.cpp← Add new donor
│   ├── DonorListForm.h / .cpp      ← Search/delete donors
│   ├── InventoryDashboard.h / .cpp ← Blood stock + progress bars
│   ├── RequestManagementForm.h/.cpp← Approve/reject requests
│   └── BloodRequestForm.h / .cpp   ← Patient submits request
│
├── Resources/
│   ├── resources.qrc               ← Qt resource file
│   ├── style_light.qss             ← Light theme CSS
│   └── style_dark.qss              ← Dark theme CSS
│
└── Database/                       ← Auto-created .txt data files
    ├── users.txt                   ← Pre-seeded with admin account
    ├── donors.txt
    ├── patients.txt
    ├── inventory.txt
    ├── requests.txt
    └── logs.txt
```

---

## 🔐 Default Login Credentials

| Role  | Username | Password  |
|-------|----------|-----------|
| Admin | `admin`  | `admin123`|

---

## 🚀 How to Open & Run in Visual Studio 2022

### Step 1 — Prerequisites
Make sure you have installed:
- **Qt 6.x** (download from qt.io — select MSVC 2022 64-bit kit)
- **Qt Visual Studio Tools** extension (search in VS → Extensions → Manage Extensions)

### Step 2 — Open the Project
1. Open **Visual Studio 2022**
2. Go to **Extensions → Qt VS Tools → Open Qt Project File (.pro)**
3. Browse to `BloodBankSystem.pro` and open it
4. VS will generate a `.vcxproj` file automatically

### Step 3 — Configure Qt Kit
1. Go to **Extensions → Qt VS Tools → Qt Versions**
2. Click **Add** → point to your Qt install:  
   e.g. `C:\Qt\6.x.x\msvc2022_64\bin\qmake.exe`
3. Make sure the project uses this kit

### Step 4 — Build and Run
1. Press **Ctrl+Shift+B** to build
2. Press **F5** to run
3. The Landing Page will appear first

---

## 🎨 UI Design Guide — Qt Designer in Visual Studio

> **Important:** All screens in this project are built **programmatically** in `.cpp` files.  
> You do NOT need Qt Designer for these files — just build and run.  
> However, if you prefer to design using `.ui` drag-and-drop, follow the guide below.

---

### 📌 How Qt Designer Works with Visual Studio

When you create a `.ui` file:
- Qt Designer generates XML describing the layout
- `uic` (UI Compiler) converts it to a `ui_FileName.h` header automatically at build time
- In your `.h` file you `#include "ui_FileName.h"` and use `ui->setupUi(this)`

---

## 📐 Screen-by-Screen Qt Designer Guide

---

### Screen 1: LandingPage

**What it looks like:**
```
┌─────────────────────────────────────┐
│    🩸 Centralized Blood Bank System  │  ← QLabel (bold, large, red)
│  Connecting Donors with Patients     │  ← QLabel (gray subtitle)
│                                     │
│  ┌─────────────────────────────┐    │
│  │   🔐 Login as Admin         │    │  ← QPushButton (dark navy)
│  └─────────────────────────────┘    │
│  ┌─────────────────────────────┐    │
│  │   👤 Login as Donor/Patient  │    │  ← QPushButton (blood red)
│  └─────────────────────────────┘    │
└─────────────────────────────────────┘
```

**Qt Designer Steps:**
1. Create new `.ui` → Widget
2. Drag **QVBoxLayout** onto the form
3. Drag **QLabel** → set text to app title, objectName = `lblTitle`
4. Drag **QLabel** → subtitle, objectName = `lblSubtitle`
5. Drag **QPushButton** → text = "Login as Admin", objectName = `btnAdminLogin`
6. Drag **QPushButton** → text = "Login as User", objectName = `btnUserLogin`
7. Set margins: 60px on all sides
8. Set spacing: 20px

**In .cpp, connect signals:**
```cpp
connect(ui->btnAdminLogin, &QPushButton::clicked, this, &LandingPage::onAdminLoginClicked);
connect(ui->btnUserLogin,  &QPushButton::clicked, this, &LandingPage::onUserLoginClicked);
```

---

### Screen 2: LoginForm

**What it looks like:**
```
┌───────────────────────────────────┐
│        🔐 Admin Login              │  ← QLabel title
│                                   │
│  Username: [________________]     │  ← QLabel + QLineEdit
│  Password: [●●●●●●●●●●●●●●●]     │  ← QLineEdit (echo=Password)
│                                   │
│  Password Strength:               │
│  [████████░░░░] Good              │  ← QProgressBar
│                                   │
│  Enter CAPTCHA:                   │
│  [ X 4 P Q 2 ]  [🔄 New]         │  ← QLabel + QPushButton
│  [________________]               │  ← QLineEdit
│                                   │
│  [        Login        ]          │  ← QPushButton
│  [    Create Account   ]          │  ← QPushButton
│  [← Back]                        │  ← QPushButton
└───────────────────────────────────┘
```

**Qt Designer Steps:**
1. Widget form, set minimum width = 420
2. **QVBoxLayout** as main layout
3. Add **QLabel** → title, objectName = `lblTitle`
4. Add **QLabel** "Username:" + **QLineEdit**, objectName = `txtUsername`
5. Add **QLabel** "Password:" + **QLineEdit**, objectName = `txtPassword`
   - In Properties panel → `echoMode` → set to `Password`
6. Add **QLabel** "Password Strength:" + **QProgressBar**, objectName = `barPasswordStrength`
   - Set range: minimum=0, maximum=4
   - textVisible = false
7. **CAPTCHA row**: drag **QHBoxLayout**, inside put:
   - **QLabel** objectName = `lblCaptchaDisplay`
   - **QPushButton** text="🔄 New", objectName = `btnRefreshCaptcha`
8. Add **QLineEdit** for CAPTCHA input, objectName = `txtCaptchaInput`, maxLength=5
9. Add **QLabel** for status messages, objectName = `lblStatus`
10. Three **QPushButton** widgets for Login / Create Account / Back

---

### Screen 3: SignupForm

**What it looks like:**
```
┌───────────────────────────────────┐
│      📝 Create New Account         │
│                                   │
│  Full Name:   [____________]      │
│  Age:         [____________]      │
│  Contact:     [____________]      │
│  City:        [____________]      │
│  Blood Group: [A+ ▾]             │  ← QComboBox
│  Role:        [Donor ▾]          │  ← QComboBox
│  Username:    [____________]      │
│  Password:    [●●●●●●●●●●●●]    │
│  Confirm PW:  [●●●●●●●●●●●●]    │
│                                   │
│  Password Strength:               │
│  [████░░░░░░░░] Fair             │  ← QProgressBar
│  Add a number to strengthen       │  ← QLabel hint
│                                   │
│  [    ✅ Register    ]            │
│  [  ← Back to Login ]            │
└───────────────────────────────────┘
```

**Qt Designer Steps:**
1. Use **QFormLayout** — best for label+field pairs
2. Add rows: label text on left, widget on right
3. For Blood Group & Role: drag **QComboBox** 
   - Right-click QComboBox → Edit Items → add blood groups / roles
4. Password fields: set echoMode = Password
5. QProgressBar: range 0–4, textVisible = false

---

### Screen 4: AdminDashboard

**What it looks like:**
```
┌────────────────────────────────────────────────┐
│ 👋 Welcome, Admin: admin    ⚠️ LOW STOCK: O-   │  ← QHBoxLayout top bar
├───────────┬────────────────────────────────────┤
│ 📌 Nav    │                                    │
│           │   [Content Area — QStackedWidget]  │
│ [Donors]  │                                    │
│ [Inventory│   Switches between:                │
│ [Requests]│     Page 0: DonorListForm          │
│           │     Page 1: InventoryDashboard     │
│ [🌙 Dark] │     Page 2: RequestManagementForm  │
│ [📊 CSV ] │                                    │
│           │                                    │
│ [🚪Logout]│                                    │
└───────────┴────────────────────────────────────┘
```

**Qt Designer Steps:**
1. Main layout: **QHBoxLayout**
2. Left side: **QVBoxLayout** inside a **QWidget**, width = 180, objectName = `sidebar`
   - Add **QLabel** "📌 Navigation"
   - Add 3x **QPushButton** for Donors, Inventory, Requests
   - Add **QPushButton** for Dark Mode toggle
   - Add **QPushButton** for Export CSV
   - Add **vertical spacer** (drag from Widget Box)
   - Add **QPushButton** Logout at bottom
3. Right side: **QStackedWidget**, objectName = `stackedWidget`
   - Right-click → "Insert Page" three times
   - Each page will hold a child form widget

**Connecting page switch:**
```cpp
connect(btnDonors, &QPushButton::clicked, this, [this](){
    stackedWidget->setCurrentIndex(0);
});
```

---

### Screen 5: DonorListForm (inside AdminDashboard Page 0)

**What it looks like:**
```
┌─────────────────────────────────────────────────┐
│ [🔍 Search by name...] Filter: [All Groups ▾]  │
│                                     Donors: 12  │
├─────────────────────────────────────────────────┤
│ Name      │ Age │ City   │ Blood │ Elig │ Last  │
├───────────┼─────┼────────┼───────┼──────┼───────┤
│ Ali Khan  │ 25  │ Lahore │  O+   │ ✅  │ 2024  │
│ Sara Bano │ 30  │ Karachi│  A-   │ ❌  │ 2025  │
├─────────────────────────────────────────────────┤
│ [➕ Add Donor]  [👁 View Profile]  [🗑 Delete]  │
└─────────────────────────────────────────────────┘
```

**Qt Designer Steps:**
1. **QVBoxLayout** as root
2. Top row: **QHBoxLayout** containing:
   - **QLineEdit** (search), objectName = `txtSearch`
   - **QLabel** "Filter:"
   - **QComboBox** objectName = `cmbFilter`
   - **QLabel** objectName = `lblCount`
3. **QTableWidget**, objectName = `tblDonors`
   - Columns: 6
   - Set column headers in Properties
   - selectionBehavior = SelectRows
   - editTriggers = NoEditTriggers
   - sortingEnabled = true
4. Bottom row: **QHBoxLayout** with 3 buttons

---

### Screen 6: InventoryDashboard (Admin Page 1)

**What it looks like:**
```
┌──────────────────────────────────────────────────┐
│  🩸 Blood Inventory Dashboard                    │
│  ⚠️ 2 blood groups are LOW STOCK                │
├──────────────────────────────────────────────────┤
│ Blood Group │ Units │ Stock Level      │ Expired │
├─────────────┼───────┼──────────────────┼─────────┤
│    O-       │  3    │ [███░░░░░░] 3   │  None   │
│    A+       │  12   │ [████████░░] 12 │  None   │
│    B-       │  1    │ [█░░░░░░░░░]  1 │  1 ⚠️   │
├──────────────────────────────────────────────────┤
│ [🔄 Refresh]  [🗑 Remove Expired]  [📊 CSV]     │
└──────────────────────────────────────────────────┘
```

**Qt Designer Steps:**
1. **QVBoxLayout**
2. **QLabel** title + **QLabel** objectName = `lblSummary`
3. **QTableWidget** 4 columns, objectName = `tblInventory`
   - Column 2 (Stock Level) will have QProgressBar inserted via `setCellWidget()`
   - This is done in code, not Designer
4. Bottom button row in **QHBoxLayout**

---

### Screen 7: RequestManagementForm (Admin Page 2)

**What it looks like:**
```
┌──────────────────────────────────────────────────────────┐
│  📋 Blood Request Management         ⚠️ 3 pending        │
│  Filter: [All ▾]                                         │
├──────────────────────────────────────────────────────────┤
│ ID      │ Patient │ Blood │ Units │ Hospital │ Date │ Status│
├─────────┼─────────┼───────┼───────┼──────────┼──────┼──────┤
│REQ-001  │ Ahmed   │  A+   │  2    │ Mayo Hosp│today │🟠Pend│
│REQ-002  │ Fatima  │  O-   │  1    │ Services │today │✅App │
├──────────────────────────────────────────────────────────┤
│ [✅ Approve]  [❌ Reject]  [📄 Certificate]  [🔄 Refresh]│
└──────────────────────────────────────────────────────────┘
```

**Qt Designer Steps:**
1. **QVBoxLayout**
2. Top bar: **QHBoxLayout** → QLabel "Filter:" + QComboBox + spacer + QLabel summary
3. **QTableWidget** 7 columns, objectName = `tblRequests`
   - editTriggers = NoEditTriggers
   - selectionBehavior = SelectRows
4. Bottom: **QHBoxLayout** with 4 buttons

---

### Screen 8: BloodRequestForm (Patient submits request)

**What it looks like:**
```
┌───────────────────────────────────────┐
│        🩸 Submit Blood Request         │
│                                       │
│  Patient Name: [Ahmed Khan_______]    │
│  Hospital:     [Mayo Hospital____]    │
│  Contact:      [0300-1234567_____]   │
│  Blood Group:  [O- ▾]               │
│  Units Needed: [1 unit(s) ▲▼]       │  ← QSpinBox
│                                       │
│  ✅ Can receive from: O-, O+          │  ← Auto-updates on change
│                                       │
│  [📨 Submit Request] [🗑 Clear] [← Back]│
└───────────────────────────────────────┘
```

**Qt Designer Steps:**
1. **QVBoxLayout** outer
2. **QFormLayout** for the input fields:
   - Patient Name → QLineEdit
   - Hospital → QLineEdit
   - Contact → QLineEdit
   - Blood Group → QComboBox (add all 8 groups)
   - Units Needed → **QSpinBox**, range 1–10, suffix = " unit(s)"
   - Compatible donors → QLabel (read-only, updates via code)
3. Button row: QHBoxLayout with 3 buttons

**Blood group change signal:**
```cpp
connect(ui->cmbBloodGroup, &QComboBox::currentTextChanged,
        this, &BloodRequestForm::updateCompatibilityInfo);
```

---

### Screen 9: DonorDashboard (Donor's personal portal)

**What it looks like:**
```
┌──────────────────────────────────┐
│     🩸 Donor Dashboard           │
│                                  │
│  Name:               Ali Khan    │
│  Blood Group:        O+          │
│  Eligible to Donate: ✅ Yes      │
│  Last Donation:      15-03-2024  │
│                                  │
│  📋 Donation History:            │
│  ┌────────────────────────────┐  │
│  │ Date         │ Units       │  │
│  ├──────────────┼─────────────┤  │
│  │ 15-03-2024   │ 1 unit      │  │
│  └────────────────────────────┘  │
│                                  │
│  [🚪 Logout]                    │
└──────────────────────────────────┘
```

**Qt Designer Steps:**
1. QVBoxLayout
2. QLabel title (objectName = lblTitle)
3. Four QLabel widgets for profile info
4. QLabel "Donation History"
5. QTableWidget 2 columns (Date, Units), objectName = tblHistory
6. QPushButton Logout

---

### Screen 10: PatientDashboard

**What it looks like:**
```
┌────────────────────────────────────────────────┐
│           🏥 Patient Dashboard                 │
│  Patient: fatima_patient                       │
│                                                │
│  📋 My Blood Requests:                         │
│  ┌──────────┬───────┬───────┬──────────┬──────┐│
│  │ ID       │ Blood │ Units │ Hospital │Status││
│  ├──────────┼───────┼───────┼──────────┼──────┤│
│  │ REQ-001  │  A+   │  2    │ Mayo     │🟠Pend││
│  └──────────┴───────┴───────┴──────────┴──────┘│
│                                                │
│  [➕ Submit New Request]  [🔄 Refresh Status]  │
│  [🚪 Logout]                                   │
└────────────────────────────────────────────────┘
```

**Qt Designer Steps:**
1. QVBoxLayout
2. QLabel title, QLabel for patient name
3. QTableWidget 5 columns, objectName = tblRequests
4. QHBoxLayout with Submit + Refresh buttons
5. QPushButton Logout

---

## 🗂️ OOP Concepts Map

| Concept | Where It's Used |
|---------|----------------|
| **Abstraction** | `Person` (pure virtual `display()`), `FileManager` (hides file I/O), `CompatibilityChecker` |
| **Encapsulation** | All private fields in every class, accessed only via getters/setters |
| **Inheritance** | `Donor : Person`, `Patient : Person` |
| **Polymorphism** | `Person* p = new Donor()` → `p->display()` calls `Donor::display()` |
| **Aggregation** | `BloodInventory` holds `QList<BloodBag>`. BloodBag can exist independently |
| **File Handling** | `FileManager` reads/writes all `.txt` files. Every model has `toFileString()` / `fromFileString()` |

---

## 👥 Team Domain Assignments

| Domain | Member | Screens |
|--------|--------|---------|
| 1 | Muhammad Ali | Person, Donor, Patient, DonorListForm, DonorRegistrationForm, DonorDashboard |
| 2 | Zara Shah | BloodBag, BloodInventory, InventoryDashboard |
| 3 | Noor Fatima | BloodRequest, BloodRequestForm, RequestManagementForm, PatientDashboard, CompatibilityChecker |
| 4 | Esha Qaisar | User, FileManager, LandingPage, LoginForm, SignupForm, AdminDashboard |

---

## 🔧 File Flow (How Data Moves)

```
User clicks "Submit Request"
        │
        ▼
BloodRequestForm::onSubmitClicked()
        │  Creates BloodRequest object
        ▼
FileManager::saveRequest(request)
        │  Opens requests.txt in Append mode
        │  Writes: "REQ-001,Ahmed,A+,2,Mayo,2025-01-15,Pending\n"
        ▼
Admin opens RequestManagementForm
        │  Calls FileManager::loadRequests()
        │  Reads all lines from requests.txt
        │  Creates BloodRequest objects via fromFileString()
        ▼
Admin clicks "Approve"
        │  FileManager::updateRequestStatus("REQ-001", "Approved")
        │  Rewrites requests.txt with updated status
        │  BloodInventory::useBag("A+", 2) deducts from inventory.txt
        │  FileManager::logActivity(...)  → logs.txt
        ▼
Patient refreshes dashboard → sees "Approved" ✅
```

---

## ⚠️ Common Build Errors & Fixes

| Error | Fix |
|-------|-----|
| `Q_OBJECT macro not found` | Make sure `.h` files are in HEADERS in `.pro` |
| `undefined reference to vtable` | Add `Q_OBJECT` to class and clean/rebuild |
| `cannot open file 'ui_xxx.h'` | Build once first — `uic` generates it |
| QSS not loading | Check `Resources/resources.qrc` lists the file correctly |
| `Database/users.txt not found` | Make sure `Database/` folder exists next to `.exe` |
| Dark mode not applying | Make sure `qApp->setStyleSheet()` is called, not `this->setStyleSheet()` |
