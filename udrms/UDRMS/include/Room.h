#ifndef ROOM_H
#define ROOM_H
using namespace std;
#include <string>
#include <vector>

class Room
{
private:
    string roomNumber;
    int capacity;
    vector<int> occupantIds;

public:
    Room(const string &roomNumber, int capacity);

    string getRoomNumber() const;
    int getCapacity() const;
    int getOccupancy() const;
    bool isFull() const;
    bool hasOccupant(int studentId) const;
    const vector<int> &getOccupantIds() const;

    void addOccupant(int studentId);
    void removeOccupant(int studentId);

    void display() const;
};

#endif
