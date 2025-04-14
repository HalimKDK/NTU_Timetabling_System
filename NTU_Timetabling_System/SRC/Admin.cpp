/**
  *
  * This file contains the implementation of the Admin class.
  *
  * The Admin class handles all administrative operations for the
  * timetabling system—things like managing modules, groups, sessions,
  * rooms, users, and timetables. It also defines how the admin menu
  * is displayed and how user choices are handled.
  */

#include "Admin.h"
#include "TimetableManager.h"
#include "Module.h"
#include "Group.h"
#include "SessionType.h"
#include "Room.h"
#include "Student.h"
#include "Lecturer.h"
#include "Session.h"
#include "Timetable.h"
#include "TimeSlot.h"
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

 // Constructor
Admin::Admin(const std::string& username, const std::string& password)
    : User(username, password, User::UserType::ADMIN) {
    // Any additional initialization code
}

// Destructor
Admin::~Admin() {
    // Cleanup if needed
}

// Helper functions to convert between string and enum
Room::RoomType Admin::stringToRoomType(const std::string& typeStr) const {
    if (typeStr == "LECTURE_HALL") return Room::RoomType::LECTURE_HALL;
    if (typeStr == "LAB") return Room::RoomType::LAB;
    if (typeStr == "TUTORIAL") return Room::RoomType::TUTORIAL;
    if (typeStr == "MEETING_ROOM") return Room::RoomType::MEETING_ROOM;
    return Room::RoomType::OTHER;
}

std::string Admin::roomTypeToString(Room::RoomType type) const {
    switch (type) {
    case Room::RoomType::LECTURE_HALL: return "LECTURE_HALL";
    case Room::RoomType::LAB: return "LAB";
    case Room::RoomType::TUTORIAL: return "TUTORIAL";
    case Room::RoomType::MEETING_ROOM: return "MEETING_ROOM";
    default: return "OTHER";
    }
}

// Display admin-specific menu (from User base class)
void Admin::displayMenu() const {
    displayMainMenu();
}

// Display the main menu for admin actions (from UserInterface)
void Admin::displayMainMenu() const {
    std::cout << "\n=== Admin Menu ===\n";
    std::cout << "1. Manage Modules\n";
    std::cout << "2. Manage Groups\n";
    std::cout << "3. Manage Session Types\n";
    std::cout << "4. Manage Students\n";
    std::cout << "5. Manage Lecturers\n";
    std::cout << "6. Manage Rooms\n";
    std::cout << "7. Manage Timetable\n";
    std::cout << "0. Logout\n";
    std::cout << "Enter your choice: ";
}

// Process the main menu choice
bool Admin::processMenuChoice(int choice) {
    switch (choice) {
    case 1:
        displayModuleMenu();
        return true;
    case 2:
        displayGroupMenu();
        return true;
    case 3:
        displaySessionTypeMenu();
        return true;
    case 4:
        displayStudentMenu();
        return true;
    case 5:
        displayLecturerMenu();
        return true;
    case 6:
        displayRoomMenu();
        return true;
    case 7:
        displayTimetableMenu();
        return true;
    case 0:
        std::cout << "Logging out..." << std::endl;
        return false;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        return true;
    }
}

// Run the admin interface
bool Admin::run() {
    int choice;
    bool running = true;

    while (running) {
        displayMainMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        running = processMenuChoice(choice);
    }

    return true;
}

// Module Management
void Admin::displayModuleMenu() const {
    std::cout << "\n=== Module Management ===\n";
    std::cout << "1. Add Module\n";
    std::cout << "2. Edit Module\n";
    std::cout << "3. Delete Module\n";
    std::cout << "4. List Modules\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

bool Admin::processModuleMenuChoice(int choice) {
    switch (choice) {
    case 1:
        addModule();
        return true;
    case 2:
        editModule();
        return true;
    case 3:
        deleteModule();
        return true;
    case 4:
        listModules();
        return true;
    case 0:
        return false;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        return true;
    }
}

void Admin::addModule() {
    std::string code, name;

    std::cout << "Enter module code: ";
    std::getline(std::cin, code);

    std::cout << "Enter module name: ";
    std::getline(std::cin, name);

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        Module module(code, name);
        manager.addModule(module);
        std::cout << "Module added successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error adding module: " << e.what() << std::endl;
    }
}

