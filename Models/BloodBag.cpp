#include "BloodBag.h"//header file for the BloodBag class, which defines the data structure and operations for individual blood bags in the inventory
#include <QFile>//for file handling when saving and loading inventory data
#include <QTextStream>//for reading and writing inventory data to the file in a structured format
#include <QDebug>//for logging debug information, such as the number of expired bags removed from the inventory
#include <string>//for std::string, std::to_string, std::stoi used throughout
#include <QString>//used only at the Qt file I/O boundary via QString::fromStdString / .toStdString()

//default constructor: initializes a BloodBag with safe default values.
//Required so BloodBag objects can live inside plain arrays (BloodBag bags[MAX_BLOOD_BAGS]).
BloodBag::BloodBag()
    : bloodGroup(""), units(0), collectionDate(QDate()), donorName("")
{
}

//main constructor
BloodBag::BloodBag(const std::string& bloodGroup, int units,
    const QDate& collectionDate, const std::string& donorName)
    : bloodGroup(bloodGroup), units(units),
    collectionDate(collectionDate), donorName(donorName)
{
}

//getters
std::string BloodBag::getBloodGroup()     const { return bloodGroup; }
int         BloodBag::getUnits()          const { return units; }
QDate       BloodBag::getCollectionDate() const { return collectionDate; }
std::string BloodBag::getDonorName()      const { return donorName; }

//how many days until this bag expires? (negative = already expired)
int BloodBag::getDaysUntilExpiry() const {
    QDate expiryDate = collectionDate.addDays(EXPIRY_DAYS);
    return QDate::currentDate().daysTo(expiryDate);
}

void BloodBag::setUnits(int u) { units = u; }

//expiry check: calculate the number of days since collection and compare to the expiry threshold.
//bag is expired if more than 42 days have passed since collection.
bool BloodBag::isExpired() const {
    return collectionDate.daysTo(QDate::currentDate()) > EXPIRY_DAYS;
}

//serialize to a string format suitable for saving to inventory.txt.
//format: "bloodGroup,units,collectionDate,donorName"
//std::to_string replaces QString::number; QDate::toString converted to std::string at boundary
std::string BloodBag::toFileString() const {
    return bloodGroup + "," +
        std::to_string(units) + "," +
        collectionDate.toString("yyyy-MM-dd").toStdString() + "," +
        donorName;
}

//deserialize from inventory.txt.
//manual comma split replaces QStringList / line.split(',')
BloodBag BloodBag::fromFileString(const std::string& line) {
    std::string parts[6];
    int count = 0;
    std::string token;
    for (char ch : line) {
        if (ch == ',' && count < 5) {
            parts[count++] = token;
            token.clear();
        }
        else {
            token += ch;
        }
    }
    parts[count++] = token; //last field after final comma

    if (count < 4)
        return BloodBag("A+", 0, QDate::currentDate(), "Unknown");

    return BloodBag(
        parts[0],                                                           //bloodGroup
        std::stoi(parts[1]),                                                //units
        QDate::fromString(QString::fromStdString(parts[2]), "yyyy-MM-dd"),  //collectionDate
        parts[3]                                                            //donorName
    );
}

// -----------------------------------------------------------------------
// BloodInventory methods
// -----------------------------------------------------------------------

//add a new blood bag to the inventory (called after successful donation).
//silently ignores the add if the array is already full.
void BloodInventory::addBag(const BloodBag& bag) {
    if (bagCount < MAX_BLOOD_BAGS)
        bags[bagCount++] = bag;
}

