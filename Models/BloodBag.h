#ifndef BLOODBAG_H
#define BLOODBAG_H

#include <QString>
#include <QDate>

// ============================================================
// BloodBag.h  |  Domain 2: Zara Shah
// OOP Concepts: ENCAPSULATION + AGGREGATION
// ============================================================
//
// BloodBag represents a single physical bag of blood in storage.
// Each bag has a blood type, quantity, and collection date.
//
// ENCAPSULATION:
//   All attributes are private. Business logic (expiry check)
//   is inside the class, not scattered across the app.
//
// AGGREGATION:
//   BloodInventory (a separate class) contains a list of BloodBag objects.
//   BloodBag can exist independently — it does NOT belong to BloodInventory.
//   This is "HAS-A" relationship without ownership.
//
// Medical rule: Blood is safe to use for 42 days after collection.
// After that it must be discarded (flagged as expired).
// ============================================================

class BloodBag {
private:
    QString bloodGroup;    // "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
    int     units;         // How many units are in this bag (typically 1)
    QDate   collectionDate; // Date when this blood was collected from donor
    QString donorName;     // Who donated this bag

public:
    static const int EXPIRY_DAYS = 42; // Medical standard: blood expires after 42 days

    // Constructor
    BloodBag(const QString& bloodGroup, int units,
        const QDate& collectionDate, const QString& donorName = "");

    // ---- Getters ----
    QString getBloodGroup()    const;
    int     getUnits()         const;
    QDate   getCollectionDate() const;
    QString getDonorName()     const;
    int     getDaysUntilExpiry() const; // Returns negative if already expired

    // ---- Setters ----
    void setUnits(int u);

    // ---- Business Logic ----
    // Returns true if this bag is past its 42-day limit
    bool isExpired() const;

    // ---- File Handling ----
    // Format: bloodGroup,units,collectionDate,donorName
    QString    toFileString()         const;
    static BloodBag fromFileString(const QString& line);
};

// ============================================================
// BloodInventory  |  Domain 2: Zara Shah
// OOP Concepts: AGGREGATION
// ============================================================
//
// BloodInventory is a MANAGER class.
// It AGGREGATES (holds a list of) BloodBag objects.
// It manages adding, using, and tracking blood stock.
//
// AGGREGATION vs COMPOSITION:
//   Aggregation: BloodBag objects can exist without BloodInventory.
//   Composition: The contained object CANNOT exist without the owner.
//   Here it's Aggregation because a BloodBag is just stored, not owned.
// ============================================================

#include <QList>
#include <QMap>

class BloodInventory {
private:
    QList<BloodBag> bags; // AGGREGATION: BloodInventory holds many BloodBags

    // Low stock threshold — triggers emergency alert below this number
    static const int LOW_STOCK_THRESHOLD = 5;

public:
    // ---- Inventory Management ----
    void    addBag(const BloodBag& bag);              // Add a new donated bag
    bool    useBag(const QString& bloodGroup, int unitsNeeded); // Remove on approval
    void    removeExpiredBags();                       // Clean up old blood
    int     getAvailableUnits(const QString& bg) const; // Count available units
    bool    isLowStock(const QString& bloodGroup) const; // Below threshold?

    // ---- Getters ----
    QList<BloodBag>         getAllBags()       const;
    QMap<QString, int>      getInventoryMap() const; // { "A+": 10, "O-": 2, ... }
    QList<BloodBag>         getExpiredBags()  const; // Only expired bags

    // ---- File Handling ----
    void save(const QString& filePath)         const;
    void load(const QString& filePath);
};

#endif // BLOODBAG_H
