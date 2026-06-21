#include "Student.h"
#include <iostream>

Student::Student(int id, const std::string& fullName, const std::string& academicYear)
    : Person(id, fullName), academicYear(academicYear), dormitoryId(-1), roomNumber("") {}

std::string Student::getAcademicYear() const {
    return academicYear;
}

void Student::setAcademicYear(const std::string& year) {
    academicYear = year;
}

int Student::getDormitoryId() const {
    return dormitoryId;
}

std::string Student::getRoomNumber() const {
    return roomNumber;
}

bool Student::isAssigned() const {
    return dormitoryId != -1;
}

void Student::assignAccommodation(int dormId, const std::string& room) {
    dormitoryId = dormId;
    roomNumber = room;
}

void Student::clearAccommodation() {
    dormitoryId = -1;
    roomNumber = "";
}

void Student::display() const {
    std::cout << "[Student] #" << id << " - " << fullName
              << " (Year " << academicYear << ") - ";
    if (isAssigned()) {
        std::cout << "Dorm " << dormitoryId << ", Room " << roomNumber << "\n";
    } else {
        std::cout << "Not housed\n";
    }
}

std::string Student::getRole() const {
    return "Student";
}
