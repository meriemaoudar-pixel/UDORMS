#ifndef STAFF_H
#define STAFF_H

#include "Person.h"
#include <string>

class Staff : public Person
{
private:
    string role;

public:
    Staff(int id, const string &fullName, const string &role);

    string getStaffRole() const;
    void setStaffRole(const string &r);

    void display() const override;
    string getRole() const override;
};

#endif
