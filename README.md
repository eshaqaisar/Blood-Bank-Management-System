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
