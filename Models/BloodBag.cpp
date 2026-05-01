#include "BloodBag.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

// ============================================================
// BloodBag.cpp  |  Domain 2: Zara Shah
// ============================================================

// ============================================================
// BloodBag Implementation
// ============================================================

BloodBag::BloodBag(const QString& bloodGroup, int units,
    const QDate& collectionDate, const QString& donorName)
    : bloodGroup(bloodGroup), units(units),
    collectionDate(collectionDate), donorName(donorName)
{
}

// ---- Getters ----
QString BloodBag::getBloodGroup()     const { return bloodGroup; }
int     BloodBag::getUnits()          const { return units; }
QDate   BloodBag::getCollectionDate() const { return collectionDate; }
QString BloodBag::getDonorName()      const { return donorName; }

// How many days until this bag expires? (negative = already expired)
int BloodBag::getDaysUntilExpiry() const {
    QDate expiryDate = collectionDate.addDays(EXPIRY_DAYS);
    return QDate::currentDate().daysTo(expiryDate);
}

void BloodBag::setUnits(int u) { units = u; }

// ---- Expiry Check ----
// A bag is expired if more than 42 days have passed since collection.
bool BloodBag::isExpired() const {
    return collectionDate.daysTo(QDate::currentDate()) > EXPIRY_DAYS;
}

// ---- Serialize ----
QString BloodBag::toFileString() const {
    return bloodGroup + "," +
        QString::number(units) + "," +
        collectionDate.toString("yyyy-MM-dd") + "," +
        donorName;
}

// ---- Deserialize from inventory.txt ----
BloodBag BloodBag::fromFileString(const QString& line) {
    QStringList p = line.split(",");
    if (p.size() < 4)
        return BloodBag("A+", 0, QDate::currentDate(), "Unknown");
    return BloodBag(
        p[0],                                   // bloodGroup
        p[1].toInt(),                           // units
        QDate::fromString(p[2], "yyyy-MM-dd"), // collectionDate
        p[3]                                    // donorName
    );
}

// ============================================================
// BloodInventory Implementation
// ============================================================

// Add a new blood bag to the inventory (called after successful donation)
void BloodInventory::addBag(const BloodBag& bag) {
    bags.append(bag);
}

// Use blood from inventory when a request is approved.
// Finds bags of matching blood group and deducts units.
// Returns false if not enough stock is available.
bool BloodInventory::useBag(const QString& bloodGroup, int unitsNeeded) {
    int available = getAvailableUnits(bloodGroup);
    if (available < unitsNeeded) return false; // Not enough stock

    // Deduct from bags one by one until we have enough
    int remaining = unitsNeeded;
    for (BloodBag& bag : bags) {
        if (bag.getBloodGroup() == bloodGroup && !bag.isExpired() && remaining > 0) {
            int take = qMin(bag.getUnits(), remaining);
            bag.setUnits(bag.getUnits() - take);
            remaining -= take;
        }
    }

    // Remove bags that are now empty (units == 0)
    // NOTE: Using manual loop instead of removeIf() for Qt 5 compatibility
    QList<BloodBag> nonEmpty;
    for (const BloodBag& b : bags) {
        if (b.getUnits() > 0) nonEmpty.append(b);
    }
    bags = nonEmpty;
    return true;
}

// Scan all bags and remove those past their 42-day limit
// Qt5-compatible: manual loop instead of removeIf()
void BloodInventory::removeExpiredBags() {
    int before = bags.size();
    QList<BloodBag> fresh;
    for (const BloodBag& b : bags) {
        if (!b.isExpired()) fresh.append(b);
    }
    bags = fresh;
    int removed = before - bags.size();
    qDebug() << "Removed" << removed << "expired blood bags.";
}

// Count total available (non-expired) units of a given blood group
int BloodInventory::getAvailableUnits(const QString& bg) const {
    int total = 0;
    for (const BloodBag& bag : bags) {
        if (bag.getBloodGroup() == bg && !bag.isExpired()) {
            total += bag.getUnits();
        }
    }
    return total;
}

// Check if stock for a blood group is critically low
bool BloodInventory::isLowStock(const QString& bloodGroup) const {
    return getAvailableUnits(bloodGroup) < LOW_STOCK_THRESHOLD;
}

QList<BloodBag> BloodInventory::getAllBags() const { return bags; }

// Returns a map of blood group → total available units
// Example: { "A+": 15, "O-": 2, "B+": 8, ... }
QMap<QString, int> BloodInventory::getInventoryMap() const {
    QMap<QString, int> map;
    QStringList groups = { "A+","A-","B+","B-","AB+","AB-","O+","O-" };
    for (const QString& g : groups) {
        map[g] = getAvailableUnits(g);
    }
    return map;
}

// Returns only expired bags (for admin report)
QList<BloodBag> BloodInventory::getExpiredBags() const {
    QList<BloodBag> expired;
    for (const BloodBag& bag : bags) {
        if (bag.isExpired()) expired.append(bag);
    }
    return expired;
}

// ---- Save inventory to inventory.txt ----
void BloodInventory::save(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "ERROR: Cannot open inventory file for writing:" << filePath;
        return;
    }
    QTextStream out(&file);
    for (const BloodBag& bag : bags) {
        out << bag.toFileString() << "\n";
    }
    file.close();
}

// ---- Load inventory from inventory.txt ----
void BloodInventory::load(const QString& filePath) {
    bags.clear(); // Clear existing data before loading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "INFO: Inventory file not found. Starting empty:" << filePath;
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            bags.append(BloodBag::fromFileString(line));
        }
    }
    file.close();
    qDebug() << "Loaded" << bags.size() << "blood bags from inventory.";
}
