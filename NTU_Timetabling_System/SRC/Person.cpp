/**
 *
 * This file provides the implementation for the Person class, which is the base class
 * for all people in the timetabling system, such as students and lecturers. It handles
 * common data like names and IDs.
 */

#include "Person.h"

 // Creates a new person with a first name, last name, and optional ID
Person::Person(const std::string& firstName, const std::string& lastName, const std::string& id)
    : m_firstName(firstName), m_lastName(lastName), m_id(id) {
    // Nothing else to initialize here
}

// Virtual destructor — ensures proper cleanup for derived classes
Person::~Person() {
    // No dynamic resources to clean up
}

// Returns the person's first name
const std::string& Person::getFirstName() const {
    return m_firstName;
}

// Updates the person's first name
void Person::setFirstName(const std::string& firstName) {
    m_firstName = firstName;
}

// Returns the person's last name
const std::string& Person::getLastName() const {
    return m_lastName;
}

// Updates the person's last name
void Person::setLastName(const std::string& lastName) {
    m_lastName = lastName;
}

// Returns the person's full name (first and last combined)
std::string Person::getFullName() const {
    return m_firstName + " " + m_lastName;
}

// Returns the person's unique ID
const std::string& Person::getId() const {
    return m_id;
}

// Updates the person's ID
void Person::setId(const std::string& id) {
    m_id = id;
}
