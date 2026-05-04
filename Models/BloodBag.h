#ifndef BLOODBAG_H
#define BLOODBAG_H

#include <string>//std::string replaces QString for blood group and donor name
#include <QDate>//for handling collection date and calculating expiry based on it
#include <QList>//QList<BloodBag> kept for BloodInventory internal storage (Qt container)
#include <QMap>//QMap<std::string,int> for inventory map keyed by blood group string

class BloodBag {
private:
    std::string bloodGroup;    // "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
    int         units;         //how many units are in this bag (typically 1)
    QDate       collectionDate; //date when this blood was collected from donor
    std::string donorName;     //who donated this bag

public:
    static const int EXPIRY_DAYS = 42; //medical standard: blood expires after 42 days

    //constructor
    BloodBag(const std::string& bloodGroup, int units,
        const QDate& collectionDate, const std::string& donorName = "");

    //getters
    std::string getBloodGroup()     const;
    int         getUnits()          const;
    QDate       getCollectionDate() const;
    std::string getDonorName()      const;
    int         getDaysUntilExpiry() const; //returns negative if already expired

    //setters
    void setUnits(int u);

    //returns true if this bag is past its 42-day limit
    bool isExpired() const;

    //file handling
    //format: bloodGroup,units,collectionDate,donorName
    std::string         toFileString()                        const;
    static BloodBag     fromFileString(const std::string& line);
};


class BloodInventory {
private:
    QList<BloodBag> bags; //AGGREGATION: BloodInventory holds many BloodBags

    //low stock threshold triggers emergency alert below this number
    static const int LOW_STOCK_THRESHOLD = 5;

public:
    //inventory management
    void    addBag(const BloodBag& bag);                              //add a new donated bag
    bool    useBag(const std::string& bloodGroup, int unitsNeeded);   //remove on approval; returns false if insufficient stock
    void    removeExpiredBags();                                       //clean up old blood
    int     getAvailableUnits(const std::string& bg)   const;         //count available units
    bool    isLowStock(const std::string& bloodGroup)  const;         //below threshold?

    //getters
    QList<BloodBag>         getAllBags()       const;
    QMap<QString, int>      getInventoryMap() const; // { "A+": 10, "O-": 2, ... }
    QList<BloodBag>         getExpiredBags()  const; //only expired bags

    //file handling
    void save(const std::string& filePath) const;
    void load(const std::string& filePath);
};

#endif // BLOODBAG_H