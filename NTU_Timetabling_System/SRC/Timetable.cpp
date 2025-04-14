/**
 *
 * This file contains the implementation of the Timetable class.
 * It handles storing, managing, searching, and checking for conflicts
 * in a collection of academic sessions, organized by week.
 */


#include "Timetable.h"
#include "Session.h"
#include "Module.h"  
#include "Room.h"
#include "Lecturer.h"   
#include "Group.h"
#include <algorithm>
#include <iomanip>

 // Constructor: sets the academic year
Timetable::Timetable(const std::string& academicYear)
    : m_academicYear(academicYear) {
    // Nothing else needs to be initialized here
}

// Destructor: smart pointers clean up automatically
Timetable::~Timetable() {
    // No manual cleanup needed — shared_ptr takes care of it
}

// Returns the academic year for this timetable
const std::string& Timetable::getAcademicYear() const {
    return m_academicYear;
}

// Sets a new academic year for the timetable
void Timetable::setAcademicYear(const std::string& academicYear) {
    m_academicYear = academicYear;
}

// Adds a session to the timetable if it doesn't conflict with others
bool Timetable::addSession(std::shared_ptr<Session> session) {
    // Check for any conflicts with existing sessions
    std::vector<std::shared_ptr<Session>> conflicts = checkForConflicts(*session);
    if (!conflicts.empty()) {
        return false;  // Conflict found — don't add it
    }

    // Add to both maps (by week and by ID)
    int weekNumber = session->getWeekNumber();
    m_sessionsByWeek[weekNumber].push_back(session);
    m_sessionsById[session->getId()] = session;

    return true;
}

// Removes a session from the timetable using its ID
bool Timetable::removeSession(const std::string& sessionId) {
    // Try to find the session by its ID
    auto it = m_sessionsById.find(sessionId);
    if (it == m_sessionsById.end()) {
        return false;  // Not found
    }

    std::shared_ptr<Session> session = it->second;
    int weekNumber = session->getWeekNumber();

    // Remove it from the weekly list
    auto& weekSessions = m_sessionsByWeek[weekNumber];
    weekSessions.erase(
        std::remove(weekSessions.begin(), weekSessions.end(), session),
        weekSessions.end()
    );

    // If that week is now empty, remove the entry for the week
    if (weekSessions.empty()) {
        m_sessionsByWeek.erase(weekNumber);
    }

    // Finally, remove it from the ID map
    m_sessionsById.erase(sessionId);

    return true;
}

// Looks up a session by its ID
std::shared_ptr<Session> Timetable::getSession(const std::string& sessionId) const {
    auto it = m_sessionsById.find(sessionId);
    if (it != m_sessionsById.end()) {
        return it->second;
    }

    return nullptr;  // Not found
}

// Gets all sessions scheduled in a particular week
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForWeek(int weekNumber) const {
    auto it = m_sessionsByWeek.find(weekNumber);
    if (it != m_sessionsByWeek.end()) {
        return it->second;
    }

    return {};  // No sessions for this week
}

// Gets all sessions for a specific module
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForModule(const std::string& moduleCode) const {
    std::vector<std::shared_ptr<Session>> result;

    // Use traditional iterator instead of structured binding
    for (auto it = m_sessionsById.begin(); it != m_sessionsById.end(); ++it) {
        const std::string& id = it->first;
        const std::shared_ptr<Session>& session = it->second;

        if (session->getModule().getCode() == moduleCode) {
            result.push_back(session);
        }
    }

    return result;
}

// Gets all sessions scheduled in a specific room
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForRoom(const std::string& roomNumber) const {
    std::vector<std::shared_ptr<Session>> result;

    // Use traditional iterator instead of structured binding
    for (auto it = m_sessionsById.begin(); it != m_sessionsById.end(); ++it) {
        const std::string& id = it->first;
        const std::shared_ptr<Session>& session = it->second;

        if (session->getRoom().getNumber() == roomNumber) {
            result.push_back(session);
        }
    }

    return result;
}

// Gets all sessions being taught by a specific lecturer
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForLecturer(const std::string& lecturerId) const {
    std::vector<std::shared_ptr<Session>> result;

    // Use traditional iterator instead of structured binding
    for (auto it = m_sessionsById.begin(); it != m_sessionsById.end(); ++it) {
        const std::string& id = it->first;
        const std::shared_ptr<Session>& session = it->second;

        if (session->getLecturer().getId() == lecturerId) {
            result.push_back(session);
        }
    }

    return result;
}