void Admin::editModule() {
    int index = getModuleSelection();
    if (index < 0) return;

    std::string name;

    std::cout << "Enter new module name (or leave empty to keep current): ";
    std::getline(std::cin, name);

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        Module& module = manager.getModuleAt(index);

        if (!name.empty()) {
            module.setName(name);
        }

        std::cout << "Module updated successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating module: " << e.what() << std::endl;
    }
}

void Admin::deleteModule() {
    int index = getModuleSelection();
    if (index < 0) return;

    char confirm;
    std::cout << "Are you sure you want to delete this module? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (tolower(confirm) == 'y') {
        TimetableManager& manager = TimetableManager::getInstance();
        if (manager.removeModule(index)) {
            std::cout << "Module deleted successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to delete module." << std::endl;
        }
    }
}

void Admin::listModules() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& modules = manager.getModules();

    if (modules.empty()) {
        std::cout << "No modules found." << std::endl;
        return;
    }

    std::cout << "\n=== Modules ===\n";
    for (size_t i = 0; i < modules.size(); ++i) {
        std::cout << i + 1 << ". " << modules[i].getCode() << " - "
            << modules[i].getName() << std::endl;
    }
}

// Group Management
void Admin::displayGroupMenu() const {
    std::cout << "\n=== Group Management ===\n";
    std::cout << "1. Add Group\n";
    std::cout << "2. Edit Group\n";
    std::cout << "3. Delete Group\n";
    std::cout << "4. List Groups\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

bool Admin::processGroupMenuChoice(int choice) {
    switch (choice) {
    case 1:
        addGroup();
        return true;
    case 2:
        editGroup();
        return true;
    case 3:
        deleteGroup();
        return true;
    case 4:
        listGroups();
        return true;
    case 0:
        return false;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        return true;
    }
}

void Admin::addGroup() {
    std::string code, name;

    std::cout << "Enter group code: ";
    std::getline(std::cin, code);

    std::cout << "Enter group name: ";
    std::getline(std::cin, name);

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        Group group(code, name);
        manager.addGroup(group);
        std::cout << "Group added successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error adding group: " << e.what() << std::endl;
    }
}

void Admin::editGroup() {
    int index = getGroupSelection();
    if (index < 0) return;

    std::string name;

    std::cout << "Enter new group name (or leave empty to keep current): ";
    std::getline(std::cin, name);

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        Group& group = manager.getGroupAt(index);

        if (!name.empty()) {
            group.setName(name);
        }

        std::cout << "Group updated successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating group: " << e.what() << std::endl;
    }
}

void Admin::deleteGroup() {
    int index = getGroupSelection();
    if (index < 0) return;

    char confirm;
    std::cout << "Are you sure you want to delete this group? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (tolower(confirm) == 'y') {
        TimetableManager& manager = TimetableManager::getInstance();
        if (manager.removeGroup(index)) {
            std::cout << "Group deleted successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to delete group." << std::endl;
        }
    }
}

void Admin::listGroups() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& groups = manager.getGroups();

    if (groups.empty()) {
        std::cout << "No groups found." << std::endl;
        return;
    }

    std::cout << "\n=== Groups ===\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        std::cout << i + 1 << ". " << groups[i].getCode() << " - "
            << groups[i].getName() << std::endl;
    }
}

// Session Type Management
void Admin::displaySessionTypeMenu() const {
    std::cout << "\n=== Session Type Management ===\n";
    std::cout << "1. Add Session Type\n";
    std::cout << "2. Edit Session Type\n";
    std::cout << "3. Delete Session Type\n";
    std::cout << "4. List Session Types\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

bool Admin::processSessionTypeMenuChoice(int choice) {
    switch (choice) {
    case 1:
        addSessionType();
        return true;
    case 2:
        editSessionType();
        return true;
    case 3:
        deleteSessionType();
        return true;
    case 4:
        listSessionTypes();
        return true;
    case 0:
        return false;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        return true;
    }
}

void Admin::addSessionType() {
    std::string code, name, description;

    std::cout << "Enter session type code: ";
    std::getline(std::cin, code);

    std::cout << "Enter session type name: ";
    std::getline(std::cin, name);

    std::cout << "Enter session type description: ";
    std::getline(std::cin, description);

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        SessionType sessionType(code, name, description);
        manager.addSessionType(sessionType);
        std::cout << "Session type added successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error adding session type: " << e.what() << std::endl;
    }
}

