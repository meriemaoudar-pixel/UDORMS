#ifndef PERSON_H
#define PERSON_H
using namespace std;
#include <string>

class Person
{
protected:
    int id;
    string fullName;

public:
    Person(int id, const string &fullName);
    virtual ~Person() = default;

    int getId() const;
    string getFullName() const;
    void setFullName(const string &name);

    virtual void display() const = 0;
    virtual std::string getRole() const = 0;
};

#endif
