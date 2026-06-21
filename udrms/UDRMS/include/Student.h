#ifndef STUDENT_H
#define STUDENT_H
using namespace std;
#include "Person.h"
#include <string>

class Student : public Person
{
private:
    string academicYear;
    int dormitoryId;
    string roomNumber;

public:
    Student(int id, const string &fullName, const string &academicYear);

    string getAcademicYear() const;
    void setAcademicYear(const string &year);

    int getDormitoryId() const;
    string getRoomNumber() const;
    bool isAssigned() const;

    void assignAccommodation(int dormId, const string &room);
    void clearAccommodation();

    void display() const override;
    string getRole() const override;
};

#endif
