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
#include <iostream>
#include <limits>
#include <algorithm>
#include <iomanip>

 // Constructor that sets up all student info, including ID and group assignment
Student::Student(const std::string& username, const std::string& password,
    const std::string& firstName, const std::string& lastName,
    const std::string& studentId)
    : User(username, password, UserType::STUDENT),
    Person(firstName, lastName),
    UserInterface(),
    m_studentId(studentId),
    m_assignedGroup(nullptr) {
    // No additional setup needed
}

// Basic constructor for a student with just login details
Student::Student(const std::string& username, const std::string& password)
    : User(username, password, UserType::STUDENT),
    Person("", ""),
    UserInterface(),
    m_studentId(""),
    m_assignedGroup(nullptr) {
    // No additional setup needed
}

// Clean up the student object
Student::~Student() {
    // No ownership of group pointer, so nothing to delete here
}

// Shows the student-specific menu options
void Student::displayMenu() const {
    displayMainMenu();
}

// Shows the main menu for students with a greeting and options
void Student::displayMainMenu() const {
    displayHeader("STUDENT MENU");

    std::cout << "Welcome, " << getFirstName() << " " << getLastName() << std::endl;

    if (m_assignedGroup) {
        std::cout << "Group: " << m_assignedGroup->getName() << " (" << m_assignedGroup->getCode() << ")" << std::endl;
    }

    std::cout << "\n1. View Timetable\n"
        << "2. Search Timetable\n"
        << "3. Export Timetable to CSV\n"
        << "0. Logout\n";

    displayFooter();
}

