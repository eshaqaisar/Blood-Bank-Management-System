# BloodInventory

class BloodInventory {
public:
    // Constructor
    BloodInventory();
    
    // Destructor
    ~BloodInventory();
    
    // Add blood bag
    void addBloodBag(const BloodBag& bloodBag);
    
    // Remove blood bag
    void removeBloodBag(const std::string& bloodBagId);
    
    // Get total blood bags
    int getTotalBloodBags() const;
    
    // Check blood bag availability
    bool isBloodBagAvailable(const std::string& bloodBagId) const;
    
private:
    std::vector<BloodBag> bloodBags;
};