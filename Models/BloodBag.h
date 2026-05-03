#ifndef BLOODBAG_H
#define BLOODBAG_H

#include <QString>//for string handling of blood group, donor name, etc.
#include <QDate>//for handling collection date and calculating expiry based on it


class BloodBag {
private:
    QString bloodGroup;    // "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
    int     units;         //how many units are in this bag (typically 1)
    QDate   collectionDate; //date when this blood was collected from donor
    QString donorName;     //who donated this bag

public:
    static const int EXPIRY_DAYS = 42; //medical standard: blood expires after 42 days

    //constructor
    BloodBag(const QString& bloodGroup, int units,
        const QDate& collectionDate, const QString& donorName = "");

    //getters
    QString getBloodGroup()    const;
    int     getUnits()         const;
    QDate   getCollectionDate() const;
    QString getDonorName()     const;
    int     getDaysUntilExpiry() const; //returns negative if already expired

    //setters
    void setUnits(int u);

    //returns true if this bag is past its 42-day limit
    bool isExpired() const;

    //file handling
    //format: bloodGroup,units,collectionDate,donorName
    QString    toFileString()         const;
    static BloodBag fromFileString(const QString& line);
};


#include <QList>//for storing a list of BloodBag objects in the BloodInventory class
#include <QMap>//for mapping blood groups to available unit counts in the inventory

class BloodInventory {
private:
    QList<BloodBag> bags; //AGGREGATION: BloodInventory holds many BloodBags

    //low stock threshold triggers emergency alert below this number
    static const int LOW_STOCK_THRESHOLD = 5;

public:
    //inventory management
    void    addBag(const BloodBag& bag);              //add a new donated bag
    bool    useBag(const QString& bloodGroup, int unitsNeeded); //remove on approval
    void    removeExpiredBags();                       //clean up old blood
    int     getAvailableUnits(const QString& bg) const; //count available units
    bool    isLowStock(const QString& bloodGroup) const; //below threshold?

    //getters
    QList<BloodBag>         getAllBags()       const;
    QMap<QString, int>      getInventoryMap() const; // { "A+": 10, "O-": 2, ... }
    QList<BloodBag>         getExpiredBags()  const; //only expired bags

    //file handling
    void save(const QString& filePath)         const;
    void load(const QString& filePath);
};

#endif // BLOODBAG_H
