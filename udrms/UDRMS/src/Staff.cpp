#include "Staff.h"
#include <iostream>

Staff::Staff(int id, const std::string& fullName, const std::string& role)
    : Person(id, fullName), role(role) {}

std::string Staff::getStaffRole() const {
    return role;
}

void Staff::setStaffRole(const std::string& r) {
    role = r;
}

void Staff::display() const {
    std::cout << "[Staff]   #" << id << " - " << fullName << " (" << role << ")\n";
}

std::string Staff::getRole() const {
    return "Staff";
}
