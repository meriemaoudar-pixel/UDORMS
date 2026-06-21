#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "University.h"

class QTableWidget;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // Students tab
    void onAddStudent();
    void onRemoveStudent();

    // Dormitories tab
    void onAddDormitory();
    void onAddRoom();
    void onAssignStudent();
    void onUnassignStudent();

    // Restaurant tab
    void onDormSelectionChangedForMenu(int index);
    void onSaveMenu();
    void onRecordMeal();

    // Persistence
    void onSaveData();
    void onLoadData();

private:
    University university;

    QWidget *buildStudentsTab();
    QWidget *buildDormitoriesTab();
    QWidget *buildRestaurantTab();

    void refreshStudentsTable();
    void refreshDormitoriesTable();
    void refreshDormCombos(); // keeps every dormitory QComboBox in sync
    void refreshRestaurantPanel();

    void showError(const QString &message);
    void showInfo(const QString &message);

    // Students tab widgets
    QTableWidget *studentsTable = nullptr;
    QLineEdit *studentIdEdit = nullptr;
    QLineEdit *studentNameEdit = nullptr;
    QLineEdit *studentYearEdit = nullptr;

    // Dormitories tab widgets
    QTableWidget *dormitoriesTable = nullptr;
    QLineEdit *dormIdEdit = nullptr;
    QLineEdit *dormNameEdit = nullptr;
    QSpinBox *dormCapacitySpin = nullptr;
    QLineEdit *restaurantNameEdit = nullptr;

    QComboBox *roomDormCombo = nullptr;
    QLineEdit *roomNumberEdit = nullptr;
    QSpinBox *roomCapacitySpin = nullptr;

    QComboBox *assignStudentCombo = nullptr;
    QComboBox *assignDormCombo = nullptr;
    QLineEdit *assignRoomEdit = nullptr;
    QComboBox *unassignStudentCombo = nullptr;

    // Restaurant tab widgets
    QComboBox *menuDormCombo = nullptr;
    QLineEdit *breakfastEdit = nullptr;
    QLineEdit *lunchEdit = nullptr;
    QLineEdit *dinnerEdit = nullptr;
    QSpinBox *mealsToAddSpin = nullptr;
    QTextEdit *restaurantSummary = nullptr;
};

#endif // MAINWINDOW_H
