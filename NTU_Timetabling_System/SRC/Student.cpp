/**
 *
 * This file Implements the Student class, which represents a student user in the system.
 * This class provides functionality for students to:
 * - View their assigned group's weekly timetable
 * - Search for sessions by module, room, or lecturer
 * - Export timetable data to a CSV file
 *
 * It handles user interaction through menus, input validation,
 * and connects with the TimetableManager to retrieve relevant session data.
 */

#include "Student.h"
#include "Group.h"
#include "TimetableManager.h"
#include "Session.h"
#include "Utils.h"
#include "CSV_Exporter.h"
#include "Module.h"
#include "Room.h"
#include "Lecturer.h"
#include "SessionType.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>

 // Minimal constructor for when only login details are provided (needed by STL)
Student::Student(const std::string& username, const std::string& password)
    : User(username, password, UserType::STUDENT),
    Person("", ""),
    UserInterface(),
    m_studentId(""),
    m_assignedGroup(nullptr) {
    // No extra setup needed here
}

// Full constructor initializing student details
Student::Student(const std::string& username, const std::string& password,
    const std::string& firstName, const std::string& lastName,
    const std::string& studentId)
    : User(username, password, UserType::STUDENT),
    Person(firstName, lastName),
    UserInterface(),
    m_studentId(studentId),
    m_assignedGroup(nullptr) {
    // No extra setup needed here
}

// Destructor — nothing special to clean up here
Student::~Student() {
    // No need to delete m_assignedGroup as we don't own it
}

// Displays the main student menu
void Student::displayMenu() const {
    displayMainMenu();
}

// Shows the student main menu screen
void Student::displayMainMenu() const {
    displayHeader("STUDENT MENU");

    std::cout << "Welcome, " << getFirstName() << " " << getLastName() << std::endl;

    if (m_assignedGroup) {
        std::cout << "Group: " << m_assignedGroup->getName()
            << " (" << m_assignedGroup->getCode() << ")" << std::endl;
    }

    std::cout << std::endl;
    std::cout << "1. View Timetable" << std::endl;
    std::cout << "2. Search Timetable" << std::endl;
    std::cout << "3. Export Timetable to CSV" << std::endl;
    std::cout << "0. Logout" << std::endl;

    displayFooter();
}

// Handles user choices from the student menu
bool Student::processMenuChoice(int choice) {
    switch (choice) {
    case 0: // Logout
        displayInfo("Logging out...");
        return false;

    case 1: { // View timetable for a specific week
        displayHeader("VIEW TIMETABLE");
        std::cout << "Enter week number (1-53): ";
        int weekNumber = Utils::getValidInteger(1, 53);
        viewTimetable(weekNumber);
        waitForContinue();
        break;
    }

    case 2: { // Search within the timetable
        searchTimetable();
        break;
    }

    case 3: { // Export timetable to a CSV file
        displayHeader("EXPORT TIMETABLE");
        std::cout << "Enter week number (1-53): ";
        int weekNumber = Utils::getValidInteger(1, 53);

        std::string filename;
        std::cout << "Enter filename (without .csv extension): ";
        std::cin >> filename;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        filename += ".csv";

        if (exportTimetable(weekNumber, filename)) {
            displaySuccess("Timetable exported successfully to " + filename);
        }
        else {
            displayError("Failed to export timetable.");
        }
        waitForContinue();
        break;
    }

    default:
        displayError("Invalid choice. Please try again.");
        break;
    }

    return true; // Keep running the menu loop
}

// Starts the student interface loop
bool Student::run() {
    bool running = true;

    while (running) {
        clearScreen();
        displayMainMenu();

        std::cout << "Enter your choice: ";
        int choice = Utils::getValidInteger(0, 3);

        running = processMenuChoice(choice);
    }

    return true; // User logged out successfully
}

// Returns the student's ID
const std::string& Student::getStudentId() const {
    return m_studentId;
}