void Admin::editSessionType() {
    int index = getSessionTypeSelection();
    if (index < 0) return;

    std::string name, description;

    std::cout << "Enter new session type name (or leave empty to keep current): ";
    std::getline(std::cin, name);

    std::cout << "Enter new session type description (or leave empty to keep current): ";
    std::getline(std::cin, description);

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        SessionType& sessionType = manager.getSessionTypeAt(index);

        if (!name.empty()) {
            sessionType.setName(name);
        }

        if (!description.empty()) {
            sessionType.setDescription(description);
        }

        std::cout << "Session type updated successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating session type: " << e.what() << std::endl;
    }
}

void Admin::deleteSessionType() {
    int index = getSessionTypeSelection();
    if (index < 0) return;

    char confirm;
    std::cout << "Are you sure you want to delete this session type? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (tolower(confirm) == 'y') {
        TimetableManager& manager = TimetableManager::getInstance();
        if (manager.removeSessionType(index)) {
            std::cout << "Session type deleted successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to delete session type." << std::endl;
        }
    }
}

void Admin::listSessionTypes() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& sessionTypes = manager.getSessionTypes();

    if (sessionTypes.empty()) {
        std::cout << "No session types found." << std::endl;
        return;
    }

    std::cout << "\n=== Session Types ===\n";
    for (size_t i = 0; i < sessionTypes.size(); ++i) {
        std::cout << i + 1 << ". " << sessionTypes[i].getCode() << " - "
            << sessionTypes[i].getName() << std::endl;
    }
}

// Student Management
void Admin::displayStudentMenu() const {
    std::cout << "\n=== Student Management ===\n";
    std::cout << "1. Register Student\n";
    std::cout << "2. Assign Student to Group\n";
    std::cout << "3. List Students\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

bool Admin::processStudentMenuChoice(int choice) {
    switch (choice) {
    case 1:
        registerStudent();
        return true;
    case 2:
        assignStudentToGroup();
        return true;
    case 3:
        listStudents();
        return true;
    case 0:
        return false;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        return true;
    }
}

void Admin::registerStudent() {
    std::string studentId, firstName, lastName, username, password;

    std::cout << "Enter student ID: ";
    std::getline(std::cin, studentId);

    std::cout << "Enter first name: ";
    std::getline(std::cin, firstName);

    std::cout << "Enter last name: ";
    std::getline(std::cin, lastName);

    std::cout << "Enter username: ";
    std::getline(std::cin, username);

    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    try {
        auto student = std::make_shared<Student>(studentId, firstName, lastName, username, password);
        TimetableManager& manager = TimetableManager::getInstance();
        manager.addStudent(student);
        std::cout << "Student registered successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error registering student: " << e.what() << std::endl;
    }
}

void Admin::assignStudentToGroup() {
    int studentIndex = getStudentSelection();
    if (studentIndex < 0) return;

    int groupIndex = getGroupSelection();
    if (groupIndex < 0) return;

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        auto student = manager.getStudentAt(studentIndex);
        auto& group = manager.getGroupAt(groupIndex);

        if (manager.assignStudentToGroup(student->getStudentId(), group.getCode())) {
            std::cout << "Student assigned to group successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to assign student to group." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error assigning student to group: " << e.what() << std::endl;
    }
}

void Admin::listStudents() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& students = manager.getStudents();

    if (students.empty()) {
        std::cout << "No students found." << std::endl;
        return;
    }

    std::cout << "\n=== Students ===\n";
    for (size_t i = 0; i < students.size(); ++i) {
        std::cout << i + 1 << ". " << students[i]->getStudentId() << " - "
            << students[i]->getFirstName() << " " << students[i]->getLastName();

        // Show group if assigned
        if (students[i]->getAssignedGroup()) {
            std::cout << " (Group: " << students[i]->getAssignedGroup()->getCode() << ")";
        }

        std::cout << std::endl;
    }
}

