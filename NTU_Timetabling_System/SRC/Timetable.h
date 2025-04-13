/**
 *
 * This file defines the Timetable class, which manages collections of academic
 * sessions organized by week for a given academic year. It provides functionality
 * to add, remove, search, and check for conflicts between scheduled sessions.
 */

#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <vector>
#include <memory>
#include <string>
#include <map>

 // Forward declaration of the Session class
class Session;

/**
 * The Timetable class manages a schedule of academic sessions,
 * organized by week numbers in the academic year.
 */
class Timetable {
public:
    // Creates a new Timetable for the given academic year (e.g., "2024-2025")
    Timetable(const std::string& academicYear);

    // Destructor
    ~Timetable();

    // Returns the academic year for this timetable
    const std::string& getAcademicYear() const;

    // Sets a new academic year
    void setAcademicYear(const std::string& academicYear);

    // Adds a session to the timetable, returning true if it was added successfully
    // or false if there was a conflict
    bool addSession(std::shared_ptr<Session> session);

    // Removes a session from the timetable by its ID
    // Returns true if the session was removed, or false if it wasn’t found
    bool removeSession(const std::string& sessionId);

    // Finds and returns a session by its ID, or nullptr if not found
    std::shared_ptr<Session> getSession(const std::string& sessionId) const;

    // Returns all sessions scheduled for a specific week
    std::vector<std::shared_ptr<Session>> getSessionsForWeek(int weekNumber) const;

    // Returns all sessions associated with a specific module code
    std::vector<std::shared_ptr<Session>> getSessionsForModule(const std::string& moduleCode) const;

    // Returns all sessions scheduled in a specific room
    std::vector<std::shared_ptr<Session>> getSessionsForRoom(const std::string& roomNumber) const;

    // Returns all sessions taught by a specific lecturer
    std::vector<std::shared_ptr<Session>> getSessionsForLecturer(const std::string& lecturerId) const;

    // Returns all sessions assigned to a specific student group
    std::vector<std::shared_ptr<Session>> getSessionsForGroup(const std::string& groupCode) const;

    // Returns all sessions for a specific module and student group
    std::vector<std::shared_ptr<Session>> getSessionsForModuleAndGroup(
        const std::string& moduleCode, const std::string& groupCode) const;

    // Returns all sessions scheduled in a specific room for a student group
    std::vector<std::shared_ptr<Session>> getSessionsForRoomAndGroup(
        const std::string& roomNumber, const std::string& groupCode) const;

    // Returns all sessions taught by a specific lecturer for a student group
    std::vector<std::shared_ptr<Session>> getSessionsForLecturerAndGroup(
        const std::string& lecturerId, const std::string& groupCode) const;

    // Returns all sessions for a specific week and student group
    std::vector<std::shared_ptr<Session>> getSessionsForWeekAndGroup(
        int weekNumber, const std::string& groupCode) const;

    // Returns all pairs of sessions that have scheduling conflicts in this timetable
    std::vector<std::pair<std::shared_ptr<Session>, std::shared_ptr<Session>>> findConflicts() const;

    // Checks whether a session conflicts with any existing sessions in the timetable
    // Returns a list of conflicting sessions, or an empty list if there are none
    std::vector<std::shared_ptr<Session>> checkForConflicts(const Session& session) const;

    // Returns a list of all sessions in the timetable
    std::vector<std::shared_ptr<Session>> getAllSessions() const;

    // Removes all sessions from the timetable
    void clear();

    // Returns the total number of sessions currently in the timetable
    size_t getSessionCount() const;

private:
    std::string m_academicYear;  // Academic year (e.g., "2024-2025")
    std::map<int, std::vector<std::shared_ptr<Session>>> m_sessionsByWeek;  // Sessions grouped by week number
    std::map<std::string, std::shared_ptr<Session>> m_sessionsById;         // Sessions accessible by ID
};

#endif // TIMETABLE_H

