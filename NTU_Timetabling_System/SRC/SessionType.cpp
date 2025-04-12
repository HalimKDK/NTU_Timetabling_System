/**
 *
 * This file contains the implementation of the SessionType class, which represents
 * different types of academic sessions in the timetabling system, such as lectures,
 * labs, and tutorials. It handles storing and updating details like the session's
 * code, name, and description.
 */

#include "SessionType.h"

 // Constructor — initializes the session type with a code, name, and optional description
SessionType::SessionType(const std::string& code, const std::string& name, const std::string& description)
    : m_code(code), m_name(name), m_description(description) {
    // No additional setup needed here
}

// Destructor — nothing specific to clean up
SessionType::~SessionType() {
    // No dynamic resources to release
}

// Returns the session type's code
const std::string& SessionType::getCode() const {
    return m_code;
}

// Updates the session type's code
void SessionType::setCode(const std::string& code) {
    m_code = code;
}

// Returns the session type's name
const std::string& SessionType::getName() const {
    return m_name;
}

// Updates the session type's name
void SessionType::setName(const std::string& name) {
    m_name = name;
}

// Returns the session type's description
const std::string& SessionType::getDescription() const {
    return m_description;
}

// Updates the session type's description
void SessionType::setDescription(const std::string& description) {
    m_description = description;
}