// Lecturer Management
void Admin::displayLecturerMenu() const {
    std::cout << "\n=== Lecturer Management ===\n";
    std::cout << "1. Register Lecturer\n";
    std::cout << "2. Assign Lecturer to Module\n";
    std::cout << "3. Assign Lecturer to Session\n";
    std::cout << "4. List Lecturers\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

bool Admin::processLecturerMenuChoice(int choice) {
    switch (choice) {
    case 1:
        registerLecturer();
        return true;
    case 2:
        assignLecturerToModule();
        return true;
    case 3:
        assignLecturerToSession();
        return true;
    case 4:
        listLecturers();
        return true;
    case 0:
        return false;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        return true;
    }
}

void Admin::registerLecturer() {
    std::string id, firstName, lastName, title, department, username, password;

    std::cout << "Enter lecturer ID: ";
    std::getline(std::cin, id);

    std::cout << "Enter first name: ";
    std::getline(std::cin, firstName);

    std::cout << "Enter last name: ";
    std::getline(std::cin, lastName);

    std::cout << "Enter title (Dr., Prof., etc.): ";
    std::getline(std::cin, title);

    std::cout << "Enter department: ";
    std::getline(std::cin, department);

    std::cout << "Enter username: ";
    std::getline(std::cin, username);

    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    try {
        auto lecturer = std::make_shared<Lecturer>(id, firstName, lastName, title, department, username, password);
        TimetableManager& manager = TimetableManager::getInstance();
        manager.addLecturer(lecturer);
        std::cout << "Lecturer registered successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error registering lecturer: " << e.what() << std::endl;
    }
}

void Admin::assignLecturerToModule() {
    int lecturerIndex = getLecturerSelection();
    if (lecturerIndex < 0) return;

    int moduleIndex = getModuleSelection();
    if (moduleIndex < 0) return;

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        auto lecturer = manager.getLecturerAt(lecturerIndex);
        auto& module = manager.getModuleAt(moduleIndex);

        if (manager.assignLecturerToModule(lecturer->getId(), module.getCode())) {
            std::cout << "Lecturer assigned to module successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to assign lecturer to module." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error assigning lecturer to module: " << e.what() << std::endl;
    }
}

void Admin::assignLecturerToSession() {
    // This would be more complex, requiring session creation or selection
    std::cout << "This functionality is part of timetable entry creation." << std::endl;
    std::cout << "Please use the Create Timetable Entry option instead." << std::endl;
}

void Admin::listLecturers() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& lecturers = manager.getLecturers();

    if (lecturers.empty()) {
        std::cout << "No lecturers found." << std::endl;
        return;
    }

    std::cout << "\n=== Lecturers ===\n";
    for (size_t i = 0; i < lecturers.size(); ++i) {
        std::cout << i + 1 << ". " << lecturers[i]->getTitle() << " "
            << lecturers[i]->getFirstName() << " " << lecturers[i]->getLastName()
            << " (" << lecturers[i]->getDepartment() << ")" << std::endl;
    }
}

