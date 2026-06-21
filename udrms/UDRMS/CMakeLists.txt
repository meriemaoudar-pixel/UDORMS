cmake_minimum_required(VERSION 3.16)
project(UDRMS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB CORE_SOURCES "src/*.cpp")
list(REMOVE_ITEM CORE_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# ---- Console version: no external dependencies, always built ----
add_executable(udrms_console ${CORE_SOURCES} src/main.cpp)

# ---- Qt GUI version: only built if Qt is installed on this machine ----
find_package(Qt6 COMPONENTS Widgets QUIET)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets QUIET)
endif()

if (Qt6_FOUND OR Qt5_FOUND)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)

    add_executable(udrms_gui ${CORE_SOURCES} gui/MainWindow.cpp gui/main_gui.cpp)
    target_include_directories(udrms_gui PRIVATE gui)

    if (Qt6_FOUND)
        target_link_libraries(udrms_gui Qt6::Widgets)
    else()
        target_link_libraries(udrms_gui Qt5::Widgets)
    endif()

    message(STATUS "Qt found -> the 'udrms_gui' target will be built.")
else()
    message(STATUS "Qt not found -> only 'udrms_console' will be built. Install Qt (Widgets) to also build 'udrms_gui'.")
endif()
