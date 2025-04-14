/**
 * This file contains the declaration of the Admin class, which represents users with
 * administrative privileges in the timetabling system.
 *
 * Admins can manage all core components of the system, including
 * modules, student groups, sessions, lecturers, rooms, and timetables.
 *
 * The class also handles user interaction through menu interfaces
 * specific to administrative tasks.
 */
#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "UserInterface.h"
#include "Room.h"
#include <vector>
#include <memory>
#include <string>

 // Forward declarations to avoid circular dependencies
class Module;
class Group;
class SessionType;
class Room;
class Session;
class Timetable;
class TimeSlot;

/**
 * The Admin class provides functionality for managing the entire
 * timetabling system, including modules, groups, sessions, etc.
 */
class Admin : public User, public UserInterface {
public:
    // Constructor
    Admin(const std::string& username, const std::string& password);

    // Cleans up the admin object (if needed)
    virtual ~Admin();

    // Shows the admin-specific menu (from the User base class)
    void displayMenu() const override;

    // Displays the main menu for admin actions (from UserInterface)
    void displayMainMenu() const override;

    // Handles the user's menu selection and returns whether to keep going
    bool processMenuChoice(int choice) override;

    // Entry point for running the admin interface
    bool run() override;

    // Module management
    void addModule();
    void editModule();
    void deleteModule();
    void listModules() const;

    // Group management
    void addGroup();
    void editGroup();
    void deleteGroup();
    void listGroups() const;

    // Session type management
    void addSessionType();
    void editSessionType();
    void deleteSessionType();
    void listSessionTypes() const;

    // Student management
    void registerStudent();
    void assignStudentToGroup();
    void listStudents() const;

    // Lecturer management
    void registerLecturer();
    void assignLecturerToModule();
    void assignLecturerToSession();
    void listLecturers() const;

    // Room management
    void addRoom();
    void editRoom();
    void deleteRoom();
    void listRooms() const;

    // Timetable management
    void createTimetableEntry();
    void updateTimetableEntry();
    void deleteTimetableEntry();
    void findTimetableConflicts(); // This is an advanced feature to identify scheduling conflicts like double-booked rooms or lecturers.

private:
    // Helper functions for interface navigation
    void displayModuleMenu() const;
    bool processModuleMenuChoice(int choice); // this function returns true If processing should continue
    // or it returns false If the admin wants to go back to the main menu
    void displayGroupMenu() const; // Displays group management menu options
    bool processGroupMenuChoice(int choice);
    void displaySessionTypeMenu() const;
    bool processSessionTypeMenuChoice(int choice);
    void displayStudentMenu() const;
    bool processStudentMenuChoice(int choice);
    void displayLecturerMenu() const;
    bool processLecturerMenuChoice(int choice);
    void displayRoomMenu() const;
    bool processRoomMenuChoice(int choice);
    void displayTimetableMenu() const;
    bool processTimetableMenuChoice(int choice);

    // Selection helpers
    // Returns the selected module index or -1 if the user cancels
    int getModuleSelection() const;
    int getGroupSelection() const;
    int getSessionTypeSelection() const;
    int getRoomSelection() const;
    int getStudentSelection() const;
    int getLecturerSelection() const;

    // Helper functions to convert between string and enum
    Room::RoomType stringToRoomType(const std::string& typeStr) const;
    std::string roomTypeToString(Room::RoomType type) const;
};

#endif // ADMIN_H