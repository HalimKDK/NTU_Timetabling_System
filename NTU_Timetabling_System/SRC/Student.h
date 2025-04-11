/**
 *
 * This file defines the Student class. A student can log into the system,
 * check their weekly timetable, search for sessions, and even export their
 * schedule as a CSV file.
 */

#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "Person.h"
#include "UserInterface.h"
#include <string>
#include <vector>
#include <memory>

 // Forward declarations to avoid unnecessary includes
class Group;
class Timetable;
class Session;

/**
 * Student represents a user who can log in and access their academic schedule.
 * They can view their timetable, search it by different filters, and export it.
 */
class Student : public User, public Person, public UserInterface {
public:
    // Creates a student with full profile information (useful when loading from a database or form)
    Student(const std::string& username, const std::string& password,
        const std::string& firstName = "", const std::string& lastName = "",
        const std::string& studentId = "");

    // A simpler constructor for when you just have a username and password
    Student(const std::string& username, const std::string& password);

    // Clean up resources when a student object is destroyed
    virtual ~Student();

    // Shows the main student interface menu (overrides the base class version)
    void displayMenu() const override;

    // Displays the actual menu options specific to students
    void displayMainMenu() const override;

    // Handles the user's menu selection and performs the appropriate action
    bool processMenuChoice(int choice) override;

    // Runs the student interface loop – keeps going until the user logs out
    bool run() override;

    // Returns the student's ID number
    const std::string& getStudentId() const;

    // Updates the student's ID number
    void setStudentId(const std::string& studentId);

    // Returns the group the student is assigned to, if any
    Group* getAssignedGroup();

    // Assigns the student to a group
    void setAssignedGroup(Group* group);

    // Displays the student's timetable for a specific week (week number between 1 and 53)
    void viewTimetable(int weekNumber);

    // Lets the student search their timetable using filters like module, room, or lecturer
    void searchTimetable();

    // Exports the timetable to a CSV file, useful for printing or sharing
    bool exportTimetable(int weekNumber, const std::string& filename);

private:
    std::string m_studentId;      // Student's unique ID
    Group* m_assignedGroup;       // Pointer to the group the student belongs to

    // Shows the search filter options (like by room, lecturer, etc.)
    void displaySearchMenu() const;

    // Handles the user's search selection
    bool processSearchMenuChoice(int choice);

    // Helper to show all the sessions in a week
    void displaySessions(const std::vector<std::shared_ptr<Session>>& sessions) const;
};

#endif // STUDENT_H

