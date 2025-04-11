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
#include "Utils.h"
#include <iostream>
#include <limits>
#include <stdexcept>

 // Constructor
Admin::Admin(const std::string& username, const std::string& password)
    : User(username, password, UserType::ADMIN), UserInterface() {
    // Nothing additional to initialize for now
}

// Destructor
Admin::~Admin() {
    // Nothing to clean up for now
}

// Display the admin menu (implementation of User::displayMenu)
void Admin::displayMenu() const {
    displayMainMenu();
}

// Display the main menu (implementation of UserInterface::displayMainMenu)
void Admin::displayMainMenu() const {
    displayHeader("ADMIN MENU");

    std::cout << "1. Module Management" << std::endl;
    std::cout << "2. Group Management" << std::endl;
    std::cout << "3. Session Type Management" << std::endl;
    std::cout << "4. Student Management" << std::endl;
    std::cout << "5. Lecturer Management" << std::endl;
    std::cout << "6. Room Management" << std::endl;
    std::cout << "7. Timetable Management" << std::endl;
    std::cout << "8. Find Timetable Conflicts" << std::endl;
    std::cout << "0. Logout" << std::endl;

    displayFooter();
}

// Process menu choice (implementation of UserInterface::processMenuChoice)
bool Admin::processMenuChoice(int choice) {
    switch (choice) {
    case 0: // Logout
        displayInfo("Logging out...");
        return false;

    case 1: { // Module Management
        bool moduleMenu = true;
        while (moduleMenu) {
            clearScreen();
            displayModuleMenu();

            std::cout << "Enter your choice: ";
            int moduleChoice = Utils::getValidInteger(0, 4);

            moduleMenu = processModuleMenuChoice(moduleChoice);
        }
        break;
    }

    case 2: { // Group Management
        bool groupMenu = true;
        while (groupMenu) {
            clearScreen();
            displayGroupMenu();

            std::cout << "Enter your choice: ";
            int groupChoice = Utils::getValidInteger(0, 4);

            groupMenu = processGroupMenuChoice(groupChoice);
        }
        break;
    }

    case 3: { // Session Type Management
        bool sessionTypeMenu = true;
        while (sessionTypeMenu) {
            clearScreen();
            displaySessionTypeMenu();

            std::cout << "Enter your choice: ";
            int sessionTypeChoice = Utils::getValidInteger(0, 4);

            sessionTypeMenu = processSessionTypeMenuChoice(sessionTypeChoice);
        }
        break;
    }

    case 4: { // Student Management
        bool studentMenu = true;
        while (studentMenu) {
            clearScreen();
            displayStudentMenu();

            std::cout << "Enter your choice: ";
            int studentChoice = Utils::getValidInteger(0, 3);

            studentMenu = processStudentMenuChoice(studentChoice);
        }
        break;
    }

    case 5: { // Lecturer Management
        bool lecturerMenu = true;
        while (lecturerMenu) {
            clearScreen();
            displayLecturerMenu();

            std::cout << "Enter your choice: ";
            int lecturerChoice = Utils::getValidInteger(0, 4);

            lecturerMenu = processLecturerMenuChoice(lecturerChoice);
        }
        break;
    }

    case 6: { // Room Management
        bool roomMenu = true;
        while (roomMenu) {
            clearScreen();
            displayRoomMenu();

            std::cout << "Enter your choice: ";
            int roomChoice = Utils::getValidInteger(0, 4);

            roomMenu = processRoomMenuChoice(roomChoice);
        }
        break;
    }

    case 7: { // Timetable Management
        bool timetableMenu = true;
        while (timetableMenu) {
            clearScreen();
            displayTimetableMenu();

            std::cout << "Enter your choice: ";
            int timetableChoice = Utils::getValidInteger(0, 3);

            timetableMenu = processTimetableMenuChoice(timetableChoice);
        }
        break;
    }

    case 8: // Find Timetable Conflicts
        findTimetableConflicts();
        break;

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Continue running
}

// Run the admin interface
bool Admin::run() {
    bool running = true;

    while (running) {
        clearScreen();
        displayMainMenu();

        std::cout << "Enter your choice: ";
        int choice = Utils::getValidInteger(0, 8); // Adjust the upper limit based on menu options

        // Process the choice
        running = processMenuChoice(choice);
    }

    return true; // Normal logout
}

// Display module management menu
void Admin::displayModuleMenu() const {
    displayHeader("MODULE MANAGEMENT");

    std::cout << "1. Add Module" << std::endl;
    std::cout << "2. Edit Module" << std::endl;
    std::cout << "3. Delete Module" << std::endl;
    std::cout << "4. List Modules" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;

    displayFooter();
}

// Process module menu choice
bool Admin::processModuleMenuChoice(int choice) {
    switch (choice) {
    case 0: // Back to Main Menu
        return false;

    case 1: // Add Module
        addModule();
        break;

    case 2: // Edit Module
        editModule();
        break;

    case 3: // Delete Module
        deleteModule();
        break;

    case 4: // List Modules
        listModules();
        waitForContinue();
        break;

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Continue in module menu
}

// Display group management menu
void Admin::displayGroupMenu() const {
    displayHeader("GROUP MANAGEMENT");

    std::cout << "1. Add Group" << std::endl;
    std::cout << "2. Edit Group" << std::endl;
    std::cout << "3. Delete Group" << std::endl;
    std::cout << "4. List Groups" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;

    displayFooter();
}

// Process group menu choice
bool Admin::processGroupMenuChoice(int choice) {
    switch (choice) {
    case 0: // Back to Main Menu
        return false;

    case 1: // Add Group
        addGroup();
        break;

    case 2: // Edit Group
        editGroup();
        break;

    case 3: // Delete Group
        deleteGroup();
        break;

    case 4: // List Groups
        listGroups();
        waitForContinue();
        break;

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Continue in group menu
}

// Display session type management menu
void Admin::displaySessionTypeMenu() const {
    displayHeader("SESSION TYPE MANAGEMENT");

    std::cout << "1. Add Session Type" << std::endl;
    std::cout << "2. Edit Session Type" << std::endl;
    std::cout << "3. Delete Session Type" << std::endl;
    std::cout << "4. List Session Types" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;

    displayFooter();
}

// Process session type menu choice
bool Admin::processSessionTypeMenuChoice(int choice) {
    switch (choice) {
    case 0: // Back to Main Menu
        return false;

    case 1: // Add Session Type
        addSessionType();
        break;

    case 2: // Edit Session Type
        editSessionType();
        break;

    case 3: // Delete Session Type
        deleteSessionType();
        break;

    case 4: // List Session Types
        listSessionTypes();
        waitForContinue();
        break;

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Continue in session type menu
}

// Display student management menu
void Admin::displayStudentMenu() const {
    displayHeader("STUDENT MANAGEMENT");

    std::cout << "1. Register Student" << std::endl;
    std::cout << "2. Assign Student to Group" << std::endl;
    std::cout << "3. List Students" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;

    displayFooter();
}

// Process student menu choice
bool Admin::processStudentMenuChoice(int choice) {
    switch (choice) {
    case 0: // Back to Main Menu
        return false;

    case 1: // Register Student
        registerStudent();
        break;

    case 2: // Assign Student to Group
        assignStudentToGroup();
        break;

    case 3: // List Students
        listStudents();
        waitForContinue();
        break;

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Continue in student menu
}

// Display lecturer management menu
void Admin::displayLecturerMenu() const {
    displayHeader("LECTURER MANAGEMENT");

    std::cout << "1. Register Lecturer" << std::endl;
    std::cout << "2. Assign Lecturer to Module" << std::endl;
    std::cout << "3. Assign Lecturer to Session" << std::endl;
    std::cout << "4. List Lecturers" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;

    displayFooter();
}

// Process lecturer menu choice
bool Admin::processLecturerMenuChoice(int choice) {
    switch (choice) {
    case 0: // Back to Main Menu
        return false;

    case 1: // Register Lecturer
        registerLecturer();
        break;

    case 2: // Assign Lecturer to Module
        assignLecturerToModule();
        break;

    case 3: // Assign Lecturer to Session
        assignLecturerToSession();
        break;

    case 4: // List Lecturers
        listLecturers();
        waitForContinue();
        break;

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Continue in lecturer menu
}

// Display room management menu
void Admin::displayRoomMenu() const {
    displayHeader("ROOM MANAGEMENT");

    std::cout << "1. Add Room" << std::endl;
    std::cout << "2. Edit Room" << std::endl;
    std::cout << "3. Delete Room" << std::endl;
    std::cout << "4. List Rooms" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;

    displayFooter();
}

// Process room menu choice
bool Admin::processRoomMenuChoice(int choice) {
    switch (choice) {
    case 0: // Back to Main Menu
        return false;

    case 1: // Add Room
        addRoom();
        break;

    case 2: // Edit Room
        editRoom();
        break;

    case 3: // Delete Room
        deleteRoom();
        break;

    case 4: // List Rooms
        listRooms();
        waitForContinue();
        break;

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Continue in room menu
}

// Display timetable management menu
void Admin::displayTimetableMenu() const {
    displayHeader("TIMETABLE MANAGEMENT");

    std::cout << "1. Create Timetable Entry" << std::endl;
    std::cout << "2. Update Timetable Entry" << std::endl;
    std::cout << "3. Delete Timetable Entry" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;

    displayFooter();
}

// Process timetable menu choice
bool Admin::processTimetableMenuChoice(int choice) {
    switch (choice) {
    case 0: // Back to Main Menu
        return false;

    case 1: // Create Timetable Entry
        createTimetableEntry();
        break;

    case 2: // Update Timetable Entry
        updateTimetableEntry();
        break;

    case 3: // Delete Timetable Entry
        deleteTimetableEntry();
        break;

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Continue in timetable menu
}

// Module Management
void Admin::addModule() {
    displayHeader("ADD MODULE");

    // Get module details from user
    std::string moduleCode;
    std::string moduleName;

    std::cout << "Enter module code: ";
    std::cin >> moduleCode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter module name: ";
    std::getline(std::cin, moduleName);

    try {
        // Get the timetable manager singleton
        TimetableManager& manager = TimetableManager::getInstance();

        // Check if the module already exists
        if (manager.moduleExists(moduleCode)) {
            displayError("A module with this code already exists.");
            waitForContinue();
            return;
        }

        // Create and add the module
        Module newModule(moduleCode, moduleName);
        manager.addModule(newModule);

        displaySuccess("Module added successfully!");
        waitForContinue();
    }
    catch (const std::exception& e) {
        displayError("Error adding module: " + std::string(e.what()));
        waitForContinue();
    }
}

void Admin::editModule() {
    displayHeader("EDIT MODULE");

    // List modules and get selection
    listModules();
    int selection = getModuleSelection();

    if (selection == -1) {
        displayInfo("Module editing canceled.");
        waitForContinue();
        return;
    }

    try {
        // Get the timetable manager singleton
        TimetableManager& manager = TimetableManager::getInstance();

        // Get the module to edit
        Module& module = manager.getModuleAt(selection);

        // Get updated details
        std::string newName;

        std::cout << "Current module name: " << module.getName() << std::endl;
        std::cout << "Enter new module name (or press Enter to keep current): ";
        std::getline(std::cin, newName);

        if (!newName.empty()) {
            module.setName(newName);
        }

        displaySuccess("Module updated successfully!");
        waitForContinue();
    }
    catch (const std::exception& e) {
        displayError("Error editing module: " + std::string(e.what()));
        waitForContinue();
    }
}

void Admin::deleteModule() {
    displayHeader("DELETE MODULE");

    // List modules and get selection
    listModules();
    int selection = getModuleSelection();

    if (selection == -1) {
        displayInfo("Module deletion canceled.");
        waitForContinue();
        return;
    }

    try {
        // Get the timetable manager singleton
        TimetableManager& manager = TimetableManager::getInstance();

        // Confirm deletion
        std::string confirmCode = manager.getModuleAt(selection).getCode();
        std::string confirmation;

        std::cout << "Are you sure you want to delete module '" << confirmCode << "'? (y/n): ";
        std::cin >> confirmation;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (confirmation == "y" || confirmation == "Y") {
            manager.removeModule(selection);
            displaySuccess("Module deleted successfully!");
        }
        else {
            displayInfo("Module deletion canceled.");
        }

        waitForContinue();
    }
    catch (const std::exception& e) {
        displayError("Error deleting module: " + std::string(e.what()));
        waitForContinue();
    }
}

void Admin::listModules() const {
    displayHeader("MODULES LIST");

    try {
        // Get the timetable manager singleton
        const TimetableManager& manager = TimetableManager::getInstance();

        // Get and display the modules
        const std::vector<Module>& modules = manager.getModules();

        if (modules.empty()) {
            displayInfo("No modules found.");
            return;
        }

        for (size_t i = 0; i < modules.size(); ++i) {
            std::cout << i + 1 << ". " << modules[i].getCode() << " - " << modules[i].getName() << std::endl;
        }
    }
    catch (const std::exception& e) {
        displayError("Error listing modules: " + std::string(e.what()));
    }
}

// Helper function to get a valid module selection
int Admin::getModuleSelection() const {
    try {
        // Get the timetable manager singleton
        const TimetableManager& manager = TimetableManager::getInstance();

        // Get the modules
        const std::vector<Module>& modules = manager.getModules();

        if (modules.empty()) {
            displayInfo("No modules available for selection.");
            return -1;
        }

        std::cout << "Enter the number of the module (1-" << modules.size() << ", 0 to cancel): ";
        int selection = Utils::getValidInteger(0, static_cast<int>(modules.size()));

        if (selection == 0) {
            return -1; // User canceled
        }

        return selection - 1; // Convert to zero-based index
    }
    catch (const std::exception& e) {
        displayError("Error getting module selection: " + std::string(e.what()));
        return -1;
    }
}

// Group Management
void Admin::addGroup() {
    displayHeader("ADD GROUP");

    // Get group details from user
    std::string groupCode;
    std::string groupName;

    std::cout << "Enter group code (e.g., A, B, C): ";
    std::cin >> groupCode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter group name (e.g., Computer Science Year 1): ";
    std::getline(std::cin, groupName);

    try {
        // Get the timetable manager singleton
        TimetableManager& manager = TimetableManager::getInstance();

        // Check if the group already exists
        if (manager.groupExists(groupCode)) {
            displayError("A group with this code already exists.");
            waitForContinue();
            return;
        }

        // Create and add the group
        Group newGroup(groupCode, groupName);
        manager.addGroup(newGroup);

        displaySuccess("Group added successfully!");
        waitForContinue();
    }
    catch (const std::exception& e) {
        displayError("Error adding group: " + std::string(e.what()));
        waitForContinue();
    }
}

void Admin::editGroup() {
    displayHeader("EDIT GROUP");

    // List groups and get selection
    listGroups();
    int selection = getGroupSelection();

    if (selection == -1) {
        displayInfo("Group editing canceled.");
        waitForContinue();
        return;
    }

    try {
        // Get the timetable manager singleton
        TimetableManager& manager = TimetableManager::getInstance();

        // Get the group to edit
        Group& group = manager.getGroupAt(selection);

        // Get updated details
        std::string newName;

        std::cout << "Current group name: " << group.getName() << std::endl;
        std::cout << "Enter new group name (or press Enter to keep current): ";
        std::getline(std::cin, newName);

        if (!newName.empty()) {
            group.setName(newName);
        }

        displaySuccess("Group updated successfully!");
        waitForContinue();
    }
    catch (const std::exception& e) {
        displayError("Error editing group: " + std::string(e.what()));
        waitForContinue();
    }
}

void Admin::deleteGroup() {
    displayHeader("DELETE GROUP");

    // List groups and get selection
    listGroups();
    int selection = getGroupSelection();

    if (selection == -1) {
        displayInfo("Group deletion canceled.");
        waitForContinue();
        return;
    }

    try {
        // Get the timetable manager singleton
        TimetableManager& manager = TimetableManager::getInstance();

        // Confirm deletion
        std::string confirmCode = manager.getGroupAt(selection).getCode();
        std::string confirmation;

        std::cout << "Are you sure you want to delete group '" << confirmCode << "'? (y/n): ";
        std::cin >> confirmation;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (confirmation == "y" || confirmation == "Y") {
            manager.removeGroup(selection);
            displaySuccess("Group deleted successfully!");
        }
        else {
            displayInfo("Group deletion canceled.");
        }

        waitForContinue();
    }
    catch (const std::exception& e) {
        displayError("Error deleting group: " + std::string(e.what()));
        waitForContinue();
    }
}

void Admin::listGroups() const {
    displayHeader("GROUPS LIST");

    try {
        // Get the timetable manager singleton
        const TimetableManager& manager = TimetableManager::getInstance();

        // Get and display the groups
        const std::vector<Group>& groups = manager.getGroups();

        if (groups.empty()) {
            displayInfo("No groups found.");
            return;
        }

        for (size_t i = 0; i < groups.size(); ++i) {
            std::cout << i + 1 << ". " << groups[i].getCode() << " - " << groups[i].getName() << std::endl;
        }
    }
    catch (const std::exception& e) {
        displayError("Error listing groups: " + std::string(e.what()));
    }
}

// Helper function to get a valid group selection
int Admin::getGroupSelection() const {
    try {
        // Get the timetable manager singleton
        const TimetableManager& manager = TimetableManager::getInstance();

        // Get the groups
        const std::vector<Group>& groups = manager.getGroups();

        if (groups.empty()) {
            displayInfo("No groups available for selection.");
            return -1;
        }

        std::cout << "Enter the number of the group (1-" << groups.size() << ", 0 to cancel): ";
        int selection = Utils::getValidInteger(0, static_cast<int>(groups.size()));

        if (selection == 0) {
            return -1; // User canceled
        }

        return selection - 1; // Convert to zero-based index
    }
    catch (const std::exception& e) {
        displayError("Error getting group selection: " + std::string(e.what()));
        return -1;
    }
}