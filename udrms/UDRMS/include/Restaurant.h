#ifndef RESTAURANT_H
#define RESTAURANT_H
using namespace std;
#include <string>

struct Menu
{
    string breakfast = "Not set";
    string lunch = "Not set";
    string dinner = "Not set";
};

class Restaurant
{
private:
    string name;
    Menu todayMenu;
    int mealsServedToday;

public:
    explicit Restaurant(const string &name = "Main Restaurant");

    string getName() const;

    void setMenu(const Menu &menu);
    Menu getMenu() const;

    void recordMealServed(int count = 1);
    int getMealsServedToday() const;
    void resetDailyCount();

    void display() const;
};

#endif
