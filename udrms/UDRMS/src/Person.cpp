#include "Person.h"

Person::Person(int id, const std::string& fullName)
    : id(id), fullName(fullName) {}

int Person::getId() const {
    return id;
}

std::string Person::getFullName() const {
    return fullName;
}

void Person::setFullName(const std::string& name) {
    fullName = name;
}
