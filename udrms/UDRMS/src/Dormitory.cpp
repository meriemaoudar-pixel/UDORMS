#include "Dormitory.h"
#include "Exceptions.h"
#include <iostream>
using namespace std;

Dormitory::Dormitory(int id, const string& name, int capacity,
                      const string& restaurantName)
    : id(id), name(name), capacity(capacity), restaurant(restaurantName) {}

int Dormitory::getId() const { return id; }
string Dormitory::getName() const { return name; }
int Dormitory::getCapacity() const { return capacity; }

void Dormitory::addRoom(const Room& room) {
    if (findRoom(room.getRoomNumber()) != nullptr) {
        throw InvalidDataException("Room " + room.getRoomNumber() +
                                    " already exists in dormitory " + name);
    }
    rooms.push_back(room);
}

Room* Dormitory::findRoom(const string& roomNumber) {
    for (auto& r : rooms) {
        if (r.getRoomNumber() == roomNumber) return &r;
    }
    return nullptr;
}

void Dormitory::assignStudent(int studentId, const string& roomNumber) {
    Room* room = findRoom(roomNumber);
    if (!room) {
        throw NotFoundException("Room " + roomNumber + " in dormitory " + name);
    }
    room->addOccupant(studentId); 
}

void Dormitory::removeStudent(int studentId, const string& roomNumber) {
    Room* room = findRoom(roomNumber);
    if (!room) {
        throw NotFoundException("Room " + roomNumber + " in dormitory " + name);
    }
    room->removeOccupant(studentId); 
}

Restaurant& Dormitory::getRestaurant() { return restaurant; }
const Restaurant& Dormitory::getRestaurant() const { return restaurant; }

vector<Room>& Dormitory::getRooms() { return rooms; }
const vector<Room>& Dormitory::getRooms() const { return rooms; }

int Dormitory::getTotalOccupancy() const {
    int total = 0;
    for (const auto& r : rooms) total += r.getOccupancy();
    return total;
}

void Dormitory::displayRooms() const {
    cout << "Dormitory #" << id << " - " << name
              << " (" << getTotalOccupancy() << "/" << capacity << " students)\n";
    for (const auto& r : rooms) {
        r.display();
    }
    restaurant.display();
}
