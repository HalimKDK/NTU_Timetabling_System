/**
 *
 * This file defines the Group class, which represents a group of students
 * in the timetabling system. Examples of groups include "Group A" or
 * "Computer Science Year 1". The class provides functionality to manage
 * group details and handle the list of students in the group.
 */

#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <vector>
#include <memory>

 // Forward declaration of the Student class
class Student;

/**
 * Represents a student group, such as "Group A" or "Computer Science Year 1".
 */
class Group {
public:

   // Creates a new Group object.
    Group(const std::string& code, const std::string& name);

    
    ~Group(); // Destroys the Group object

    // Gets the group code.
    const std::string& getCode() const;

   // Updates the group code.
    void setCode(const std::string& code);

    // Returns the group name.
    const std::string& getName() const;

   // Updates the group name.
    void setName(const std::string& name);

    // Adds a student to the group.
    void addStudent(std::shared_ptr<Student> student);

    /**
     * Removes a student from the group based on their ID.
     * Returns true if the student was found and removed, otherwise false.
     */
    bool removeStudent(const std::string& studentId);

   // Returns a list of all students currently in the group.
    const std::vector<std::shared_ptr<Student>>& getStudents() const;

    // Checks if a student with the given ID is in the group.
    bool hasStudent(const std::string& studentId) const;


    // Returns the total number of students in the group.
     size_t getStudentCount() const;

private:
    std::string m_code;   // Short code for the group (e.g., "A", "B")
    std::string m_name;   // Full descriptive name of the group
    std::vector<std::shared_ptr<Student>> m_students; // List of students in this group
};

#endif // GROUP_H

