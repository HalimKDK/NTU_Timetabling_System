/**
 * Implementation of the Session class.
 *
 * This file defines the behaviour and logic for scheduling academic sessions.
 * It manages details such as the associated module, session type, room, lecturer,
 * time slot, week number, and the student groups attending. It also provides
 * conflict detection between sessions based on time, location, lecturer, and groups.
 */

#include "Session.h"
#include "Module.h"
#include "SessionType.h"
#include "Room.h"
#include "Lecturer.h"
#include "Group.h"
#include <algorithm>
#include <sstream>

 // Constructor
Session::Session(const std::string& id,
    const Module& module,
    const SessionType& sessionType,
    const Room& room,
    const Lecturer& lecturer,
    const TimeSlot& timeSlot,
    int weekNumber)
    : m_id(id),
    m_module(module),
    m_sessionType(sessionType),
    m_room(room),
    m_lecturer(lecturer),
    m_timeSlot(timeSlot),
    m_weekNumber(weekNumber) {
    // Clamp week number between 1 and 53
    if (m_weekNumber < 1) {
        m_weekNumber = 1;
    }
    else if (m_weekNumber > 53) {
        m_weekNumber = 53;
    }
}

// Destructor
Session::~Session() {
    // Nothing to manually clean up — shared_ptr handles group memory
}

// Return the session ID
const std::string& Session::getId() const {
    return m_id;
}

// Change the session ID
void Session::setId(const std::string& id) {
    m_id = id;
}

// Return the module for this session
const Module& Session::getModule() const {
    return m_module;
}

// Set a new module for this session
void Session::setModule(const Module& module) {
    const_cast<Module&>(m_module) = module;
}

// Return the session type
const SessionType& Session::getSessionType() const {
    return m_sessionType;
}

// Set a new session type
void Session::setSessionType(const SessionType& sessionType) {
    const_cast<SessionType&>(m_sessionType) = sessionType;
}

// Return the room
const Room& Session::getRoom() const {
    return m_room;
}

// Set a new room
void Session::setRoom(const Room& room) {
    const_cast<Room&>(m_room) = room;
}

// Return the lecturer
const Lecturer& Session::getLecturer() const {
    return m_lecturer;
}

// Set a new lecturer
void Session::setLecturer(const Lecturer& lecturer) {
    const_cast<Lecturer&>(m_lecturer) = lecturer;
}

// Return the time slot
const TimeSlot& Session::getTimeSlot() const {
    return m_timeSlot;
}

// Set a new time slot
void Session::setTimeSlot(const TimeSlot& timeSlot) {
    m_timeSlot = timeSlot;
}

// Return the week number
int Session::getWeekNumber() const {
    return m_weekNumber;
}

// Set a new week number, clamping it between 1 and 53
void Session::setWeekNumber(int weekNumber) {
    if (weekNumber < 1) {
        m_weekNumber = 1;
    }
    else if (weekNumber > 53) {
        m_weekNumber = 53;
    }
    else {
        m_weekNumber = weekNumber;
    }
}

// Add a group to this session if it's not already assigned
void Session::addGroup(std::shared_ptr<Group> group) {
    if (!hasGroup(group->getCode())) {
        m_groups.push_back(group);
    }
}

// Remove a group from this session by group code
bool Session::removeGroup(const std::string& groupCode) {
    auto it = std::find_if(m_groups.begin(), m_groups.end(),
        [&groupCode](const std::shared_ptr<Group>& group) {
            return group->getCode() == groupCode;
        });

    if (it != m_groups.end()) {
        m_groups.erase(it);
        return true;
    }

    return false;
}

// Return the list of groups assigned to this session
const std::vector<std::shared_ptr<Group>>& Session::getGroups() const {
    return m_groups;
}

// Check if a group with a given code is assigned to this session
bool Session::hasGroup(const std::string& groupCode) const {
    return std::find_if(m_groups.begin(), m_groups.end(),
        [&groupCode](const std::shared_ptr<Group>& group) {
            return group->getCode() == groupCode;
        }) != m_groups.end();
}

// Check for scheduling conflicts with another session
bool Session::conflictsWith(const Session& other) const {
    // Sessions in different weeks never conflict
    if (m_weekNumber != other.m_weekNumber) {
        return false;
    }

    // No conflict if time slots don’t overlap
    if (!m_timeSlot.overlaps(other.m_timeSlot)) {
        return false;
    }

    // Conflict if using the same room at the same time
    if (m_room.getNumber() == other.m_room.getNumber()) {
        return true;
    }

    // Conflict if the same lecturer is assigned at the same time
    if (m_lecturer.getId() == other.m_lecturer.getId()) {
        return true;
    }

    // Conflict if any group is assigned to both sessions
    for (const auto& group : m_groups) {
        if (other.hasGroup(group->getCode())) {
            return true;
        }
    }

    // No conflicts detected
    return false;
}

// Return a string summary of this session's details
std::string Session::toString() const {
    std::stringstream ss;

    // Build a comma-separated list of group codes
    std::string groupList;
    for (size_t i = 0; i < m_groups.size(); ++i) {
        if (i > 0) {
            groupList += ", ";
        }
        groupList += m_groups[i]->getCode();
    }

    ss << "Week " << m_weekNumber << ": "
        << m_module.getCode() << " - " << m_sessionType.getName()
        << " (" << m_timeSlot.toString() << ") "
        << "in " << m_room.getFullName()
        << " with " << m_lecturer.getFullName();

    if (!m_groups.empty()) {
        ss << " for groups: " << groupList;
    }

    return ss.str();
}
