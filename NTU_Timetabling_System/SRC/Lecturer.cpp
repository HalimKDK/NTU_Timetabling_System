/**
 *
 * This file implements the Lecturer class methods. A Lecturer can
 * have a title, belong to a department, and be assigned to modules
 * and sessions within the timetabling system.
 */
#include "Lecturer.h"
#include "Module.h"
#include "Session.h"
#include <algorithm>

 // Constructor — creates a Lecturer with name, ID, title, and department
Lecturer::Lecturer(const std::string& firstName, const std::string& lastName,
    const std::string& id, const std::string& title,
    const std::string& department)
    : Person(firstName, lastName, id),
    m_title(title),
    m_department(department) {
    // Nothing else to initialize
}

// Special constructor for STL operations with 7 string arguments
// This constructor ignores the extra parameters but allows the STL to create Lecturer objects
Lecturer::Lecturer(const std::string& firstName, const std::string& lastName,
    const std::string& id, const std::string& title,
    const std::string& department, const std::string& /* extra1 */,
    const std::string& /* extra2 */)
    : Person(firstName, lastName, id),
    m_title(title),
    m_department(department) {
    // Nothing else to initialize
}

// Destructor — no manual cleanup needed as shared_ptr handles memory
Lecturer::~Lecturer() {
    // shared_ptr vectors clean themselves up
}

// Returns the lecturer's title
const std::string& Lecturer::getTitle() const {
    return m_title;
}

// Sets the lecturer's title
void Lecturer::setTitle(const std::string& title) {
    m_title = title;
}

// Returns the lecturer's full name with title (if available)
std::string Lecturer::getFormattedName() const {
    if (m_title.empty()) {
        return getFullName();
    }
    return m_title + " " + getFullName();
}

// Returns the lecturer's department
const std::string& Lecturer::getDepartment() const {
    return m_department;
}

// Sets the lecturer's department
void Lecturer::setDepartment(const std::string& department) {
    m_department = department;
}

// Adds a module to the lecturer's list if it isn't already there
void Lecturer::addModule(std::shared_ptr<Module> module) {
    if (!teachesModule(module->getCode())) {
        m_modules.push_back(module);
    }
}

// Removes a module by code, returns true if successful
bool Lecturer::removeModule(const std::string& moduleCode) {
    auto it = std::find_if(m_modules.begin(), m_modules.end(),
        [&moduleCode](const std::shared_ptr<Module>& module) {
            return module->getCode() == moduleCode;
        });
    if (it != m_modules.end()) {
        m_modules.erase(it);
        return true;
    }
    return false;
}

// Returns the list of modules the lecturer teaches
const std::vector<std::shared_ptr<Module>>& Lecturer::getModules() const {
    return m_modules;
}

// Checks if the lecturer teaches a specific module
bool Lecturer::teachesModule(const std::string& moduleCode) const {
    return std::find_if(m_modules.begin(), m_modules.end(),
        [&moduleCode](const std::shared_ptr<Module>& module) {
            return module->getCode() == moduleCode;
        }) != m_modules.end();
}

// Adds a session to the lecturer's list if it isn't already there
void Lecturer::addSession(std::shared_ptr<Session> session) {
    if (!teachesSession(session->getId())) {
        m_sessions.push_back(session);
    }
}

// Removes a session by ID, returns true if successful
bool Lecturer::removeSession(const std::string& sessionId) {
    auto it = std::find_if(m_sessions.begin(), m_sessions.end(),
        [&sessionId](const std::shared_ptr<Session>& session) {
            return session->getId() == sessionId;
        });
    if (it != m_sessions.end()) {
        m_sessions.erase(it);
        return true;
    }
    return false;
}

// Returns the list of sessions the lecturer teaches
const std::vector<std::shared_ptr<Session>>& Lecturer::getSessions() const {
    return m_sessions;
}

// Checks if the lecturer teaches a specific session
bool Lecturer::teachesSession(const std::string& sessionId) const {
    return std::find_if(m_sessions.begin(), m_sessions.end(),
        [&sessionId](const std::shared_ptr<Session>& session) {
            return session->getId() == sessionId;
        }) != m_sessions.end();
}