#include "BloodBag.h"//header file for the BloodBag class, which defines the data structure and operations for individual blood bags in the inventory
#include <QFile>//for file handling when saving and loading inventory data
#include <QTextStream>//for reading and writing inventory data to the file in a structured format
#include <QDebug>//for logging debug information, such as the number of expired bags removed from the inventory
#include <string>//for std::string, std::to_string, std::stoi used throughout
#include <QString>//used only at the Qt file I/O boundary via QString::fromStdString / .toStdString()

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

//expiry check: calculate the number of days since collection and compare to the expiry threshold
//bag is expired if more than 42 days have passed since collection.
bool BloodBag::isExpired() const {
    return collectionDate.daysTo(QDate::currentDate()) > EXPIRY_DAYS;
}

//serialize to a string format suitable for saving to inventory.txt, using a simple comma-separated format: "bloodGroup,units,collectionDate,donorName".
// std::to_string replaces QString::number; QDate::toString converted to std::string at boundary
std::string BloodBag::toFileString() const {
    return bloodGroup + "," +
        std::to_string(units) + "," +
        collectionDate.toString("yyyy-MM-dd").toStdString() + "," +
        donorName;
}

//deserialize from inventory.txt
// manual comma split replaces QStringList / line.split(',')
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
    parts[count++] = token; //last field

    if (count < 4)
        return BloodBag("A+", 0, QDate::currentDate(), "Unknown");

    return BloodBag(
        parts[0],                                                          //bloodGroup
        std::stoi(parts[1]),                                               //units
        QDate::fromString(QString::fromStdString(parts[2]), "yyyy-MM-dd"), //collectionDate
        parts[3]                                                           //donorName
    );
}

//add a new blood bag to the inventory (called after successful donation)
void BloodInventory::addBag(const BloodBag& bag) {
    bags.append(bag);
}

//use blood from inventory when a request is approved.
//finds bags of matching blood group and deducts units.
//returns false if not enough stock is available — caller must NOT approve if false is returned.
bool BloodInventory::useBag(const std::string& bloodGroup, int unitsNeeded) {
    int available = getAvailableUnits(bloodGroup);
    if (available < unitsNeeded) return false; //not enough stock — request must be rejected

    //deduct from bags one by one until we have enough
    int remaining = unitsNeeded;
    for (BloodBag& bag : bags) {
        if (bag.getBloodGroup() == bloodGroup && !bag.isExpired() && remaining > 0) {
            int take = qMin(bag.getUnits(), remaining);
            bag.setUnits(bag.getUnits() - take);
            remaining -= take;
        }
    }

    //remove bags that are now empty (units == 0)
    QList<BloodBag> nonEmpty;
    for (const BloodBag& b : bags) {
        if (b.getUnits() > 0) nonEmpty.append(b);
    }
    bags = nonEmpty;
    return true;
}

//scan all bags and remove those past their 42-day limit
void BloodInventory::removeExpiredBags() {
    int before = bags.size();
    QList<BloodBag> fresh;
    for (const BloodBag& b : bags) {
        if (!b.isExpired())
            fresh.append(b);
    }
    bags = fresh;
    int removed = before - bags.size();
    qDebug() << "Removed" << removed << "expired blood bags.";
}

//count total available (non-expired) units of a given blood group
int BloodInventory::getAvailableUnits(const std::string& bg) const {
    int total = 0;
    for (const BloodBag& bag : bags) {
        if (bag.getBloodGroup() == bg && !bag.isExpired()) {
            total += bag.getUnits();
        }
    }
    return total;
}

//check if stock for a blood group is critically low
bool BloodInventory::isLowStock(const std::string& bloodGroup) const {
    return getAvailableUnits(bloodGroup) < LOW_STOCK_THRESHOLD;
}

QList<BloodBag> BloodInventory::getAllBags() const { return bags; }

//returns a QMap of blood group → total available units
//QMap key is kept as QString because the UI/Qt code typically uses it that way
QMap<QString, int> BloodInventory::getInventoryMap() const {
    QMap<QString, int> map;
    //iterate through all standard blood groups and populate the map
    const char* groups[] = { "A+","A-","B+","B-","AB+","AB-","O+","O-" };
    for (int i = 0; i < 8; i++) {
        //convert std::string blood group to QString only at this Qt boundary
        map[QString::fromStdString(groups[i])] = getAvailableUnits(groups[i]);
    }
    return map;
}

//returns only expired bags (for admin report)
QList<BloodBag> BloodInventory::getExpiredBags() const {
    QList<BloodBag> expired;
    for (const BloodBag& bag : bags) {
        if (bag.isExpired()) expired.append(bag);
    }
    return expired;
}

//save inventory to inventory.txt; filePath as std::string, converted to QString at QFile boundary
void BloodInventory::save(const std::string& filePath) const {
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "ERROR: Cannot open inventory file for writing:" << QString::fromStdString(filePath);
        return;
    }
    QTextStream out(&file);
    for (const BloodBag& bag : bags) {
        out << QString::fromStdString(bag.toFileString()) << "\n";
    }
    file.close();
}

//load inventory from inventory.txt; filePath as std::string, converted to QString at QFile boundary
void BloodInventory::load(const std::string& filePath) {
    bags.clear(); //clear existing data before loading
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "INFO: Inventory file not found. Starting empty:" << QString::fromStdString(filePath);
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString qline = in.readLine().trimmed();
        if (!qline.isEmpty()) {
            //convert to std::string at the boundary before passing to fromFileString
            bags.append(BloodBag::fromFileString(qline.toStdString()));
        }
    }
    file.close();
    qDebug() << "Loaded" << bags.size() << "blood bags from inventory.";
}