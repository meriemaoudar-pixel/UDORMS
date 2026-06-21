#include "MainWindow.h"
#include "FileManager.h"
#include "Exceptions.h"

#include <QTabWidget>
#include <QTableWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>

namespace {
const QString DATA_FOLDER = "data";
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), university("ENSIA Campus") {
    setWindowTitle("University Dormitory & Restaurant Management System");
    resize(900, 600);

    auto* tabs = new QTabWidget(this);
    tabs->addTab(buildStudentsTab(), "Students");
    tabs->addTab(buildDormitoriesTab(), "Dormitories && Rooms");
    tabs->addTab(buildRestaurantTab(), "Restaurant");

    auto* central = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->addWidget(tabs);

    auto* persistenceRow = new QHBoxLayout();
    auto* saveBtn = new QPushButton("Save Data", this);
    auto* loadBtn = new QPushButton("Load Data", this);
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveData);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoadData);
    persistenceRow->addStretch();
    persistenceRow->addWidget(loadBtn);
    persistenceRow->addWidget(saveBtn);
    mainLayout->addLayout(persistenceRow);

    setCentralWidget(central);

    refreshStudentsTable();
    refreshDormitoriesTable();
    refreshDormCombos();
    refreshRestaurantPanel();
}

// ==================== Students tab ====================

QWidget* MainWindow::buildStudentsTab() {
    auto* page = new QWidget(this);
    auto* layout = new QVBoxLayout(page);

    studentsTable = new QTableWidget(0, 5, page);
    studentsTable->setHorizontalHeaderLabels({"ID", "Name", "Year", "Dormitory", "Room"});
    studentsTable->horizontalHeader()->setStretchLastSection(true);
    studentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    studentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    layout->addWidget(studentsTable);

    auto* formBox = new QGroupBox("Add student", page);
    auto* form = new QFormLayout(formBox);
    studentIdEdit = new QLineEdit(formBox);
    studentIdEdit->setValidator(new QIntValidator(0, 999999999, formBox));
    studentNameEdit = new QLineEdit(formBox);
    studentYearEdit = new QLineEdit(formBox);
    form->addRow("Student ID:", studentIdEdit);
    form->addRow("Full name:", studentNameEdit);
    form->addRow("Academic year:", studentYearEdit);

    auto* btnRow = new QHBoxLayout();
    auto* addBtn = new QPushButton("Add Student", formBox);
    auto* removeBtn = new QPushButton("Remove Selected Student", formBox);
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveStudent);
    btnRow->addWidget(addBtn);
    btnRow->addWidget(removeBtn);
    form->addRow(btnRow);

    layout->addWidget(formBox);
    return page;
}

void MainWindow::onAddStudent() {
    if (studentIdEdit->text().isEmpty() || studentNameEdit->text().isEmpty()) {
        showError("Student ID and full name are required.");
        return;
    }
    try {
        int id = studentIdEdit->text().toInt();
        Student s(id, studentNameEdit->text().toStdString(), studentYearEdit->text().toStdString());
        university.addStudent(s);
        studentIdEdit->clear();
        studentNameEdit->clear();
        studentYearEdit->clear();
        refreshStudentsTable();
        refreshDormCombos();
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::onRemoveStudent() {
    int row = studentsTable->currentRow();
    if (row < 0) {
        showError("Select a student in the table first.");
        return;
    }
    int id = studentsTable->item(row, 0)->text().toInt();
    try {
        university.removeStudent(id);
        refreshStudentsTable();
        refreshDormitoriesTable();
        refreshDormCombos();
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::refreshStudentsTable() {
    const auto& students = university.getStudents();
    studentsTable->setRowCount(static_cast<int>(students.size()));
    int row = 0;
    for (const auto& [id, s] : students) {
        studentsTable->setItem(row, 0, new QTableWidgetItem(QString::number(s.getId())));
        studentsTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s.getFullName())));
        studentsTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(s.getAcademicYear())));
        studentsTable->setItem(row, 3, new QTableWidgetItem(
            s.isAssigned() ? QString::number(s.getDormitoryId()) : QString("-")));
        studentsTable->setItem(row, 4, new QTableWidgetItem(
            s.isAssigned() ? QString::fromStdString(s.getRoomNumber()) : QString("-")));
        ++row;
    }
}

// ==================== Dormitories & Rooms tab ====================

