/**
 *
 * This file contains the implementation of the Module class, which is part of the
 * timetabling system. It defines the behavior for managing academic modules,
 * including setting and retrieving module details like code and name, and handling
 * the assignment and removal of lecturers associated with each module.
 */

#include "Module.h"
#include "Lecturer.h"
#include <algorithm>

 // Constructor
Module::Module(const std::string& code, const std::string& name)
    : m_code(code), m_name(name) {
    // Nothing else to initialize
}

// Destructor
Module::~Module() {
    // Automatically cleans up the vector of shared_ptr
}

// Get the module code
const std::string& Module::getCode() const {
    return m_code;
}

// Set the module code
void Module::setCode(const std::string& code) {
    m_code = code;
}

// Get the module name
const std::string& Module::getName() const {
    return m_name;
}

// Set the module name
void Module::setName(const std::string& name) {
    m_name = name;
}

// Add a lecturer to this module
void Module::addLecturer(std::shared_ptr<Lecturer> lecturer) {
    // Check if the lecturer is already assigned
    if (!hasLecturer(lecturer->getId())) {
        m_lecturers.push_back(lecturer);
    }
}

// Remove a lecturer from this module
bool Module::removeLecturer(const std::string& lecturerId) {
    auto it = std::find_if(m_lecturers.begin(), m_lecturers.end(),
        [&lecturerId](const std::shared_ptr<Lecturer>& lecturer) {
            return lecturer->getId() == lecturerId;
        });

    if (it != m_lecturers.end()) {
        m_lecturers.erase(it);
        return true;
    }

    return false;
}

// Get the lecturers assigned to this module
const std::vector<std::shared_ptr<Lecturer>>& Module::getLecturers() const {
    return m_lecturers;
}

// Check if a lecturer is assigned to this module
bool Module::hasLecturer(const std::string& lecturerId) const {
    return std::find_if(m_lecturers.begin(), m_lecturers.end(),
        [&lecturerId](const std::shared_ptr<Lecturer>& lecturer) {
            return lecturer->getId() == lecturerId;
        }) != m_lecturers.end();
}