// Updates the student's ID
void Student::setStudentId(const std::string& studentId) {
    m_studentId = studentId;
}

// Returns the student's password
const std::string& Student::getPassword() const {
    return User::m_password;
}

// Gets the group this student is assigned to
Group* Student::getAssignedGroup() {
    return m_assignedGroup;
}

// Assigns a group to the student
void Student::setAssignedGroup(Group* group) {
    m_assignedGroup = group;
}

// Implementation of searchTimetable method
void Student::searchTimetable() {
    bool searchMenu = true;
    while (searchMenu) {
        clearScreen();
        displaySearchMenu();

        std::cout << "Enter your choice: ";
        int searchChoice = Utils::getValidInteger(0, 3);

        searchMenu = processSearchMenuChoice(searchChoice);
    }
}

// Shows the timetable search options menu
void Student::displaySearchMenu() const {
    displayHeader("SEARCH TIMETABLE");

    std::cout << "Search by:" << std::endl;
    std::cout << "1. Module" << std::endl;
    std::cout << "2. Room" << std::endl;
    std::cout << "3. Lecturer" << std::endl;
    std::cout << "0. Back to Main Menu" << std::endl;

    displayFooter();
}

// Processes the user's search menu selection
bool Student::processSearchMenuChoice(int choice) {
    if (choice == 0) {
        return false; // Go back to main menu
    }

    if (!m_assignedGroup) {
        displayError("You are not assigned to any group yet.");
        waitForContinue();
        return true;
    }

    try {
        TimetableManager& manager = TimetableManager::getInstance();
        std::vector<std::shared_ptr<Session>> sessions;

        switch (choice) {
        case 1: { // Search by module
            displayHeader("SEARCH BY MODULE");
            const std::vector<Module>& modules = manager.getModules();

            if (modules.empty()) {
                displayInfo("No modules available.");
                waitForContinue();
                return true;
            }

            for (size_t i = 0; i < modules.size(); ++i) {
                std::cout << i + 1 << ". " << modules[i].getCode()
                    << " - " << modules[i].getName() << std::endl;
            }

            std::cout << "Enter module number: ";
            int moduleIndex = Utils::getValidInteger(1, static_cast<int>(modules.size())) - 1;
            sessions = manager.getSessionsForModuleAndGroup(modules[moduleIndex].getCode(), m_assignedGroup->getCode());
            displayHeader("SESSIONS FOR MODULE " + modules[moduleIndex].getName());
            break;
        }

        case 2: { // Search by room
            displayHeader("SEARCH BY ROOM");
            const std::vector<Room>& rooms = manager.getRooms();

            if (rooms.empty()) {
                displayInfo("No rooms available.");
                waitForContinue();
                return true;
            }

            for (size_t i = 0; i < rooms.size(); ++i) {
                std::cout << i + 1 << ". " << rooms[i].getNumber()
                    << " - " << rooms[i].getName() << std::endl;
            }

            std::cout << "Enter room number: ";
            int roomIndex = Utils::getValidInteger(1, static_cast<int>(rooms.size())) - 1;
            sessions = manager.getSessionsForRoomAndGroup(rooms[roomIndex].getNumber(), m_assignedGroup->getCode());
            displayHeader("SESSIONS IN ROOM " + rooms[roomIndex].getName());
            break;
        }

        case 3: { // Search by lecturer
            displayHeader("SEARCH BY LECTURER");
            const std::vector<std::shared_ptr<Lecturer>>& lecturers = manager.getLecturers();

            if (lecturers.empty()) {
                displayInfo("No lecturers available.");
                waitForContinue();
                return true;
            }

            for (size_t i = 0; i < lecturers.size(); ++i) {
                std::cout << i + 1 << ". " << lecturers[i]->getFirstName()
                    << " " << lecturers[i]->getLastName() << std::endl;
            }

            std::cout << "Enter lecturer number: ";
            int lecturerIndex = Utils::getValidInteger(1, static_cast<int>(lecturers.size())) - 1;
            sessions = manager.getSessionsForLecturerAndGroup(lecturers[lecturerIndex]->getId(), m_assignedGroup->getCode());
            displayHeader("SESSIONS WITH " + lecturers[lecturerIndex]->getFullName());
            break;
        }
        }

        if (sessions.empty()) {
            displayInfo("No matching sessions found.");
        }
        else {
            displaySessions(sessions);
        }

        waitForContinue();
    }
    catch (const std::exception& e) {
        displayError("Error searching timetable: " + std::string(e.what()));
        waitForContinue();
    }

    return true;
}