QWidget* MainWindow::buildDormitoriesTab() {
    auto* page = new QWidget(this);
    auto* layout = new QHBoxLayout(page);

    // ---- left: overview table ----
    auto* leftCol = new QVBoxLayout();
    dormitoriesTable = new QTableWidget(0, 3, page);
    dormitoriesTable->setHorizontalHeaderLabels({"Dormitory", "Room", "Occupancy"});
    dormitoriesTable->horizontalHeader()->setStretchLastSection(true);
    dormitoriesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    leftCol->addWidget(dormitoriesTable);
    layout->addLayout(leftCol, 2);

    // ---- right: forms ----
    auto* rightCol = new QVBoxLayout();

    auto* dormBox = new QGroupBox("Add dormitory", page);
    auto* dormForm = new QFormLayout(dormBox);
    dormIdEdit = new QLineEdit(dormBox);
    dormIdEdit->setValidator(new QIntValidator(0, 999999999, dormBox));
    dormNameEdit = new QLineEdit(dormBox);
    dormCapacitySpin = new QSpinBox(dormBox);
    dormCapacitySpin->setRange(1, 100000);
    restaurantNameEdit = new QLineEdit(dormBox);
    restaurantNameEdit->setPlaceholderText("Main Restaurant");
    dormForm->addRow("Dormitory ID:", dormIdEdit);
    dormForm->addRow("Name:", dormNameEdit);
    dormForm->addRow("Capacity:", dormCapacitySpin);
    dormForm->addRow("Restaurant name:", restaurantNameEdit);
    auto* addDormBtn = new QPushButton("Add Dormitory", dormBox);
    connect(addDormBtn, &QPushButton::clicked, this, &MainWindow::onAddDormitory);
    dormForm->addRow(addDormBtn);
    rightCol->addWidget(dormBox);

    auto* roomBox = new QGroupBox("Add room", page);
    auto* roomForm = new QFormLayout(roomBox);
    roomDormCombo = new QComboBox(roomBox);
    roomNumberEdit = new QLineEdit(roomBox);
    roomCapacitySpin = new QSpinBox(roomBox);
    roomCapacitySpin->setRange(1, 20);
    roomForm->addRow("Dormitory:", roomDormCombo);
    roomForm->addRow("Room number:", roomNumberEdit);
    roomForm->addRow("Capacity:", roomCapacitySpin);
    auto* addRoomBtn = new QPushButton("Add Room", roomBox);
    connect(addRoomBtn, &QPushButton::clicked, this, &MainWindow::onAddRoom);
    roomForm->addRow(addRoomBtn);
    rightCol->addWidget(roomBox);

    auto* assignBox = new QGroupBox("Assign / remove student", page);
    auto* assignForm = new QFormLayout(assignBox);
    assignStudentCombo = new QComboBox(assignBox);
    assignDormCombo = new QComboBox(assignBox);
    assignRoomEdit = new QLineEdit(assignBox);
    assignForm->addRow("Student:", assignStudentCombo);
    assignForm->addRow("Dormitory:", assignDormCombo);
    assignForm->addRow("Room number:", assignRoomEdit);
    auto* assignBtn = new QPushButton("Assign to Room", assignBox);
    connect(assignBtn, &QPushButton::clicked, this, &MainWindow::onAssignStudent);
    assignForm->addRow(assignBtn);

    unassignStudentCombo = new QComboBox(assignBox);
    assignForm->addRow("Housed student:", unassignStudentCombo);
    auto* unassignBtn = new QPushButton("Remove from Room", assignBox);
    connect(unassignBtn, &QPushButton::clicked, this, &MainWindow::onUnassignStudent);
    assignForm->addRow(unassignBtn);

    rightCol->addWidget(assignBox);
    rightCol->addStretch();
    layout->addLayout(rightCol, 1);

    return page;
}

