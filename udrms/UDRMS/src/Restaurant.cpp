#include "Restaurant.h"
#include <iostream>

Restaurant::Restaurant(const std::string& name)
    : name(name), mealsServedToday(0) {}

std::string Restaurant::getName() const {
    return name;
}

void Restaurant::setMenu(const Menu& menu) {
    todayMenu = menu;
}

Menu Restaurant::getMenu() const {
    return todayMenu;
}

void Restaurant::recordMealServed(int count) {
    mealsServedToday += count;
}

int Restaurant::getMealsServedToday() const {
    return mealsServedToday;
}

void Restaurant::resetDailyCount() {
    mealsServedToday = 0;
}

void Restaurant::display() const {
    std::cout << "  Restaurant: " << name << "\n"
              << "    Breakfast: " << todayMenu.breakfast << "\n"
              << "    Lunch:     " << todayMenu.lunch << "\n"
              << "    Dinner:    " << todayMenu.dinner << "\n"
              << "    Meals served today: " << mealsServedToday << "\n";
}
