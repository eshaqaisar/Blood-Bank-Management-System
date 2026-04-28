#include <iostream>

class BloodBag {
public:
    BloodBag(int bloodType, int volume) : bloodType(bloodType), volume(volume) {}

    void displayInfo() {
        std::cout << "Blood Type: " << bloodType << std::endl;
        std::cout << "Volume: " << volume << " ml" << std::endl;
    }

private:
    int bloodType; // 0: A+, 1: A-, 2: B+, 3: B-, 4: O+, 5: O-, 6: AB+, 7: AB-
    int volume;
};

int main() {
    BloodBag bag(1, 450);
    bag.displayInfo();
    return 0;
}