// Gets all sessions attended by a specific group
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForGroup(const std::string& groupCode) const {
    std::vector<std::shared_ptr<Session>> result;

    // Use traditional iterator instead of structured binding
    for (auto it = m_sessionsById.begin(); it != m_sessionsById.end(); ++it) {
        const std::string& id = it->first;
        const std::shared_ptr<Session>& session = it->second;

        if (session->hasGroup(groupCode)) {
            result.push_back(session);
        }
    }

    return result;
}

// Gets sessions for a particular module and group
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForModuleAndGroup(
    const std::string& moduleCode, const std::string& groupCode) const {
    std::vector<std::shared_ptr<Session>> result;

    // Use traditional iterator instead of structured binding
    for (auto it = m_sessionsById.begin(); it != m_sessionsById.end(); ++it) {
        const std::string& id = it->first;
        const std::shared_ptr<Session>& session = it->second;

        if (session->getModule().getCode() == moduleCode && session->hasGroup(groupCode)) {
            result.push_back(session);
        }
    }

    return result;
}

// Gets sessions for a particular room and group
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForRoomAndGroup(
    const std::string& roomNumber, const std::string& groupCode) const {
    std::vector<std::shared_ptr<Session>> result;

    // Use traditional iterator instead of structured binding
    for (auto it = m_sessionsById.begin(); it != m_sessionsById.end(); ++it) {
        const std::string& id = it->first;
        const std::shared_ptr<Session>& session = it->second;

        if (session->getRoom().getNumber() == roomNumber && session->hasGroup(groupCode)) {
            result.push_back(session);
        }
    }

    return result;
}

// Gets sessions for a specific lecturer and group
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForLecturerAndGroup(
    const std::string& lecturerId, const std::string& groupCode) const {
    std::vector<std::shared_ptr<Session>> result;

    // Use traditional iterator instead of structured binding
    for (auto it = m_sessionsById.begin(); it != m_sessionsById.end(); ++it) {
        const std::string& id = it->first;
        const std::shared_ptr<Session>& session = it->second;

        if (session->getLecturer().getId() == lecturerId && session->hasGroup(groupCode)) {
            result.push_back(session);
        }
    }

    return result;
}

// Gets sessions for a particular week and group
std::vector<std::shared_ptr<Session>> Timetable::getSessionsForWeekAndGroup(
    int weekNumber, const std::string& groupCode) const {
    std::vector<std::shared_ptr<Session>> result;

    auto weekSessions = getSessionsForWeek(weekNumber);
    for (const auto& session : weekSessions) {
        if (session->hasGroup(groupCode)) {
            result.push_back(session);
        }
    }

    return result;
}

// Finds all conflicts between sessions in the timetable
std::vector<std::pair<std::shared_ptr<Session>, std::shared_ptr<Session>>> Timetable::findConflicts() const {
    std::vector<std::pair<std::shared_ptr<Session>, std::shared_ptr<Session>>> conflicts;

    // Get all sessions currently in the timetable
    std::vector<std::shared_ptr<Session>> allSessions = getAllSessions();

    // Check every unique pair for conflicts
    for (size_t i = 0; i < allSessions.size(); ++i) {
        for (size_t j = i + 1; j < allSessions.size(); ++j) {
            if (allSessions[i]->conflictsWith(*allSessions[j])) {
                conflicts.emplace_back(allSessions[i], allSessions[j]);
            }
        }
    }

    return conflicts;
}

// Checks if a given session would conflict with existing ones
std::vector<std::shared_ptr<Session>> Timetable::checkForConflicts(const Session& session) const {
    std::vector<std::shared_ptr<Session>> conflicts;

    // Get other sessions in the same week
    auto weekSessions = getSessionsForWeek(session.getWeekNumber());

    // Check them for conflicts
    for (const auto& existingSession : weekSessions) {
        if (session.conflictsWith(*existingSession)) {
            conflicts.push_back(existingSession);
        }
    }

    return conflicts;
}

// Returns a list of all sessions in the timetable
std::vector<std::shared_ptr<Session>> Timetable::getAllSessions() const {
    std::vector<std::shared_ptr<Session>> allSessions;
    allSessions.reserve(m_sessionsById.size());

    // Use traditional iterator instead of structured binding
    for (auto it = m_sessionsById.begin(); it != m_sessionsById.end(); ++it) {
        const std::string& id = it->first;
        const std::shared_ptr<Session>& session = it->second;

        allSessions.push_back(session);
    }

    return allSessions;
}

// Removes all sessions from the timetable
void Timetable::clear() {
    m_sessionsByWeek.clear();
    m_sessionsById.clear();
}

// Returns the total number of sessions in the timetable
size_t Timetable::getSessionCount() const {
    return m_sessionsById.size();
}