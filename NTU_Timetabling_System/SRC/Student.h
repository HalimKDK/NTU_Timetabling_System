/**
  * Student.h
  * Student user class for the timetabling system
  *
  * This file contains the declaration of the Student class which
  * represents student users who can view and search timetables.
  */

#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include "Person.h"
#include "UserInterface.h"
#include <string>
#include <vector>
#include <memory>

  // Forward declarations
class Group;
class Timetable;
class Session;

/**
 * Student
 * Student user with timetable viewing capabilities
 *
 * The Student class represents a student user who can view and
 * search their timetable based on various criteria.
 */
class Student : public User, public Person, public UserInterface {
public:
    /**
     * Default constructor for STL compatibility
     */
    Student() : User(), Person(), UserInterface(), m_studentId(""), m_assignedGroup(nullptr) {}

    /**
     * Construct a new Student object with minimal information
     *
     * username Student username
     * password Student password
     */
    Student(const std::string& username, const std::string& password);

    /**
     * Construct a new Student object with full details
     *
     * username Student username
     * password Student password
     * firstName Student's first name
     * lastName Student's last name
     * studentId Student's ID number
     */
    Student(const std::string& username, const std::string& password,
        const std::string& firstName, const std::string& lastName,
        const std::string& studentId);

    /**
     * Explicitly declare copy/move constructors and assignment operators
     */
    Student(const Student&) = default;
    Student(Student&&) = default;
    Student& operator=(const Student&) = default;
    Student& operator=(Student&&) = default;

    /**
     * Destroy the Student object
     */
    virtual ~Student();

    /**
     * Display the student menu
     *
     * Implementation of the pure virtual function from User
     */
    void displayMenu() const override;

    /**
     * Display the main menu
     *
     * Implementation of the pure virtual function from UserInterface
     */
    void displayMainMenu() const override;

    /**
     * Process a menu choice
     *
     * Implementation of the pure virtual function from UserInterface
     *
     * choice User's menu choice
     * return true If processing should continue
     * return false If the user wants to exit
     */
    bool processMenuChoice(int choice) override;

    /**
     * Run the student interface
     *
     * Main function to execute the student user interface and handle
     * user input for viewing timetables.
     *
     * return true If the student logs out normally
     * return false If there was an error or forced exit
     */
    bool run() override;

    /**
     * Get the student's ID
     *
     * return The student ID
     */
    const std::string& getStudentId() const;

    /**
     * Set the student's ID
     *
     * studentId The new student ID
     */
    void setStudentId(const std::string& studentId);

    /**
     * Get the student's password
     *
     * return The password
     */
    const std::string& getPassword() const;

    /**
     * Get the student's assigned group
     *
     * return Pointer to the assigned group, or nullptr if not assigned
     */
    Group* getAssignedGroup();

    /**
     * Set the student's assigned group
     *
     * group Pointer to the group to assign
     */
    void setAssignedGroup(Group* group);

    /**
     * View the student's timetable for a specific week
     *
     * weekNumber The week number to view (1-53)
     */
    void viewTimetable(int weekNumber);

    /**
     * Search the timetable by various criteria
     *
     * Advanced feature to search timetable by module, room, lecturer, etc.
     */
    void searchTimetable();

    /**
     * Export the timetable to a CSV file
     *
     * Advanced feature to export the timetable to a CSV file.
     *
     * weekNumber The week number to export
     * filename The filename to export to
     * return true If export was successful
     * return false If there was an error
     */
    bool exportTimetable(int weekNumber, const std::string& filename);

private:
    std::string m_studentId;  // Student's ID number
    Group* m_assignedGroup;   // Pointer to the assigned group

    /**
     * Display search options menu
     */
    void displaySearchMenu() const;

    /**
     * Process search menu selection
     *
     * choice User's menu choice
     * return true If processing should continue
     * return false If the student wants to go back to main menu
     */
    bool processSearchMenuChoice(int choice);

    /**
     * Display the sessions for a specific week
     *
     * sessions Vector of sessions to display
     */
    void displaySessions(const std::vector<std::shared_ptr<Session>>& sessions) const;
};

#endif // STUDENT_H