// Handles what happens when a student picks an option from the menu
bool Student::processMenuChoice(int choice) {
    switch (choice) {
    case 0:
        displayInfo("Logging out...");
        return false;

    case 1: {
        displayHeader("VIEW TIMETABLE");
        std::cout << "Enter week number (1-53): ";
        int weekNumber = Utils::getValidInteger(1, 53);
        viewTimetable(weekNumber);
        waitForContinue();
        break;
    }

    case 2: {
        bool searchMenu = true;
        while (searchMenu) {
            clearScreen();
            displaySearchMenu();
            std::cout << "Enter your choice: ";
            int searchChoice = Utils::getValidInteger(0, 3);
            searchMenu = processSearchMenuChoice(searchChoice);
        }
        break;
    }

    case 3: {
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

    return true;
}

// Launches the student menu loop
bool Student::run() {
    bool running = true;

    while (running) {
        clearScreen();
        displayMainMenu();

        std::cout << "Enter your choice: ";
        int choice = Utils::getValidInteger(0, 3);
        running = processMenuChoice(choice);
    }

    return true;
}

// Returns the student's ID
const std::string& Student::getStudentId() const {
    return m_studentId;
}

// Updates the student's ID
void Student::setStudentId(const std::string& studentId) {
    m_studentId = studentId;
}

// Gets the group the student is currently in
Group* Student::getAssignedGroup() {
    return m_assignedGroup;
}

// Assigns the student to a group
void Student::setAssignedGroup(Group* group) {
    m_assignedGroup = group;
}

// Displays the menu for searching timetable sessions
void Student::displaySearchMenu() const {
    displayHeader("SEARCH TIMETABLE");

    std::cout << "Search by:\n"
        << "1. Module\n"
        << "2. Room\n"
        << "3. Lecturer\n"
        << "0. Back to Main Menu\n";

    displayFooter();
}

// Handles the logic for searching based on the student's input
bool Student::processSearchMenuChoice(int choice) {
    if (choice == 0) {
        return false;
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
        case 1: {
            displayHeader("SEARCH BY MODULE");
            const auto& modules = manager.getModules();
            if (modules.empty()) {
                displayInfo("No modules available.");
                waitForContinue();
                return true;
            }

            for (size_t i = 0; i < modules.size(); ++i) {
                std::cout << i + 1 << ". " << modules[i].getCode() << " - " << modules[i].getName() << std::endl;
            }

            std::cout << "Enter module number: ";
            int index = Utils::getValidInteger(1, modules.size()) - 1;
            sessions = manager.getSessionsForModuleAndGroup(modules[index].getCode(), m_assignedGroup->getCode());
            displayHeader("SESSIONS FOR MODULE " + modules[index].getName());
            break;
        }

        case 2: {
            displayHeader("SEARCH BY ROOM");
            const auto& rooms = manager.getRooms();
            if (rooms.empty()) {
                displayInfo("No rooms available.");
                waitForContinue();
                return true;
            }

            for (size_t i = 0; i < rooms.size(); ++i) {
                std::cout << i + 1 << ". " << rooms[i].getNumber() << " - " << rooms[i].getName() << std::endl;
            }

            std::cout << "Enter room number: ";
            int index = Utils::getValidInteger(1, rooms.size()) - 1;
            sessions = manager.getSessionsForRoomAndGroup(rooms[index].getNumber(), m_assignedGroup->getCode());
            displayHeader("SESSIONS IN ROOM " + rooms[index].getName());
            break;
        }

        case 3: {
            displayHeader("SEARCH BY LECTURER");
            const auto& lecturers = manager.getLecturers();
            if (lecturers.empty()) {
                displayInfo("No lecturers available.");
                waitForContinue();
                return true;
            }

            for (size_t i = 0; i < lecturers.size(); ++i) {
                std::cout << i + 1 << ". " << lecturers[i]->getFirstName() << " " << lecturers[i]->getLastName() << std::endl;
            }

            std::cout << "Enter lecturer number: ";
            int index = Utils::getValidInteger(1, lecturers.size()) - 1;
            sessions = manager.getSessionsForLecturerAndGroup(lecturers[index]->getId(), m_assignedGroup->getCode());
            displayHeader("SESSIONS WITH " + lecturers[index]->getFullName());
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

// Displays the full weekly timetable for the student's group
void Student::viewTimetable(int weekNumber) {
    try {
        if (!m_assignedGroup) {
            displayError("You are not assigned to any group yet.");
            return;
        }

        TimetableManager& manager = TimetableManager::getInstance();
        auto sessions = manager.getSessionsForGroup(m_assignedGroup->getCode(), weekNumber);
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

// Saves the timetable to a CSV file
bool Student::exportTimetable(int weekNumber, const std::string& filename) {
    try {
        if (!m_assignedGroup) {
            displayError("You are not assigned to any group yet.");
            return false;
        }

        TimetableManager& manager = TimetableManager::getInstance();
        auto sessions = manager.getSessionsForGroup(m_assignedGroup->getCode(), weekNumber);
        CSV_Exporter exporter;

        return exporter.exportTimetable(sessions, filename, "Week " + std::to_string(weekNumber), m_assignedGroup->getCode());
    }
    catch (const std::exception& e) {
        displayError("Error exporting timetable: " + std::string(e.what()));
        return false;
    }
}

// Formats and prints out a list of sessions in a nice table
void Student::displaySessions(const std::vector<std::shared_ptr<Session>>& sessions) const {
    auto sortedSessions = sessions;

    std::sort(sortedSessions.begin(), sortedSessions.end(),
        [](const auto& a, const auto& b) {
            if (a->getTimeSlot().getDay() != b->getTimeSlot().getDay()) {
                return a->getTimeSlot().getDay() < b->getTimeSlot().getDay();
            }
            return a->getTimeSlot().getStartTime() < b->getTimeSlot().getStartTime();
        });

    std::cout << std::setw(10) << "Day"
        << std::setw(15) << "Time"
        << std::setw(15) << "Module"
        << std::setw(15) << "Type"
        << std::setw(20) << "Room"
        << std::setw(25) << "Lecturer" << std::endl;

    std::cout << std::string(100, '-') << std::endl;

    for (const auto& session : sortedSessions) {
        std::cout << std::setw(10) << Utils::dayToString(session->getTimeSlot().getDay())
            << std::setw(15) << session->getTimeSlot().getStartTimeString() + "-" + session->getTimeSlot().getEndTimeString()
            << std::setw(15) << session->getModule().getCode()
            << std::setw(15) << session->getSessionType().getName()
            << std::setw(20) << session->getRoom().getName()
            << std::setw(25) << session->getLecturer().getFullName()
            << std::endl;
    }
}
