#include "FileManager.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace
{

    std::vector<std::string> split(const std::string &line, char delim)
    {
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, delim))
        {
            fields.push_back(field);
        }

        if (!line.empty() && line.back() == delim)
        {
            fields.push_back("");
        }
        return fields;
    }

}

void FileManager::saveAll(const std::string &folder, const University &uni)
{
    fs::create_directories(folder);

    {
        std::ofstream out(folder + "/dormitories.txt");
        if (!out)
            throw InvalidDataException("Could not open dormitories.txt for writing");
        for (const auto &d : uni.getDormitories())
        {
            const Menu m = d.getRestaurant().getMenu();
            out << d.getId() << '|' << d.getName() << '|' << d.getCapacity() << '|'
                << d.getRestaurant().getName() << '|' << m.breakfast << '|' << m.lunch << '|'
                << m.dinner << '|' << d.getRestaurant().getMealsServedToday() << '\n';
        }
    }

    {
        std::ofstream out(folder + "/rooms.txt");
        if (!out)
            throw InvalidDataException("Could not open rooms.txt for writing");
        for (const auto &d : uni.getDormitories())
        {
            for (const auto &r : d.getRooms())
            {
                out << d.getId() << '|' << r.getRoomNumber() << '|' << r.getCapacity() << '|';
                const auto &occupants = r.getOccupantIds();
                for (size_t i = 0; i < occupants.size(); ++i)
                {
                    out << occupants[i];
                    if (i + 1 < occupants.size())
                        out << ',';
                }
                out << '\n';
            }
        }
    }

    {
        std::ofstream out(folder + "/students.txt");
        if (!out)
            throw InvalidDataException("Could not open students.txt for writing");
        for (const auto &[id, s] : uni.getStudents())
        {
            out << s.getId() << '|' << s.getFullName() << '|' << s.getAcademicYear() << '|'
                << s.getDormitoryId() << '|' << s.getRoomNumber() << '\n';
        }
    }

    {
        std::ofstream out(folder + "/staff.txt");
        if (!out)
            throw InvalidDataException("Could not open staff.txt for writing");
        for (const auto &s : uni.getStaff())
        {
            out << s.getId() << '|' << s.getFullName() << '|' << s.getStaffRole() << '\n';
        }
    }
}

void FileManager::loadAll(const std::string &folder, University &uni)
{

    {
        std::ifstream in(folder + "/dormitories.txt");
        std::string line;
        while (in && std::getline(in, line))
        {
            if (line.empty())
                continue;
            auto f = split(line, '|');
            if (f.size() < 8)
                throw InvalidDataException("Malformed line in dormitories.txt");
            int id = std::stoi(f[0]);
            int capacity = std::stoi(f[2]);
            Dormitory d(id, f[1], capacity, f[3]);
            Menu m;
            m.breakfast = f[4];
            m.lunch = f[5];
            m.dinner = f[6];
            d.getRestaurant().setMenu(m);
            d.getRestaurant().recordMealServed(std::stoi(f[7]));
            uni.addDormitory(d);
        }
    }

    {
        std::ifstream in(folder + "/rooms.txt");
        std::string line;
        while (in && std::getline(in, line))
        {
            if (line.empty())
                continue;
            auto f = split(line, '|');
            if (f.size() < 4)
                throw InvalidDataException("Malformed line in rooms.txt");
            int dormId = std::stoi(f[0]);
            int capacity = std::stoi(f[2]);
            Room room(f[1], capacity);
            if (!f[3].empty())
            {
                for (const auto &occStr : split(f[3], ','))
                {
                    if (!occStr.empty())
                    {
                        room.addOccupant(std::stoi(occStr));
                    }
                }
            }
            uni.findDormitory(dormId).addRoom(room);
        }
    }

    {
        std::ifstream in(folder + "/students.txt");
        std::string line;
        while (in && std::getline(in, line))
        {
            if (line.empty())
                continue;
            auto f = split(line, '|');
            if (f.size() < 5)
                throw InvalidDataException("Malformed line in students.txt");
            int id = std::stoi(f[0]);
            Student s(id, f[1], f[2]);
            int dormId = std::stoi(f[3]);
            if (dormId != -1 && !f[4].empty())
            {
                s.assignAccommodation(dormId, f[4]);
            }
            uni.addStudent(s);
        }
    }

    {
        std::ifstream in(folder + "/staff.txt");
        std::string line;
        while (in && std::getline(in, line))
        {
            if (line.empty())
                continue;
            auto f = split(line, '|');
            if (f.size() < 3)
                throw InvalidDataException("Malformed line in staff.txt");
            int id = std::stoi(f[0]);
            uni.addStaff(Staff(id, f[1], f[2]));
        }
    }
}
