#ifndef DORMITORY_H
#define DORMITORY_H
using namespace std;
#include <string>
#include <vector>
#include "Room.h"
#include "Restaurant.h"

class Dormitory
{
private:
    int id;
    string name;
    int capacity;
    vector<Room> rooms;
    Restaurant restaurant;

public:
    Dormitory(int id, const string &name, int capacity,
              const string &restaurantName = "Main Restaurant");

    int getId() const;
    string getName() const;
    int getCapacity() const;

    void addRoom(const Room &room);
    Room *findRoom(const string &roomNumber);

    void assignStudent(int studentId, const string &roomNumber);
    void removeStudent(int studentId, const string &roomNumber);

    Restaurant &getRestaurant();
    const Restaurant &getRestaurant() const;
    vector<Room> &getRooms();
    const vector<Room> &getRooms() const;

    int getTotalOccupancy() const;
    void displayRooms() const;
};

#endif
