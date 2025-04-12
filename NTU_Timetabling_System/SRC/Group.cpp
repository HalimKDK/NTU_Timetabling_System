/**
 *
 * This file contains the implementation of the Group class, which is used
 * to manage student groups within the timetabling system. It handles
 * operations such as adding, removing, and checking for students in a group.
 */

#include "Group.h"
#include "Student.h"
#include <algorithm>

 // Constructor
Group::Group(const std::string& code, const std::string& name)
    : m_code(code), m_name(name) {
    // Nothing else to initialize
}

// Destructor
Group::~Group() {
    // Automatically cleans up the vector of shared_ptr
}

// Get the group code
const std::string& Group::getCode() const {
    return m_code;
}

// Set the group code
void Group::setCode(const std::string& code) {
    m_code = code;
}

// Get the group name
const std::string& Group::getName() const {
    return m_name;
}

// Set the group name
void Group::setName(const std::string& name) {
    m_name = name;
}

// Add a student to this group
void Group::addStudent(std::shared_ptr<Student> student) {
    // Check if the student is already in the group
    if (!hasStudent(student->getStudentId())) {
        m_students.push_back(student);

        // Also update the student's assigned group
        student->setAssignedGroup(this);
    }
}

// Remove a student from this group
bool Group::removeStudent(const std::string& studentId) {
    auto it = std::find_if(m_students.begin(), m_students.end(),
        [&studentId](const std::shared_ptr<Student>& student) {
            return student->getStudentId() == studentId;
        });

    if (it != m_students.end()) {
        // Update the student's assigned group to nullptr
        (*it)->setAssignedGroup(nullptr);

        // Remove the student from this group
        m_students.erase(it);
        return true;
    }

    return false;
}

// Get the students in this group
const std::vector<std::shared_ptr<Student>>& Group::getStudents() const {
    return m_students;
}

// Check if a student is in this group
bool Group::hasStudent(const std::string& studentId) const {
    return std::find_if(m_students.begin(), m_students.end(),
        [&studentId](const std::shared_ptr<Student>& student) {
            return student->getStudentId() == studentId;
        }) != m_students.end();
}

// Get the number of students in this group
size_t Group::getStudentCount() const {
    return m_students.size();
}