// Room Management
void Admin::displayRoomMenu() const {
    std::cout << "\n=== Room Management ===\n";
    std::cout << "1. Add Room\n";
    std::cout << "2. Edit Room\n";
    std::cout << "3. Delete Room\n";
    std::cout << "4. List Rooms\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

bool Admin::processRoomMenuChoice(int choice) {
    switch (choice) {
    case 1:
        addRoom();
        return true;
    case 2:
        editRoom();
        return true;
    case 3:
        deleteRoom();
        return true;
    case 4:
        listRooms();
        return true;
    case 0:
        return false;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        return true;
    }
}

void Admin::addRoom() {
    std::string number, name, building, typeStr;
    int capacity;

    std::cout << "Enter room number: ";
    std::getline(std::cin, number);

    std::cout << "Enter room name: ";
    std::getline(std::cin, name);

    std::cout << "Enter capacity: ";
    std::cin >> capacity;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter room type (LECTURE_HALL, LAB, TUTORIAL, MEETING_ROOM, OTHER): ";
    std::getline(std::cin, typeStr);

    std::cout << "Enter building: ";
    std::getline(std::cin, building);

    try {
        // Convert string to Room::RoomType
        Room::RoomType type = stringToRoomType(typeStr);

        TimetableManager& manager = TimetableManager::getInstance();
        Room room(number, name, capacity, type, building);
        manager.addRoom(room);
        std::cout << "Room added successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error adding room: " << e.what() << std::endl;
    }
}

void Admin::editRoom() {
    int index = getRoomSelection();
    if (index < 0) return;

    std::string name, building, typeStr;
    std::string capacityStr;

    std::cout << "Enter new room name (or leave empty to keep current): ";
    std::getline(std::cin, name);

    std::cout << "Enter new building (or leave empty to keep current): ";
    std::getline(std::cin, building);

    std::cout << "Enter new room type (LECTURE_HALL, LAB, TUTORIAL, MEETING_ROOM, OTHER, or leave empty): ";
    std::getline(std::cin, typeStr);

    std::cout << "Enter new capacity (or leave empty to keep current): ";
    std::getline(std::cin, capacityStr);

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        Room& room = manager.getRoomAt(index);

        if (!name.empty()) {
            room.setName(name);
        }

        if (!building.empty()) {
            room.setBuilding(building);
        }

        if (!typeStr.empty()) {
            // Convert string to Room::RoomType
            Room::RoomType type = stringToRoomType(typeStr);
            room.setType(type);
        }

        if (!capacityStr.empty()) {
            room.setCapacity(std::stoi(capacityStr));
        }

        std::cout << "Room updated successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating room: " << e.what() << std::endl;
    }
}

void Admin::deleteRoom() {
    int index = getRoomSelection();
    if (index < 0) return;

    char confirm;
    std::cout << "Are you sure you want to delete this room? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (tolower(confirm) == 'y') {
        TimetableManager& manager = TimetableManager::getInstance();
        if (manager.removeRoom(index)) {
            std::cout << "Room deleted successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to delete room." << std::endl;
        }
    }
}

void Admin::listRooms() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& rooms = manager.getRooms();

    if (rooms.empty()) {
        std::cout << "No rooms found." << std::endl;
        return;
    }

    std::cout << "\n=== Rooms ===\n";
    for (size_t i = 0; i < rooms.size(); ++i) {
        std::cout << i + 1 << ". " << rooms[i].getNumber() << " - "
            << rooms[i].getName() << " ("
            << rooms[i].getTypeString()  // Using getTypeString method to avoid stream operator issues
            << ", Capacity: " << rooms[i].getCapacity() << ")" << std::endl;
    }
}

// Timetable Management
void Admin::displayTimetableMenu() const {
    std::cout << "\n=== Timetable Management ===\n";
    std::cout << "1. Create Timetable Entry\n";
    std::cout << "2. Update Timetable Entry\n";
    std::cout << "3. Delete Timetable Entry\n";
    std::cout << "4. Find Timetable Conflicts\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "Enter your choice: ";
}

bool Admin::processTimetableMenuChoice(int choice) {
    switch (choice) {
    case 1:
        createTimetableEntry();
        return true;
    case 2:
        updateTimetableEntry();
        return true;
    case 3:
        deleteTimetableEntry();
        return true;
    case 4:
        findTimetableConflicts();
        return true;
    case 0:
        return false;
    default:
        std::cout << "Invalid choice. Please try again." << std::endl;
        return true;
    }
}

