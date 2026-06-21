# University Dormitory & Restaurant Management System (UDRMS)

A C++ console + Qt GUI implementation of the dormitory/restaurant project
brief, organized into header/implementation pairs as required by the
"Code Organization" evaluation criterion.

## File structure

```
UDRMS/
├── include/              # All class declarations (.h)
│   ├── Exceptions.h          custom exception hierarchy for error handling
│   ├── Person.h               abstract base class (id, fullName, display())
│   ├── Student.h               : public Person
│   ├── Staff.h                  : public Person
│   ├── Room.h                  room number, capacity, occupant IDs
│   ├── Restaurant.h            menu + meals-served counter
│   ├── Dormitory.h             owns Rooms + exactly one Restaurant (composition)
│   ├── University.h            owns all Students and Dormitories
│   └── FileManager.h            save/load everything to/from text files
│
├── src/                  # Matching implementations (.cpp) + console entry point
│   ├── Person.cpp, Student.cpp, Staff.cpp
│   ├── Room.cpp, Restaurant.cpp, Dormitory.cpp
│   ├── University.cpp, FileManager.cpp
│   └── main.cpp              console demo / test harness for the whole system
│
├── gui/                  # Qt Widgets graphical interface
│   ├── MainWindow.h / MainWindow.cpp   tabs: Students | Dormitories & Rooms | Restaurant
│   └── main_gui.cpp                     GUI entry point
│
├── data/                 # Created automatically by FileManager when you save
│   ├── students.txt
│   ├── dormitories.txt
│   ├── rooms.txt
│   └── staff.txt
│
└── CMakeLists.txt        # builds udrms_console always; builds udrms_gui only if Qt is found
```

## Why this split

- **One class per header/cpp pair.** Each class lives in exactly one place,
  which is what the "Code Organization" criterion is checking for.
- **`include/` vs `src/`** is the standard C++ convention: declarations
  (what an object looks like) separate from definitions (how it behaves).
  It also lets `gui/` and `src/` both `#include` the same headers without
  duplicating any class.
- **`gui/` is separate from `src/`** so the console version has zero
  dependency on Qt. `src/main.cpp` and `gui/main_gui.cpp` are two different
  `main()` functions; CMake builds them as two separate executables that
  share every other file.
- **Composition vs inheritance, made concrete in the file layout:**
  `Dormitory` *contains* a `Restaurant` and a `vector<Room>` by value
  (composition — see `Dormitory.h`), while `Student` and `Staff` *inherit*
  from `Person` (see `Person.h`, which declares the pure-virtual
  `display()` that each subclass overrides — that's the polymorphism;
  `University::displayRoster()` in `University.cpp` calls it through
  `Person*` to show it in action).

## Building

### Console version (no dependencies beyond a C++17 compiler)
```bash
g++ -std=c++17 -Iinclude src/*.cpp -o udrms_console
./udrms_console
```
or, with CMake:
```bash
mkdir build && cd build
cmake ..
cmake --build .
./udrms_console
```

### GUI version (requires Qt 5 or 6 "Widgets" installed, e.g. via Qt Creator
or `apt install qt6-base-dev` / `qtbase5-dev`)
```bash
mkdir build && cd build
cmake ..              # will print "Qt found -> the 'udrms_gui' target will be built."
cmake --build .
./udrms_gui
```
If Qt isn't installed, CMake simply skips the `udrms_gui` target and only
builds the console version — nothing breaks.

## Data persistence

`FileManager::saveAll("data", university)` writes four pipe-delimited text
files (shown above); `FileManager::loadAll("data", university)` reads them
back and rebuilds every `Student`, `Dormitory`, `Room`, and `Restaurant`
object, including room occupancy. Both the console menu (options 12/13)
and the GUI's *Save Data* / *Load Data* buttons call these same two
functions — persistence logic only exists once.

## What's intentionally left for you to extend

- **UML diagrams** (class diagram, optionally a sequence diagram) — draw
  these from the structure above using a UML tool (e.g. draw.io, PlantUML,
  StarUML); the project brief asks for them as separate deliverables.
- **Weekly menus / meal history** — `Restaurant` currently tracks one
  "today's menu" and a daily meal counter, matching the brief's "(Optional)"
  tag; extend `Restaurant` with a `std::map<std::string, Menu>` keyed by
  weekday if your instructor wants a full week.
- **More input validation in the GUI** — `QIntValidator` is wired up for
  ID fields as a starting point; extend it to dormitory/room fields too.