void MainWindow::onAddDormitory() {
    if (dormIdEdit->text().isEmpty() || dormNameEdit->text().isEmpty()) {
        showError("Dormitory ID and name are required.");
        return;
    }
    try {
        int id = dormIdEdit->text().toInt();
        QString restName = restaurantNameEdit->text().isEmpty() ? "Main Restaurant" : restaurantNameEdit->text();
        Dormitory d(id, dormNameEdit->text().toStdString(), dormCapacitySpin->value(), restName.toStdString());
        university.addDormitory(d);
        dormIdEdit->clear();
        dormNameEdit->clear();
        restaurantNameEdit->clear();
        refreshDormitoriesTable();
        refreshDormCombos();
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::onAddRoom() {
    if (roomDormCombo->count() == 0 || roomNumberEdit->text().isEmpty()) {
        showError("Add a dormitory first, then provide a room number.");
        return;
    }
    try {
        int dormId = roomDormCombo->currentData().toInt();
        university.findDormitory(dormId).addRoom(Room(roomNumberEdit->text().toStdString(), roomCapacitySpin->value()));
        roomNumberEdit->clear();
        refreshDormitoriesTable();
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::onAssignStudent() {
    if (assignStudentCombo->count() == 0 || assignDormCombo->count() == 0 || assignRoomEdit->text().isEmpty()) {
        showError("A student, a dormitory, and a room number are all required.");
        return;
    }
    try {
        int studentId = assignStudentCombo->currentData().toInt();
        int dormId = assignDormCombo->currentData().toInt();
        university.assignStudentToDormitory(studentId, dormId, assignRoomEdit->text().toStdString());
        assignRoomEdit->clear();
        refreshStudentsTable();
        refreshDormitoriesTable();
        refreshDormCombos();
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::onUnassignStudent() {
    if (unassignStudentCombo->count() == 0) {
        showError("No housed student to remove.");
        return;
    }
    try {
        int studentId = unassignStudentCombo->currentData().toInt();
        university.removeStudentFromDormitory(studentId);
        refreshStudentsTable();
        refreshDormitoriesTable();
        refreshDormCombos();
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::refreshDormitoriesTable() {
    const auto& dorms = university.getDormitories();
    int totalRooms = 0;
    for (const auto& d : dorms) totalRooms += static_cast<int>(d.getRooms().size());

    dormitoriesTable->setRowCount(totalRooms == 0 ? static_cast<int>(dorms.size()) : totalRooms);
    int row = 0;
    for (const auto& d : dorms) {
        if (d.getRooms().empty()) {
            dormitoriesTable->setItem(row, 0, new QTableWidgetItem(
                QString::fromStdString(d.getName()) + " (#" + QString::number(d.getId()) + ")"));
            dormitoriesTable->setItem(row, 1, new QTableWidgetItem("(no rooms yet)"));
            dormitoriesTable->setItem(row, 2, new QTableWidgetItem("-"));
            ++row;
            continue;
        }
        for (const auto& r : d.getRooms()) {
            dormitoriesTable->setItem(row, 0, new QTableWidgetItem(
                QString::fromStdString(d.getName()) + " (#" + QString::number(d.getId()) + ")"));
            dormitoriesTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(r.getRoomNumber())));
            dormitoriesTable->setItem(row, 2, new QTableWidgetItem(
                QString("%1/%2%3").arg(r.getOccupancy()).arg(r.getCapacity()).arg(r.isFull() ? " (FULL)" : "")));
            ++row;
        }
    }
}

void MainWindow::refreshDormCombos() {
    roomDormCombo->clear();
    assignDormCombo->clear();
    assignStudentCombo->clear();
    unassignStudentCombo->clear();
    menuDormCombo->blockSignals(true);
    menuDormCombo->clear();

    for (const auto& d : university.getDormitories()) {
        const QString label = QString::fromStdString(d.getName()) + " (#" + QString::number(d.getId()) + ")";
        roomDormCombo->addItem(label, d.getId());
        assignDormCombo->addItem(label, d.getId());
        menuDormCombo->addItem(label, d.getId());
    }
    for (const auto& [id, s] : university.getStudents()) {
        const QString label = QString::fromStdString(s.getFullName()) + " (#" + QString::number(id) + ")";
        if (!s.isAssigned()) {
            assignStudentCombo->addItem(label, id);
        } else {
            unassignStudentCombo->addItem(label, id);
        }
    }

    menuDormCombo->blockSignals(false);
    refreshRestaurantPanel();
}

// ==================== Restaurant tab ====================

QWidget* MainWindow::buildRestaurantTab() {
    auto* page = new QWidget(this);
    auto* layout = new QVBoxLayout(page);

    auto* selectorRow = new QHBoxLayout();
    selectorRow->addWidget(new QLabel("Dormitory:", page));
    menuDormCombo = new QComboBox(page);
    connect(menuDormCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onDormSelectionChangedForMenu);
    selectorRow->addWidget(menuDormCombo);
    selectorRow->addStretch();
    layout->addLayout(selectorRow);

    auto* menuBox = new QGroupBox("Today's menu", page);
    auto* form = new QFormLayout(menuBox);
    breakfastEdit = new QLineEdit(menuBox);
    lunchEdit = new QLineEdit(menuBox);
    dinnerEdit = new QLineEdit(menuBox);
    form->addRow("Breakfast:", breakfastEdit);
    form->addRow("Lunch:", lunchEdit);
    form->addRow("Dinner:", dinnerEdit);
    auto* saveMenuBtn = new QPushButton("Save Menu", menuBox);
    connect(saveMenuBtn, &QPushButton::clicked, this, &MainWindow::onSaveMenu);
    form->addRow(saveMenuBtn);
    layout->addWidget(menuBox);

    auto* mealsBox = new QGroupBox("Meals served today", page);
    auto* mealsLayout = new QHBoxLayout(mealsBox);
    mealsToAddSpin = new QSpinBox(mealsBox);
    mealsToAddSpin->setRange(1, 10000);
    auto* recordBtn = new QPushButton("Record Meal(s) Served", mealsBox);
    connect(recordBtn, &QPushButton::clicked, this, &MainWindow::onRecordMeal);
    mealsLayout->addWidget(mealsToAddSpin);
    mealsLayout->addWidget(recordBtn);
    mealsLayout->addStretch();
    layout->addWidget(mealsBox);

    restaurantSummary = new QTextEdit(page);
    restaurantSummary->setReadOnly(true);
    layout->addWidget(restaurantSummary);

    return page;
}

void MainWindow::onDormSelectionChangedForMenu(int /*index*/) {
    refreshRestaurantPanel();
}

void MainWindow::onSaveMenu() {
    if (menuDormCombo->count() == 0) {
        showError("Add a dormitory first.");
        return;
    }
    try {
        int dormId = menuDormCombo->currentData().toInt();
        Menu m;
        m.breakfast = breakfastEdit->text().toStdString();
        m.lunch = lunchEdit->text().toStdString();
        m.dinner = dinnerEdit->text().toStdString();
        university.findDormitory(dormId).getRestaurant().setMenu(m);
        showInfo("Menu updated.");
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::onRecordMeal() {
    if (menuDormCombo->count() == 0) {
        showError("Add a dormitory first.");
        return;
    }
    try {
        int dormId = menuDormCombo->currentData().toInt();
        university.findDormitory(dormId).getRestaurant().recordMealServed(mealsToAddSpin->value());
        refreshRestaurantPanel();
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::refreshRestaurantPanel() {
    if (menuDormCombo->count() == 0) {
        breakfastEdit->clear();
        lunchEdit->clear();
        dinnerEdit->clear();
        restaurantSummary->clear();
        return;
    }
    try {
        int dormId = menuDormCombo->currentData().toInt();
        Dormitory& d = university.findDormitory(dormId);
        Menu m = d.getRestaurant().getMenu();
        breakfastEdit->setText(QString::fromStdString(m.breakfast));
        lunchEdit->setText(QString::fromStdString(m.lunch));
        dinnerEdit->setText(QString::fromStdString(m.dinner));

        QString summary = QString("Restaurant: %1\nMeals served today: %2\nDormitory occupancy: %3/%4")
            .arg(QString::fromStdString(d.getRestaurant().getName()))
            .arg(d.getRestaurant().getMealsServedToday())
            .arg(d.getTotalOccupancy())
            .arg(d.getCapacity());
        restaurantSummary->setPlainText(summary);
    } catch (const UDRMSException&) {
        // Combo box temporarily out of sync during a refresh; ignore.
    }
}

// ==================== Persistence ====================

void MainWindow::onSaveData() {
    try {
        FileManager::saveAll(DATA_FOLDER.toStdString(), university);
        showInfo("Data saved to '" + DATA_FOLDER + "/'.");
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

void MainWindow::onLoadData() {
    try {
        University fresh(university.getName());
        FileManager::loadAll(DATA_FOLDER.toStdString(), fresh);
        university = std::move(fresh);
        refreshStudentsTable();
        refreshDormitoriesTable();
        refreshDormCombos();
        showInfo("Data loaded from '" + DATA_FOLDER + "/'.");
    } catch (const UDRMSException& e) {
        showError(QString::fromStdString(e.what()));
    }
}

// ==================== Helpers ====================

void MainWindow::showError(const QString& message) {
    QMessageBox::warning(this, "Error", message);
}

void MainWindow::showInfo(const QString& message) {
    QMessageBox::information(this, "Info", message);
}