//use blood from inventory when a request is approved.
//finds bags of matching blood group and deducts units one by one.
//returns false if not enough stock is available -- caller must NOT approve if false is returned.
bool BloodInventory::useBag(const std::string& bloodGroup, int unitsNeeded) {
    int available = getAvailableUnits(bloodGroup);
    if (available < unitsNeeded) return false; //not enough stock -- request must be rejected

    //deduct from bags one by one until we have fulfilled the request
    int remaining = unitsNeeded;
    for (int i = 0; i < bagCount && remaining > 0; i++) {
        if (bags[i].getBloodGroup() == bloodGroup && !bags[i].isExpired()) {
            int take = bags[i].getUnits();
            if (take > remaining) take = remaining;
            bags[i].setUnits(bags[i].getUnits() - take);
            remaining -= take;
        }
    }

    //compact the array: remove bags whose units have dropped to 0.
    //plain shift-left loop replaces QList filter pattern.
    int newCount = 0;
    for (int i = 0; i < bagCount; i++) {
        if (bags[i].getUnits() > 0) {
            bags[newCount++] = bags[i];
        }
    }
    bagCount = newCount;
    return true;
}

//scan all bags and remove those past their 42-day limit.
//uses the same compact/shift-left pattern to avoid any list type.
void BloodInventory::removeExpiredBags() {
    int before = bagCount;
    int newCount = 0;
    for (int i = 0; i < bagCount; i++) {
        if (!bags[i].isExpired()) {
            bags[newCount++] = bags[i];
        }
    }
    bagCount = newCount;
    int removed = before - bagCount;
    qDebug() << "Removed" << removed << "expired blood bags.";
}

//count total available (non-expired) units of a given blood group
int BloodInventory::getAvailableUnits(const std::string& bg) const {
    int total = 0;
    for (int i = 0; i < bagCount; i++) {
        if (bags[i].getBloodGroup() == bg && !bags[i].isExpired()) {
            total += bags[i].getUnits();
        }
    }
    return total;
}

//check if stock for a blood group is critically low
bool BloodInventory::isLowStock(const std::string& bloodGroup) const {
    return getAvailableUnits(bloodGroup) < LOW_STOCK_THRESHOLD;
}

//returns all bags (including expired) as a BagArray.
//BagArray replaces QList<BloodBag> as the return type.
BagArray BloodInventory::getAllBags() const {
    BagArray result;
    for (int i = 0; i < bagCount; i++)
        result.append(bags[i]);
    return result;
}

//returns only expired bags as a BagArray (for admin report and donor history).
//BagArray replaces QList<BloodBag> as the return type.
BagArray BloodInventory::getExpiredBags() const {
    BagArray result;
    for (int i = 0; i < bagCount; i++) {
        if (bags[i].isExpired()) result.append(bags[i]);
    }
    return result;
}

//returns a QMap of blood group -> total available units.
//QMap key is kept as QString because the Qt UI (InventoryDashboard) needs it.
QMap<QString, int> BloodInventory::getInventoryMap() const {
    QMap<QString, int> map;
    const char* groups[] = { "A+","A-","B+","B-","AB+","AB-","O+","O-" };
    for (int i = 0; i < 8; i++) {
        //convert std::string blood group to QString only at this Qt boundary
        map[QString::fromStdString(groups[i])] = getAvailableUnits(groups[i]);
    }
    return map;
}

//save inventory to inventory.txt.
//filePath is std::string; converted to QString only at the QFile API boundary.
void BloodInventory::save(const std::string& filePath) const {
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "ERROR: Cannot open inventory file for writing:"
            << QString::fromStdString(filePath);
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < bagCount; i++) {
        out << QString::fromStdString(bags[i].toFileString()) << "\n";
    }
    file.close();
}

//load inventory from inventory.txt.
//filePath is std::string; converted to QString only at the QFile API boundary.
void BloodInventory::load(const std::string& filePath) {
    bagCount = 0; //reset array counter (replaces bags.clear())
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "INFO: Inventory file not found. Starting empty:"
            << QString::fromStdString(filePath);
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString qline = in.readLine().trimmed();
        if (!qline.isEmpty() && bagCount < MAX_BLOOD_BAGS) {
            //convert to std::string at the boundary before passing to fromFileString
            bags[bagCount++] = BloodBag::fromFileString(qline.toStdString());
        }
    }
    file.close();
    qDebug() << "Loaded" << bagCount << "blood bags from inventory.";
}