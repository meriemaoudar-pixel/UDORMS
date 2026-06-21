#include "University.h"
#include "Exceptions.h"
#include <iostream>

University::University(const std::string &name) : name(name) {}

std::string University::getName() const { return name; }

// ---------------- Students ----------------

void University::addStudent(const Student &s)
{
    if (students.find(s.getId()) != students.end())
    {
        throw DuplicateIdException(s.getId());
    }
    students.emplace(s.getId(), s);
}

void University::removeStudent(int studentId)
{
    auto it = students.find(studentId);
    if (it == students.end())
    {
        throw NotFoundException("Student " + std::to_string(studentId));
    }

    if (it->second.isAssigned())
    {
        removeStudentFromDormitory(studentId);
    }
    students.erase(it);
}

Student &University::findStudent(int studentId)
{
    auto it = students.find(studentId);
    if (it == students.end())
    {
        throw NotFoundException("Student " + std::to_string(studentId));
    }
    return it->second;
}

std::map<int, Student> &University::getStudents() { return students; }
const std::map<int, Student> &University::getStudents() const { return students; }

// ---------------- Staff ----------------

void University::addStaff(const Staff &s)
{
    staff.push_back(s);
}

std::vector<Staff> &University::getStaff() { return staff; }
const std::vector<Staff> &University::getStaff() const { return staff; }

// ---------------- Dormitories ----------------

void University::addDormitory(const Dormitory &d)
{
    for (const auto &existing : dormitories)
    {
        if (existing.getId() == d.getId())
        {
            throw DuplicateIdException(d.getId());
        }
    }
    dormitories.push_back(d);
}

Dormitory &University::findDormitory(int dormId)
{
    for (auto &d : dormitories)
    {
        if (d.getId() == dormId)
            return d;
    }
    throw NotFoundException("Dormitory " + std::to_string(dormId));
}

std::vector<Dormitory> &University::getDormitories() { return dormitories; }
const std::vector<Dormitory> &University::getDormitories() const { return dormitories; }

// ---------------- Cross-cutting operations ----------------

void University::assignStudentToDormitory(int studentId, int dormId, const std::string &roomNumber)
{
    Student &s = findStudent(studentId);
    Dormitory &d = findDormitory(dormId);

    if (s.isAssigned())
    {

        removeStudentFromDormitory(studentId);
    }

    d.assignStudent(studentId, roomNumber);
    s.assignAccommodation(dormId, roomNumber);
}

void University::removeStudentFromDormitory(int studentId)
{
    Student &s = findStudent(studentId);
    if (!s.isAssigned())
    {
        throw InvalidDataException("Student " + std::to_string(studentId) + " is not currently housed");
    }
    Dormitory &d = findDormitory(s.getDormitoryId());
    d.removeStudent(studentId, s.getRoomNumber());
    s.clearAccommodation();
}

void University::displayRoster() const
{
    std::cout << "--- Roster (" << name << ") ---\n";
    std::vector<const Person *> roster;
    for (const auto &[id, st] : students)
        roster.push_back(&st);
    for (const auto &sf : staff)
        roster.push_back(&sf);

    for (const Person *p : roster)
    {
        p->display();
    }
}

void University::displayAllStudents() const
{
    std::cout << "--- Students (" << students.size() << ") ---\n";
    for (const auto &[id, s] : students)
    {
        s.display();
    }
}

void University::displayAllDormitories() const
{
    std::cout << "--- Dormitories (" << dormitories.size() << ") ---\n";
    for (const auto &d : dormitories)
    {
        d.displayRooms();
    }
}
