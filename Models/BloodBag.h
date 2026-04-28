#ifndef BLOODBAG_H
#define BLOODBAG_H

class BloodBag {
public:
    BloodBag();
    ~BloodBag();

    void fill(int volume);
    int getVolume() const;
    bool isFull() const;

private:
    int volume;
    const int capacity;
};

#endif // BLOODBAG_H