// Shows the timetable for a specific week
void Student::viewTimetable(int weekNumber) {
    try {
        if (!m_assignedGroup) {
            displayError("You are not assigned to any group yet.");
            return;
        }

        TimetableManager& manager = TimetableManager::getInstance();
        std::vector<std::shared_ptr<Session>> sessions = manager.getSessionsForGroup(m_assignedGroup->getCode(), weekNumber);

        displayHeader("TIMETABLE FOR WEEK " + std::to_string(weekNumber) + " - GROUP " + m_assignedGroup->getCode());

        if (sessions.empty()) {
            displayInfo("No sessions scheduled for this week.");
        }
        else {
            displaySessions(sessions);
        }
    }
    catch (const std::exception& e) {
        displayError("Error viewing timetable: " + std::string(e.what()));
    }
}

// Exports the timetable to a CSV file
bool Student::exportTimetable(int weekNumber, const std::string& filename) {
    try {
        if (!m_assignedGroup) {
            displayError("You are not assigned to any group yet.");
            return false;
        }

        TimetableManager& manager = TimetableManager::getInstance();
        std::vector<std::shared_ptr<Session>> sessions = manager.getSessionsForGroup(m_assignedGroup->getCode(), weekNumber);

        CSV_Exporter exporter;
        return exporter.exportTimetable(sessions, filename, "Week " + std::to_string(weekNumber), m_assignedGroup->getCode());
    }
    catch (const std::exception& e) {
        displayError("Error exporting timetable: " + std::string(e.what()));
        return false;
    }
}

// Displays a list of sessions in a formatted table
void Student::displaySessions(const std::vector<std::shared_ptr<Session>>& sessions) const {
    std::vector<std::shared_ptr<Session>> sortedSessions = sessions;

    // Sort sessions by day first, then by start time
    std::sort(sortedSessions.begin(), sortedSessions.end(),
        [](const std::shared_ptr<Session>& a, const std::shared_ptr<Session>& b) {
            if (a->getTimeSlot().getDay() != b->getTimeSlot().getDay()) {
                return a->getTimeSlot().getDay() < b->getTimeSlot().getDay();
            }
            return a->getTimeSlot().getStartTime() < b->getTimeSlot().getStartTime();
        });

    // Print the session table header
    std::cout << std::setw(10) << "Day"
        << std::setw(15) << "Time"
        << std::setw(15) << "Module"
        << std::setw(15) << "Type"
        << std::setw(20) << "Room"
        << std::setw(25) << "Lecturer" << std::endl;

    std::cout << std::string(100, '-') << std::endl;

    // Print each session in the table
    for (const auto& session : sortedSessions) {
        std::cout << std::setw(10) << Utils::dayToString(session->getTimeSlot().getDay())
            << std::setw(15) << session->getTimeSlot().getStartTimeString() + "-" + session->getTimeSlot().getEndTimeString()
            << std::setw(15) << session->getModule().getCode()
            << std::setw(15) << session->getSessionType().getName()
            << std::setw(20) << session->getRoom().getName()
            << std::setw(25) << session->getLecturer().getFirstName() + " " + session->getLecturer().getLastName()
            << std::endl;
    }
}