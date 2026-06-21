#include <iostream>
#include <limits>
#include "University.h"
#include "FileManager.h"
#include "Exceptions.h"

namespace
{

    const std::string DATA_FOLDER = "data";

    int readInt(const std::string &prompt)
    {
        int value;
        while (true)
        {
            std::cout << prompt;
            if (std::cin >> value)
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cout << "  Please enter a valid number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string readLine(const std::string &prompt)
    {
        std::cout << prompt;
        std::string value;
        std::getline(std::cin, value);
        return value;
    }

    void printMenu()
    {
        std::cout << "\n==================== UDRMS ====================\n"
                  << " 1. Add student\n"
                  << " 2. Remove student\n"
                  << " 3. List all students\n"
                  << " 4. Add dormitory\n"
                  << " 5. Add room to dormitory\n"
                  << " 6. Assign student to a room\n"
                  << " 7. Remove student from their room\n"
                  << " 8. Display dormitories (rooms + restaurant)\n"
                  << " 9. Set restaurant menu\n"
                  << "10. Record a meal served\n"
                  << "11. Show full roster (polymorphism demo)\n"
                  << "12. Save data to disk\n"
                  << "13. Load data from disk\n"
                  << " 0. Exit\n"
                  << "================================================\n"
                  << "Choice: ";
    }

} // namespace

int main()
{
    University uni("ENSIA Campus");

    std::cout << "University Dormitory & Restaurant Management System\n";
    std::cout << "(console demo - see gui/ for the Qt graphical interface)\n";

    bool running = true;
    while (running)
    {
        printMenu();
        int choice = readInt("");

        try
        {
            switch (choice)
            {
            case 1:
            {
                int id = readInt("Student ID: ");
                std::string fullName = readLine("Full name: ");
                std::string year = readLine("Academic year: ");
                uni.addStudent(Student(id, fullName, year));
                std::cout << "Student added.\n";
                break;
            }
            case 2:
            {
                int id = readInt("Student ID to remove: ");
                uni.removeStudent(id);
                std::cout << "Student removed.\n";
                break;
            }
            case 3:
                uni.displayAllStudents();
                break;
            case 4:
            {
                int id = readInt("Dormitory ID: ");
                std::string name = readLine("Dormitory name: ");
                int capacity = readInt("Dormitory capacity: ");
                std::string restName = readLine("Restaurant name: ");
                uni.addDormitory(Dormitory(id, name, capacity, restName));
                std::cout << "Dormitory added.\n";
                break;
            }
            case 5:
            {
                int dormId = readInt("Dormitory ID: ");
                std::string roomNumber = readLine("Room number: ");
                int capacity = readInt("Room capacity: ");
                uni.findDormitory(dormId).addRoom(Room(roomNumber, capacity));
                std::cout << "Room added.\n";
                break;
            }
            case 6:
            {
                int studentId = readInt("Student ID: ");
                int dormId = readInt("Dormitory ID: ");
                std::string roomNumber = readLine("Room number: ");
                uni.assignStudentToDormitory(studentId, dormId, roomNumber);
                std::cout << "Student assigned.\n";
                break;
            }
            case 7:
            {
                int studentId = readInt("Student ID: ");
                uni.removeStudentFromDormitory(studentId);
                std::cout << "Student unassigned.\n";
                break;
            }
            case 8:
                uni.displayAllDormitories();
                break;
            case 9:
            {
                int dormId = readInt("Dormitory ID: ");
                Menu m;
                m.breakfast = readLine("Breakfast: ");
                m.lunch = readLine("Lunch: ");
                m.dinner = readLine("Dinner: ");
                uni.findDormitory(dormId).getRestaurant().setMenu(m);
                std::cout << "Menu updated.\n";
                break;
            }
            case 10:
            {
                int dormId = readInt("Dormitory ID: ");
                int count = readInt("Meals to add: ");
                uni.findDormitory(dormId).getRestaurant().recordMealServed(count);
                std::cout << "Recorded.\n";
                break;
            }
            case 11:
                uni.displayRoster();
                break;
            case 12:
                FileManager::saveAll(DATA_FOLDER, uni);
                std::cout << "Data saved to '" << DATA_FOLDER << "/'.\n";
                break;
            case 13:
                FileManager::loadAll(DATA_FOLDER, uni);
                std::cout << "Data loaded from '" << DATA_FOLDER << "/'.\n";
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Unknown option.\n";
            }
        }
        catch (const UDRMSException &e)
        {
            // All project-specific errors land here with a clear message.
            std::cout << "Error: " << e.what() << "\n";
        }
        catch (const std::exception &e)
        {
            // Safety net for anything unexpected (e.g. std::stoi failures).
            std::cout << "Unexpected error: " << e.what() << "\n";
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}