void Admin::createTimetableEntry() {
    // This would be a complex operation that requires gathering all the necessary
    // data for creating a session. This is a simplified version.
    std::cout << "Creating a new timetable entry...\n";

    int moduleIndex = getModuleSelection();
    if (moduleIndex < 0) return;

    int sessionTypeIndex = getSessionTypeSelection();
    if (sessionTypeIndex < 0) return;

    int roomIndex = getRoomSelection();
    if (roomIndex < 0) return;

    int lecturerIndex = getLecturerSelection();
    if (lecturerIndex < 0) return;

    int weekNumber;
    std::cout << "Enter week number (1-53): ";
    std::cin >> weekNumber;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (weekNumber < 1 || weekNumber > 53) {
        std::cout << "Invalid week number. Must be between 1 and 53." << std::endl;
        return;
    }

    // Get day and time information
    int dayInt;
    std::cout << "Enter day (0=Monday, 1=Tuesday, 2=Wednesday, 3=Thursday, 4=Friday, 5=Saturday, 6=Sunday): ";
    std::cin >> dayInt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (dayInt < 0 || dayInt > 6) {
        std::cout << "Invalid day number. Must be between 0 and 6." << std::endl;
        return;
    }

    int startHour, startMinute, endHour, endMinute;
    std::cout << "Enter start hour (0-23): ";
    std::cin >> startHour;

    std::cout << "Enter start minute (0-59): ";
    std::cin >> startMinute;

    std::cout << "Enter end hour (0-23): ";
    std::cin >> endHour;

    std::cout << "Enter end minute (0-59): ";
    std::cin >> endMinute;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (startHour < 0 || startHour > 23 || startMinute < 0 || startMinute > 59 ||
        endHour < 0 || endHour > 23 || endMinute < 0 || endMinute > 59) {
        std::cout << "Invalid time parameters." << std::endl;
        return;
    }

    // Create a time slot with the correct parameters
    TimeSlot::Day day = static_cast<TimeSlot::Day>(dayInt);
    TimeSlot timeSlot(day, startHour, startMinute, endHour, endMinute);

    // Get groups to assign to this session
    std::vector<std::string> groupCodes;
    char addGroup;
    do {
        int groupIndex = getGroupSelection();
        if (groupIndex >= 0) {
            TimetableManager& manager = TimetableManager::getInstance();
            groupCodes.push_back(manager.getGroupAt(groupIndex).getCode());
            std::cout << "Group added to session. Add another? (y/n): ";
            std::cin >> addGroup;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            break;
        }
    } while (tolower(addGroup) == 'y');

    try {
        TimetableManager& manager = TimetableManager::getInstance();

        std::shared_ptr<Session> session = manager.createSession(
            manager.getModuleAt(moduleIndex).getCode(),
            manager.getSessionTypeAt(sessionTypeIndex).getCode(),
            manager.getRoomAt(roomIndex).getNumber(),
            manager.getLecturerAt(lecturerIndex)->getId(),
            timeSlot,
            weekNumber,
            groupCodes
        );

        if (session) {
            std::cout << "Timetable entry created successfully with ID: " << session->getId() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating timetable entry: " << e.what() << std::endl;
    }
}

void Admin::updateTimetableEntry() {
    // This would require first selecting an existing session, then updating its properties
    std::cout << "This functionality is not fully implemented yet." << std::endl;
    // A complete implementation would require listing all sessions,
    // allowing the user to select one, and then updating its properties
}

void Admin::deleteTimetableEntry() {
    // This would require selecting an existing session to delete
    std::cout << "This functionality is not fully implemented yet." << std::endl;
    // A complete implementation would require listing all sessions,
    // allowing the user to select one, and then deleting it
}

void Admin::findTimetableConflicts() {
    try {
        TimetableManager& manager = TimetableManager::getInstance();
        auto conflicts = manager.findTimetableConflicts();

        if (conflicts.empty()) {
            std::cout << "No timetable conflicts found." << std::endl;
            return;
        }

        std::cout << "\n=== Timetable Conflicts ===\n";
        for (size_t i = 0; i < conflicts.size(); ++i) {
            auto& conflict = conflicts[i];
            std::cout << "Conflict " << i + 1 << ":\n";

            // First session - avoid using << operator directly with complex types
            std::cout << "  Session 1: " << conflict.first->getId();
            std::cout << " - " << conflict.first->getModule().getCode();
            std::cout << " (" << conflict.first->getSessionType().getName() << ")\n";

            std::cout << "    Room: " << conflict.first->getRoom().getNumber();
            std::cout << ", Week: " << conflict.first->getWeekNumber() << "\n";

            // Second session - avoid using << operator directly with complex types
            std::cout << "  Session 2: " << conflict.second->getId();
            std::cout << " - " << conflict.second->getModule().getCode();
            std::cout << " (" << conflict.second->getSessionType().getName() << ")\n";

            std::cout << "    Room: " << conflict.second->getRoom().getNumber();
            std::cout << ", Week: " << conflict.second->getWeekNumber() << "\n";

            std::cout << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error finding timetable conflicts: " << e.what() << std::endl;
    }
}

// Selection helpers
int Admin::getModuleSelection() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& modules = manager.getModules();

    if (modules.empty()) {
        std::cout << "No modules available." << std::endl;
        return -1;
    }

    std::cout << "\n=== Select Module ===\n";
    for (size_t i = 0; i < modules.size(); ++i) {
        std::cout << i + 1 << ". " << modules[i].getCode() << " - "
            << modules[i].getName() << std::endl;
    }
    std::cout << "0. Cancel\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        return -1;
    }

    if (choice < 1 || choice > static_cast<int>(modules.size())) {
        std::cout << "Invalid choice." << std::endl;
        return -1;
    }

    return choice - 1;
}

