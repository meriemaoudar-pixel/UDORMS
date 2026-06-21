#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
using namespace std;
#include <exception>
#include <string>

class UDRMSException : public exception
{
protected:
    string message;

public:
    explicit UDRMSException(const string &msg) : message(msg) {}
    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

class RoomFullException : public UDRMSException
{
public:
    explicit RoomFullException(const string &roomNumber)
        : UDRMSException("Room " + roomNumber + " is already at full capacity.") {}
};

class NotFoundException : public UDRMSException
{
public:
    explicit NotFoundException(const string &what)
        : UDRMSException(what + " was not found.") {}
};

class DuplicateIdException : public UDRMSException
{
public:
    explicit DuplicateIdException(int id)
        : UDRMSException("ID " + to_string(id) + " is already in use.") {}
};

class InvalidDataException : public UDRMSException
{
public:
    explicit InvalidDataException(const string &reason)
        : UDRMSException("Invalid data: " + reason) {}
};

#endif
