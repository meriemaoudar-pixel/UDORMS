#include "Room.h"
#include "Exceptions.h"
#include <algorithm>
#include <iostream>

Room::Room(const std::string &roomNumber, int capacity)
    : roomNumber(roomNumber), capacity(capacity) {}

std::string Room::getRoomNumber() const
{
    return roomNumber;
}

int Room::getCapacity() const
{
    return capacity;
}

int Room::getOccupancy() const
{
    return static_cast<int>(occupantIds.size());
}

bool Room::isFull() const
{
    return getOccupancy() >= capacity;
}

bool Room::hasOccupant(int studentId) const
{
    return std::find(occupantIds.begin(), occupantIds.end(), studentId) != occupantIds.end();
}

const std::vector<int> &Room::getOccupantIds() const
{
    return occupantIds;
}

void Room::addOccupant(int studentId)
{
    if (isFull())
    {
        throw RoomFullException(roomNumber);
    }
    if (hasOccupant(studentId))
    {
        return;
    }
    occupantIds.push_back(studentId);
}

void Room::removeOccupant(int studentId)
{
    auto it = std::find(occupantIds.begin(), occupantIds.end(), studentId);
    if (it == occupantIds.end())
    {
        throw NotFoundException("Student " + std::to_string(studentId) + " in room " + roomNumber);
    }
    occupantIds.erase(it);
}

void Room::display() const
{
    std::cout << "  Room " << roomNumber << " (" << getOccupancy() << "/" << capacity << ")";
    if (isFull())
        std::cout << " [FULL]";
    std::cout << "\n";
}