int Admin::getGroupSelection() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& groups = manager.getGroups();

    if (groups.empty()) {
        std::cout << "No groups available." << std::endl;
        return -1;
    }

    std::cout << "\n=== Select Group ===\n";
    for (size_t i = 0; i < groups.size(); ++i) {
        std::cout << i + 1 << ". " << groups[i].getCode() << " - "
            << groups[i].getName() << std::endl;
    }
    std::cout << "0. Cancel\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        return -1;
    }

    if (choice < 1 || choice > static_cast<int>(groups.size())) {
        std::cout << "Invalid choice." << std::endl;
        return -1;
    }

    return choice - 1;
}

int Admin::getSessionTypeSelection() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& sessionTypes = manager.getSessionTypes();

    if (sessionTypes.empty()) {
        std::cout << "No session types available." << std::endl;
        return -1;
    }

    std::cout << "\n=== Select Session Type ===\n";
    for (size_t i = 0; i < sessionTypes.size(); ++i) {
        std::cout << i + 1 << ". " << sessionTypes[i].getCode() << " - "
            << sessionTypes[i].getName() << std::endl;
    }
    std::cout << "0. Cancel\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        return -1;
    }

    if (choice < 1 || choice > static_cast<int>(sessionTypes.size())) {
        std::cout << "Invalid choice." << std::endl;
        return -1;
    }

    return choice - 1;
}

int Admin::getRoomSelection() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& rooms = manager.getRooms();

    if (rooms.empty()) {
        std::cout << "No rooms available." << std::endl;
        return -1;
    }

    std::cout << "\n=== Select Room ===\n";
    for (size_t i = 0; i < rooms.size(); ++i) {
        std::cout << i + 1 << ". " << rooms[i].getNumber() << " - "
            << rooms[i].getName() << " (" << rooms[i].getTypeString()
            << ", Capacity: " << rooms[i].getCapacity() << ")" << std::endl;
    }
    std::cout << "0. Cancel\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        return -1;
    }

    if (choice < 1 || choice > static_cast<int>(rooms.size())) {
        std::cout << "Invalid choice." << std::endl;
        return -1;
    }

    return choice - 1;
}

int Admin::getStudentSelection() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& students = manager.getStudents();

    if (students.empty()) {
        std::cout << "No students available." << std::endl;
        return -1;
    }

    std::cout << "\n=== Select Student ===\n";
    for (size_t i = 0; i < students.size(); ++i) {
        std::cout << i + 1 << ". " << students[i]->getStudentId() << " - "
            << students[i]->getFirstName() << " " << students[i]->getLastName() << std::endl;
    }
    std::cout << "0. Cancel\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        return -1;
    }

    if (choice < 1 || choice > static_cast<int>(students.size())) {
        std::cout << "Invalid choice." << std::endl;
        return -1;
    }

    return choice - 1;
}

int Admin::getLecturerSelection() const {
    TimetableManager& manager = TimetableManager::getInstance();
    const auto& lecturers = manager.getLecturers();

    if (lecturers.empty()) {
        std::cout << "No lecturers available." << std::endl;
        return -1;
    }

    std::cout << "\n=== Select Lecturer ===\n";
    for (size_t i = 0; i < lecturers.size(); ++i) {
        std::cout << i + 1 << ". " << lecturers[i]->getId() << " - "
            << lecturers[i]->getTitle() << " " << lecturers[i]->getFirstName()
            << " " << lecturers[i]->getLastName() << std::endl;
    }
    std::cout << "0. Cancel\n";
    std::cout << "Enter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 0) {
        return -1;
    }

    if (choice < 1 || choice > static_cast<int>(lecturers.size())) {
        std::cout << "Invalid choice." << std::endl;
        return -1;
    }

    return choice - 1;
}