#ifndef BLOODBAG_H
#define BLOODBAG_H

#include <string>//std::string replaces QString for blood group and donor name
#include <QDate>//for handling collection date and calculating expiry based on it
#include <QMap>//QMap<QString,int> for inventory map returned to Qt UI layer only

//maximum number of blood bags the inventory can hold at one time.
//QList<BloodBag> replaced with a fixed-size plain array of BloodBag + a count integer.
#define MAX_BLOOD_BAGS 500

class BloodBag {
private:
    std::string bloodGroup;     // "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
    int         units;          //how many units are in this bag (typically 1)
    QDate       collectionDate; //date when this blood was collected from donor
    std::string donorName;      //who donated this bag

public:
    static const int EXPIRY_DAYS = 42; //medical standard: blood expires after 42 days

    //default constructor needed so BloodBag can be used in plain arrays
    BloodBag();

    //main constructor
    BloodBag(const std::string& bloodGroup, int units,
        const QDate& collectionDate, const std::string& donorName = "");

    //getters
    std::string getBloodGroup()      const;
    int         getUnits()           const;
    QDate       getCollectionDate()  const;
    std::string getDonorName()       const;
    int         getDaysUntilExpiry() const; //returns negative if already expired

    //setters
    void setUnits(int u);

    //returns true if this bag is past its 42-day limit
    bool isExpired() const;

    //file handling -- format: bloodGroup,units,collectionDate,donorName
    std::string     toFileString()                        const;
    static BloodBag fromFileString(const std::string& line);
};


//BagArray: a simple plain-array wrapper that replaces QList<BloodBag>.
//Holds up to MAX_BLOOD_BAGS bags with an integer count of how many are filled.
struct BagArray {
    BloodBag data[MAX_BLOOD_BAGS];
    int      count;

    BagArray() : count(0) {}

    //append a bag; silently ignores if array is full
    void append(const BloodBag& bag) {
        if (count < MAX_BLOOD_BAGS) data[count++] = bag;
    }

    //clear all entries
    void clear() { count = 0; }

    //access by index
    BloodBag& operator[](int i) { return data[i]; }
    const BloodBag& operator[](int i) const { return data[i]; }
};


class BloodInventory {
private:
    //QList<BloodBag> replaced with plain fixed-size array + count
    BloodBag bags[MAX_BLOOD_BAGS];
    int      bagCount; //how many slots are actually filled

    //low stock threshold triggers emergency alert below this number
    static const int LOW_STOCK_THRESHOLD = 5;

public:
    BloodInventory() : bagCount(0) {}

    //inventory management
    void addBag(const BloodBag& bag);                              //add a new donated bag
    bool useBag(const std::string& bloodGroup, int unitsNeeded);   //remove on approval; returns false if insufficient stock
    void removeExpiredBags();                                       //clean up old blood
    int  getAvailableUnits(const std::string& bg)  const;          //count available units
    bool isLowStock(const std::string& bloodGroup) const;          //below threshold?

    //getters -- return BagArray (plain array wrapper) instead of QList
    BagArray           getAllBags()      const; //all bags including expired
    BagArray           getExpiredBags() const;  //only expired bags
    QMap<QString, int> getInventoryMap() const; //blood group -> units, for Qt UI only

    //file handling
    void save(const std::string& filePath) const;
    void load(const std::string& filePath);
};

#endif // BLOODBAG_H