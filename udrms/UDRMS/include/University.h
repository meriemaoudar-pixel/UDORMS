#ifndef UNIVERSITY_H
#define UNIVERSITY_H
using namespace std;
#include <string>
#include <map>
#include <vector>
#include "Student.h"
#include "Staff.h"
#include "Dormitory.h"

class University
{
private:
    string name;
    map<int, Student> students;
    vector<Dormitory> dormitories;
    vector<Staff> staff;

public:
    explicit University(const string &name);

    std::string getName() const;

    void addStudent(const Student &s);
    void removeStudent(int studentId);
    Student &findStudent(int studentId);
    map<int, Student> &getStudents();
    const map<int, Student> &getStudents() const;

    void addStaff(const Staff &s);
    vector<Staff> &getStaff();
    const vector<Staff> &getStaff() const;

    void addDormitory(const Dormitory &d);
    Dormitory &findDormitory(int dormId);
    vector<Dormitory> &getDormitories();
    const vector<Dormitory> &getDormitories() const;

    void assignStudentToDormitory(int studentId, int dormId, const string &roomNumber);
    void removeStudentFromDormitory(int studentId);

    void displayRoster() const;

    void displayAllStudents() const;
    void displayAllDormitories() const;
};

#endif
