// NTU_Timetabling_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/**
 *
 * This file contains the main function and application flow
 * for the NTU timetabling system.
 */

#include "TimetableManager.h"
#include "Admin.h"
#include "Student.h"
#include "Utils.h"
#include "Module.h"
#include "Group.h"
#include "Room.h"
#include "Lecturer.h"
#include "TimeSlot.h"
#include <iostream>
#include <memory>
#include <string>
#include <limits>

 /**
  * Display of the login screen
  */
void displayLoginScreen() {
    std::cout << "===============================================" << std::endl;
    std::cout << "  NOTTINGHAM TRENT UNIVERSITY TIMETABLE SYSTEM  " << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << std::endl;
}

/**
 *
 * std::shared_ptr<User> Pointer to the authenticated user, or nullptr if authentication failed
 */
std::shared_ptr<User> authenticateUser() {
    std::string username, password;
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        std::cout << "Username: ";
        std::cin >> username;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Password: ";
        std::cin >> password;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Try to authenticate
        auto user = TimetableManager::getInstance().authenticateUser(username, password);

        if (user) {
            return user;
        }

        attempts++;
        std::cout << "Invalid username or password. "
            << (attempts < maxAttempts ? "Please try again." : "Too many failed attempts.")
            << std::endl;
    }

    return nullptr;
}

/**
 *  I Initialized the system with sample data for testing
 */
void initializeWithSampleData() {
    TimetableManager& manager = TimetableManager::getInstance();

    // Initialize with current academic year
    manager.initialize("2024-2025");

    // Add sample modules
    manager.addModule(Module("SOFT20091", "Software Design and Implementation"));
    manager.addModule(Module("COMP20011", "Computer Systems Architecture"));
    manager.addModule(Module("DATA20042", "Database Design & Implementation"));

    // Add sample groups
    manager.addGroup(Group("A", "Group A - Computer Science Year 2"));
    manager.addGroup(Group("B", "Group B - Computer Science Year 2"));

    // Add sample rooms
    manager.addRoom(Room("MAE 202", "Maudsley Building Room 202", 30, Room::RoomType::LAB, "Maudsley"));
    manager.addRoom(Room("JCLT 006", "John Clare Lecture Theatre 006", 150, Room::RoomType::LECTURE_HALL, "Mary Ann Evans"));

    // Add sample lecturers
    auto drSmith = std::make_shared<Lecturer>("John", "Smith", "L001", "Dr", "Computer Science");
    auto profJones = std::make_shared<Lecturer>("Sarah", "Jones", "L002", "Prof", "Computer Science");
    manager.addLecturer(drSmith);
    manager.addLecturer(profJones);

    // Assign lecturers to modules
    manager.assignLecturerToModule("L001", "SOFT20091");
    manager.assignLecturerToModule("L002", "COMP20011");
    manager.assignLecturerToModule("L001", "DATA20042");

    // Add sample students
    auto alice = std::make_shared<Student>("alice", "pass123", "Alice", "Johnson", "N0123456");
    auto bob = std::make_shared<Student>("bob", "pass456", "Bob", "Smith", "N0654321");
    manager.addStudent(alice);
    manager.addStudent(bob);

    // Assign students to groups
    manager.assignStudentToGroup("N0123456", "A");
    manager.assignStudentToGroup("N0654321", "B");

    // Add admin user - now we can properly add the admin user
    auto adminUser = std::make_shared<Admin>("admin", "admin123");
    manager.addAdmin(adminUser);

    // Create some sample sessions
    TimeSlot mondayMorning(TimeSlot::Day::MONDAY, 9, 0, 11, 0);
    TimeSlot tuesdayAfternoon(TimeSlot::Day::TUESDAY, 14, 0, 16, 0);
    TimeSlot wednesdayMorning(TimeSlot::Day::WEDNESDAY, 10, 0, 12, 0);

    try {
        // Create sessions for week 1
        manager.createSession("SOFT20091", "LEC", "JCLT 006", "L001", mondayMorning, 1, { "A", "B" });
        manager.createSession("COMP20011", "LEC", "JCLT 006", "L002", tuesdayAfternoon, 1, { "A", "B" });
        manager.createSession("DATA20042", "LEC", "JCLT 006", "L001", wednesdayMorning, 1, { "A", "B" });

        // Create lab sessions for week 1
        TimeSlot thursdayMorningA(TimeSlot::Day::THURSDAY, 9, 0, 11, 0);
        TimeSlot thursdayMorningB(TimeSlot::Day::THURSDAY, 11, 0, 13, 0);

        manager.createSession("SOFT20091", "LAB", "MAE 202", "L001", thursdayMorningA, 1, { "A" });
        manager.createSession("SOFT20091", "LAB", "MAE 202", "L001", thursdayMorningB, 1, { "B" });
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating sample sessions: " << e.what() << std::endl;
    }

    // Save the data
    manager.saveData();
}

// This is the main function
int main() {
    // Display welcome message
    std::cout << "Starting NTU Timetabling System..." << std::endl;

    // Initialize the system
    TimetableManager& manager = TimetableManager::getInstance();

    // Try to load existing data
    bool dataLoaded = manager.loadData();

    // If no data exists, initialize with sample data
    if (!dataLoaded) {
        std::cout << "No existing data found. Initializing with sample data..." << std::endl;
        initializeWithSampleData();
    }

    // Main application loop
    bool running = true;

    while (running) {
        // Clear the screen (platform-dependent)
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        // Display login screen
        displayLoginScreen();

        // Authenticate user
        std::shared_ptr<User> currentUser = authenticateUser();

        if (currentUser) {
            // User authenticated, check type and run appropriate interface
            if (currentUser->getUserType() == User::UserType::ADMIN) {
                // Cast to Admin and run admin interface
                auto admin = std::dynamic_pointer_cast<Admin>(currentUser);
                if (admin) {
                    admin->run();
                }
            }
            else if (currentUser->getUserType() == User::UserType::STUDENT) {
                // Cast to Student and run student interface
                auto student = std::dynamic_pointer_cast<Student>(currentUser);
                if (student) {
                    student->run();
                }
            }
        }
        else {
            // Authentication failed
            std::cout << "Login failed. ";

            // Ask if the user wants to try again
            std::cout << "Do you want to try again? (y/n): ";
            char response;
            std::cin >> response;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (response != 'y' && response != 'Y') {
                running = false;
            }
        }
    }

    // Save data before exiting
    manager.saveData();

    std::cout << "Thank you for using the NTU Timetabling System. Goodbye!" << std::endl;

    return 0;
}